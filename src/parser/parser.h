#ifndef PARSER_PARSER_H_20211229
#define PARSER_PARSER_H_20211229

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include <functions/functions.h>

/// Пространстов имен, содержащее классы, нужные для парсинга
namespace simpleparser {

	/// Перечисление, содержащее все виды токенов
	enum class TokenType {
		WHITESPACE,
		NUMERICAL,
		PLUS,
		MINUS,
		MULT,
		DIVISION,
		POWER,
		PI,
		E,
		X,
		OPEN_BRACKET,
		CLOSED_BRACKET,
		SIN,
		COS,
		TG,
		CTG,
		SQRT,
		LN,
		LG,
		NONE
	};
	/*!
	\brief Класс содержащий описание токенов 

	Token атомарная единица парсинга строки в математическую функцию, будь то 
	числа, простейшие функции, операторы, скобки

	*/
	class Token {
	public:
		/// Конструктор по умолчанию
		Token() = default;
		/// Конструктор копирования
		Token(const Token&) = default;
		/// Конструктор перемещающего копирования
		Token(Token&&) = default;
		/// Оператор копирующего присваивания
		Token& operator=(const Token&) = default;
		/// Оператор перемещающего присваивания
		Token& operator=(Token&&) = default;
		/// Деструктор
		~Token() = default;
		/// Тип токена
		enum class TokenType type { TokenType::WHITESPACE };
		/*!
		* \brief Метод находит соответствующий тексту тип токена
		* 
		* При отсутствии соответствующего типа возвращает TokenType::NONE
		* \return enum class TokenType
		*/
		const enum class TokenType found() const noexcept;
		/*!
		* \brief Текстовая репрезентация токена
		*/
		std::string text{ "" };
	private:
		std::unordered_map<std::string, enum class TokenType> names{
			{"sin", TokenType::SIN}, {"cos", TokenType::COS}, 
			{"x", TokenType::X}, {"ln", TokenType::LN},
			{"lg", TokenType::LG}, {"sqrt", TokenType::SQRT}, 
			{"tg", TokenType::TG}, {"ctg", TokenType::CTG}, 
			{"^", TokenType::POWER}, {"+", TokenType::PLUS}, 
			{"-", TokenType::MINUS}, {"/", TokenType::DIVISION}, 
			{"*", TokenType::MULT}, {"e", TokenType::E},
			{"pi", TokenType::PI},
			{"(", TokenType::OPEN_BRACKET},
			{"[", TokenType::OPEN_BRACKET},
			{"{", TokenType::OPEN_BRACKET},
			{"}", TokenType::CLOSED_BRACKET},
			{"]", TokenType::CLOSED_BRACKET},
			{")", TokenType::CLOSED_BRACKET}
		};
	};
	/*!
	\brief Класс нужный для разбиения строки на токены

	Пример создания и использования
	\code
	#include <iostream>

	#include <parser/parser.cpp>

	int main(){
		simpleparser::Tokenizer tz;
		auto x = tz.Tokenize("(x + 22) * cos(x / 2)");
		for (auto s : x) {
			std::cout << s.text << " | ";
		}
		std::cout << "\n\n";
	}
	\endcode
	*/
	class Tokenizer {
	public:
		/// Конструктор по умолчанию
		Tokenizer() = default;
		/// Конструктор копирования
		Tokenizer(const Tokenizer&) = default;
		/// Конструктор перемещающего копирования
		Tokenizer(Tokenizer&&) = default;
		/// Оператор копирующего присваивания
		Tokenizer& operator=(const Tokenizer&) = default;
		/// Оператор перемещающего присваивания
		Tokenizer& operator=(Tokenizer&&) = default;
		/// Деструктор
		~Tokenizer() = default;
		/*!
		\brief Метод разбивающий строку на токены
		\param[in] std::string
		\throws std::runtime_error - при неправильном вводе чисел
		\return std::vector<Token>
		*/
		std::vector<Token> Tokenize(const std::string& str);
	private:
		void endToken(Token& token, std::vector<Token>& tokens);
	};

	/*!
	\brief Класс нужный для составления математической функции из набора токенов

	Пример создания и использования
	\code
	#include <iostream>

	#include <parser/parser.cpp>

	int main(){
		simpleparser::Parser parser("x*x*(x^10)+15*sin(x)");
		Func* f(parser.Parse());
		simpleparser::Parser p("cos(x)^x");
		Func* g(p.Parse());
	}
	\endcode
	*/
	class Parser {
	public:
		/// Конструктор по умолчанию
		Parser() = default;
		/// Конструктор копирования
		Parser(const Parser&) = default;
		/// Конструктор перемещающего копирования
		Parser(Parser&&) = default;
		/// Оператор копирующего присваивания
		Parser& operator=(const Parser&) = default;
		/// Оператор перемещающего присваивания
		Parser& operator=(Parser&&) = default;
		/// Деструктор
		~Parser() = default;
		/*!
		\brief Конструктор класса
		\param[in] std::string
		*/
		Parser(const std::string& str);
		/*!
		\brief Метод парсит строку
		\throw std::runtime_error - при вводе пустой либо недопустимой функции
		\throw std::runtime_error - при вводе функции с несбалансированным количеством скобок
		\throw std::runtime_error - при вводе функции с отсутсвующим вторым операндом
		\throw std::runtime_error - при вводе функции с отсутствующим аргументом
		\return Func*. Возвращает математическую функцию 
		*/
		Func* Parse();
	private:
		std::vector<Token> tokens;
		int i{ -1 };
		std::unordered_map<enum class TokenType, int> orders{
			{TokenType::SIN, 4}, {TokenType::COS, 4}, 
			{TokenType::LN, 4}, {TokenType::LG, 4}, 
			{TokenType::SQRT, 4}, {TokenType::TG, 4}, 
			{TokenType::CTG, 4}, {TokenType::POWER, 3}, 
			{TokenType::MULT, 2}, {TokenType::DIVISION, 2}, 
			{TokenType::PLUS, 1}, {TokenType::MINUS, 1}, 
			{TokenType::X, 0}, {TokenType::NUMERICAL, 0}, 
			{TokenType::PI, 0}, {TokenType::E, 0}, 
			{TokenType::WHITESPACE, -1}
		};
	private:
		Token GetToken();
		Func* MakeFunc(Token t, Func* arg);
		Func* MakeFunc(Func* f1, Token op, Func* f2);
		Func* ParseSimpleExpression();
		Func* ParseBinaryExpression(int order);
	};
}

#endif !PARSER_PARSER_H_20211229