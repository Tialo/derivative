#include <iostream>

#include <parser/parser.cpp>

int main() {
	simpleparser::Tokenizer tz;
	auto x = tz.Tokenize("(x + 22) * cos(x / 2)");
	for (auto s : x) {
		std::cout << s.text << " | ";
	}
	std::cout << "\n\n";
	simpleparser::Parser parser("x*x*(x^10)+15*sin(x)");
	Func* f(parser.Parse());
	std::cout << f->repr() << '\n';
	std::cout << f->Der()->repr() << "\n\n";
	simpleparser::Parser p("cos(x)^x");
	Func* g(p.Parse());
	std::cout << g->Der()->repr() << "\n\n";
}