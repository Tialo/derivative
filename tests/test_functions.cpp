#include <iostream>

#include <functions/functions.cpp>

int main() {
	Func* num(new Num(4));
	Func* cos(new Cos(num));
	std::cout << cos->repr() << '\n';
	Func* exp(new Pow(new E(), new X()));
	Func* sum(new Sum(cos, exp));
	std::cout << sum->repr() << '\n';
	std::cout << sum->Der()->repr() << '\n';
}
