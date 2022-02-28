#include <parser/parser.h>
#include <algorithm>

namespace simpleparser {
	// TOKEN
	const enum class TokenType Token::found() const noexcept {
		if (names.find(text) != names.end()) return names.find(text)->second;
		else return TokenType::NONE;
	}
	// TOKENIZER
	std::vector<Token> Tokenizer::Tokenize(const std::string& str) {
		std::vector<Token> tokens;
		Token currentToken;
		for (char ch : str) {
			if (ch == ' ') {
				endToken(currentToken, tokens);
				continue;
			}
			else if (std::any_of(currentToken.text.begin(), currentToken.text.end(), [](int c){ return isalpha(c); }) && isdigit(ch)) {
				throw std::runtime_error("Syntax error");
			}
			else if ((currentToken.type == TokenType::WHITESPACE || currentToken.type == TokenType::NUMERICAL) && isdigit(ch) || ch == '.') {
				currentToken.type = TokenType::NUMERICAL;
			}
			else if (currentToken.type == TokenType::NUMERICAL && !isdigit(ch) && ch != '.') {
				endToken(currentToken, tokens);
			}
			currentToken.text += ch;
			enum TokenType found = currentToken.found();
			if (found != TokenType::NONE) {
				currentToken.type = found;
				endToken(currentToken, tokens);
			}
		}
		endToken(currentToken, tokens);
		return tokens;
	}
	void Tokenizer::endToken(Token& token, std::vector<Token>& tokens) {
		if (token.type != TokenType::WHITESPACE) {
			tokens.push_back(token);
			token.type = TokenType::WHITESPACE;
			token.text.erase();
		}
	}
	// PARSER
	Parser::Parser(const std::string& str) {
		Tokenizer tz;
		tokens = tz.Tokenize(str);
	}
	Func* Parser::Parse() {
		if (tokens.size() == 0) throw std::runtime_error("Function is empty or not allowed");
		return ParseBinaryExpression(0);
	}
	Token Parser::GetToken() {
		if (i + 1 >= (int)tokens.size()) return Token();
		return tokens[++i];
	}
	Func* Parser::MakeFunc(Token t, Func* arg) {
		if (t.type == TokenType::MINUS) return new Sub(new Num(0.0f), arg);
		if (t.type == TokenType::SIN) return new Sin(arg);
		if (t.type == TokenType::COS) return new Cos(arg);
		if (t.type == TokenType::LN) return new Ln(arg);
		if (t.type == TokenType::LG) return new Lg(arg);
		if (t.type == TokenType::SQRT) return new Sqrt(arg);
		if (t.type == TokenType::TG) return new Tg(arg);
		if (t.type == TokenType::CTG) return new Ctg(arg);
		throw std::runtime_error("Function is not allowed");
	}
	Func* Parser::MakeFunc(Func* f1, Token op, Func* f2) {
		if (op.type == TokenType::PLUS) return new Sum(f1, f2);
		if (op.type == TokenType::MINUS) return new Sub(f1, f2);
		if (op.type == TokenType::MULT) return new Mult(f1, f2);
		if (op.type == TokenType::DIVISION) return new Division(f1, f2);
		if (op.type == TokenType::POWER) return new Pow(f1, f2);
		throw std::runtime_error("Function is not allowed");
	}
	Func* Parser::ParseSimpleExpression() {
		Token token = GetToken();
		if (token.type == TokenType::NUMERICAL) 
			return new Num(std::stof(token.text));
		if (token.type == TokenType::X) 
			return new X();
		if (token.type == TokenType::PI) 
			return new PI();
		if (token.type == TokenType::E) 
			return new E();
		if (token.type == TokenType::OPEN_BRACKET) {
			Func* result = ParseBinaryExpression(0);
			if (GetToken().type != TokenType::CLOSED_BRACKET) 
				throw std::runtime_error("Expected closing bracket");
			return result;
		}
		if (token.type == TokenType::WHITESPACE) 
			throw std::runtime_error("Second operand is missing");
		try {
			Func* argument = ParseSimpleExpression();
			return MakeFunc(token, argument);
		}
		catch (std::runtime_error e) {
			std::string what = e.what();
			if (what == "Second operand is missing")
				throw std::runtime_error("Missing argument for " + token.text + " function");
			else
				throw;
		}
	}
	Func* Parser::ParseBinaryExpression(int order) {
		Func* left_exp = ParseSimpleExpression();
		for (;;) {
			Token op = GetToken();
			if (orders[op.type] <= order) {
				--i;
				return left_exp;
			}
			Func* right_exp = ParseBinaryExpression(orders[op.type]);
			left_exp = MakeFunc(left_exp, op, right_exp);
		}
	}
}