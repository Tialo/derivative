#include <functions/functions.h>

// CONSTANT

std::string Num::repr() {
	std::string str = std::to_string(value);
	size_t s = str.find_last_not_of('0');
	if (str[s] == '.') return str.substr(0, s);
	return str.substr(0, s + 1);
}

// +

std::string Sum::repr() {
	std::string arg1_repr(arg1->repr()), arg2_repr(arg2->repr());
	if (arg1_repr == arg2_repr && arg2_repr == "0") return "0";
	if (arg1_repr == "0") return arg2_repr;
	if (arg2_repr == "0") return arg1_repr;
	return arg1_repr + " + " + arg2_repr;
}

// -

std::string Sub::repr() {
	std::string arg1_repr(arg1->repr()), arg2_repr(arg2->repr());
	if (arg1_repr == arg2_repr && arg2_repr == "0") return "0";
	if (arg1_repr == "0") return "-" + arg2_repr;
	if (arg2_repr == "0") return arg1_repr;
	std::string s2 = arg2->order < 5 && arg2->order > 0 ? "(" + arg2_repr + ")" : arg2_repr;
	return arg1_repr + " - " + s2;
}

// *

Mult::Mult(Func* f1, Func* f2) : arg1(f1), arg2(f2) {
	std::string arg1_repr(arg1->repr()), arg2_repr(arg2->repr());
	if (arg1_repr == "0" || arg2_repr == "0") order = 0;
	else if (arg1_repr == "1") order = arg2->order;
	else if (arg2_repr == "1") order = arg1->order;
	else order = 3;
}

Func* Mult::Der() {
	return new Sum(
		new Mult(arg1->Der(), arg2),
		new Mult(arg1, arg2->Der())
	);
}

std::string Mult::repr() {
	std::string arg1_repr(arg1->repr()), arg2_repr(arg2->repr());
	if (arg1_repr == "0" || arg2_repr == "0") return "0";
	if (arg1_repr == "1") return arg2_repr;
	if (arg2_repr == "1") return arg1_repr;
	std::string s1 = arg1->order < 3 && arg1->order > 0 ? "(" + arg1_repr + ")" : arg1_repr;
	std::string s2 = arg2->order < 4 && arg2->order > 0 ? "(" + arg2_repr + ")" : arg2_repr;
	return s1 + " * " + s2;
}

// /

std::string Division::repr() {
	std::string arg1_repr(arg1->repr());
	if (arg1_repr == "0") return "0";
	std::string arg2_repr(arg2->repr());
	if (arg2_repr == "1") return arg1_repr;
	std::string s1 = arg1->order < 3 && arg1->order > 1 ? "(" + arg1_repr + ")" : arg1_repr;
	std::string s2 = arg2->order < 4 && arg2->order > 0 ? "(" + arg2_repr + ")" : arg2_repr;
	return s1 + " / " + s2;
}

Func* Division::Der() {
	return new Division(
		new Sub(
			new Mult(arg1->Der(), arg2),
			new Mult(arg1, arg2->Der())
		),
		new Pow(arg2, new Num(2))
	);
}

// SIN

Func* Sin::Der() {
	return new Mult(new Cos(arg), arg->Der());
}

// COS

Func* Cos::Der() {
	return new Sub(new Num(0), new Mult(new Sin(arg), arg->Der()));
}

// TG

Func* Tg::Der() {
	return new Division(
		arg->Der(),
		new Pow(new Cos(arg), new Num(2))
	);
}

// CTG

Func* Ctg::Der() {
	return new Sub(
		new Num(0),
		new Division(
			arg->Der(),
			new Pow(new Sin(arg), new Num(2))
		)
	);
}

// LN

std::string Ln::repr() {
	std::string arg_repr(arg->repr());
	if (arg_repr == "e") return "1";
	if (arg_repr == "1") return "0";
	return "ln(" + arg_repr + ")";
}

// LG

Func* Lg::Der() {
	return new Division(
		arg->Der(),
		new Mult(arg, new Ln(new Num(10)))
	);
}

std::string Lg::repr() {
	std::string arg_repr(arg->repr());
	if (arg_repr == "10") return "1";
	if (arg_repr == "1") return "0";
	return "lg(" + arg_repr + ")";
}

// POWER

Func* Pow::Der() {
	return new Sum(
		new Mult(
			new Mult(
				new Pow(
					base,
					new Sub(arg, new Num(1.0f))
				),
				arg
			),
			base->Der()
		),
		new Mult(
			new Mult(arg->Der(), new Ln(base)),
			new Pow(base, arg)
		)
	);
}

std::string Pow::repr() {
	std::string arg_repr(arg->repr());
	std::string base_repr(base->repr());
	if (arg_repr == "1") return base_repr;
	if (arg_repr == "0") return "1";
	if (base_repr == "0") return "0";
	std::string s1 = base->order < 4 && base->order > 0 ? "(" + base_repr + ")" : base_repr;
	std::string s2 = arg->order < 5 && arg->order > 0 ? "(" + arg_repr + ")" : arg_repr;
	return s1 + " ^ " + s2;
}

// SQRT

Func* Sqrt::Der() {
	return new Division(
		arg->Der(),
		new Mult(new Num(2), new Sqrt(arg))
	);
}

std::string Sqrt::repr() {
	std::string arg_repr(arg->repr());
	if (arg_repr == "0") return "0";
	return "sqrt(" + arg_repr + ")";
}
