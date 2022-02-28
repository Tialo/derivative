#ifndef FUNCTIONS_FUNCTIONS_H_20221801
#define FUNCTIONS_FUNCTIONS_H_20221801

#include <string>

/*!
\brief Абстрактный класс, имеющий методы Der(), repr().

Пример создания и использования
\code
#include <iostream>

#include <functions/functions.cpp>

int main(){
	Func* num(new Num(4));
	Func* cos(new Cos(num));
	std::cout << cos->repr() << '\n';
	std::cout << cos->Der()->repr() << '\n';
}
\endcode
*/
class Func {
public:
	/// Конструктор по умолчанию
	Func() = default;
	/// Конструктор копирования
	Func(const Func&) = default;
	/// Конструктор перемещающего копирования
	Func(Func&&) = default;
	/// Оператор копирующего присваивания
	Func& operator=(const Func&) = default;
	/// Оператор перемещающего присваивания
	Func& operator=(Func&&) = default;
	/// Виртуальный деструктор
	virtual ~Func() = default;
	/*!
	Метод вычисляет производную для текущего экземпляра

	\return Func* производная функции
	*/
	virtual Func* Der() = 0;
	/*!
	Метод получает строковую репрезентацию функции

	\return string строковая репрезентация
	*/
	virtual std::string repr() = 0;
	/// Приоритет операции
	int order{ 0 };
};

// CONSTANT

/*!
\brief Класс наследующийся от класса Func. Является числом
*/
class Num : public Func {
public:
	Num(const float v) : value(v) { order = 0; }
	/*!
	\f$Num' = 0\f$
	*/
	Func* Der() override { return new Num(0); }
	std::string repr() override;
private:
	float value{ 0.0f };
};

/*!
\brief Класс наследующийся от класса Func. Является числом, равным константе Эйлера
*/
class E : public Func {
public:
	E() { order = 0; }
	/*!
	\f$E' = 0\f$
	*/
	Func* Der() override { return new Num(0); }
	std::string repr() override { return "e"; }
};

/*!
\brief Класс наследующийся от класса Func. Является числом, равным константе Пи
*/
class PI : public Func {
public:
	PI() { order = 0; }
	/*!
	\f$Pi' = 0\f$
	*/
	Func* Der() override { return new Num(0); }
	std::string repr() override { return "pi"; }
};

// X

/*!
\brief Класс наследующийся от класса Func. Является независимой переменной
*/
class X : public Func {
public:
	X() { order = 0; }
	/*!
	\f$X' = 1\f$
	*/
	Func* Der() override { return new Num(1); }
	std::string repr() override { return "x"; }
};

// BINARY OPERATORS

/*!
\brief Класс наследующийся от класса Func. Является суммой двух функций
*/
class Sum : public Func {
public:
	Sum(Func* f1, Func* f2) : arg1(f1), arg2(f2) { order = (f1->repr() == "0" || f2->repr() == "0" ? 0 : 2); }
	/*!
	\f$(a + b)' = a' + b'\f$
	*/
	Func* Der() override { return new Sum(arg1->Der(), arg2->Der()); }
	std::string repr() override;
private:
	Func* arg1, * arg2;
};

/*!
\brief Класс наследующийся от класса Func. Является разностью двух функций
*/
class Sub : public Func {
public:
	Sub(Func* f1, Func* f2) : arg1(f1), arg2(f2) { order = (f1->repr() == "0" ? 1 : 2); }
	/*!
	\f$(a - b)' = a' - b'\f$
	*/
	Func* Der() override { return new Sub(arg1->Der(), arg2->Der()); }
	std::string repr() override;
private:
	Func* arg1, * arg2;
};

/*!
\brief Класс наследующийся от класса Func. Является произведением двух функций
*/
class Mult : public Func {
public:
	Mult(Func* f1, Func* f2);
	/*!
	\f$(ab)' = a'b + ab'\f$
	*/
	Func* Der() override;
	std::string repr() override;
private:
	Func* arg1, * arg2;
};

/*!
\brief Класс наследующийся от класса Func. Является частным двух функций
*/
class Division : public Func {
public:
	Division(Func* f1, Func* f2) : arg1(f1), arg2(f2) { order = 3; }
	/*!
	\f$(a/b)' = (a'b - ab') / b^2\f$
	*/
	Func* Der() override;
	std::string repr() override;
private:
	Func* arg1, * arg2;
};

// TRIGINOMETRY

/*!
\brief Класс наследующийся от класса Func. Является синусом функции
*/
class Sin : public Func {
public:
	Sin(Func* f) : arg(f) { order = 5; }
	Func* Der() override;
	/*!
	\f$sin(a)' = cos(a)a'\f$
	*/
	std::string repr() override { return "sin(" + arg->repr() + ")"; }
private:
	Func* arg;
};

/*!
\brief Класс наследующийся от класса Func. Является косинусом функции
*/
class Cos : public Func {
public:
	Cos(Func* f) : arg(f) { order = 5; }
	/*!
	\f$cos(a)' = -sin(a)a'\f$
	*/
	Func* Der() override;
	std::string repr() override { return "cos(" + arg->repr() + ")"; }
private:
	Func* arg;
};

/*!
\brief Класс наследующийся от класса Func. Является тангенсом функции
*/
class Tg : public Func {
public:
	Tg(Func* f) : arg(f) { order = 5; }
	/*!
	\f$tg(a)' = a'/cos^2(a)\f$
	*/
	Func* Der() override;
	std::string repr() override { return "tg(" + arg->repr() + ")"; }
private:
	Func* arg;
};

/*!
\brief Класс наследующийся от класса Func. Является котангенсом функции
*/
class Ctg : public Func {
public:
	Ctg(Func* f) : arg(f) { order = 5; }
	/*!
	\f$ctg(a)' = a'/sin^2(a)\f$
	*/
	Func* Der() override;
	std::string repr() override { return "ctg(" + arg->repr() + ")"; }
private:
	Func* arg;
};


// LOG

/*!
\brief Класс наследующийся от класса Func. Является натуральным логарифмом функции
*/
class Ln : public Func {
public:
	Ln(Func* f) : arg(f) { order = 5; }
	/*!
	\f$ln(a)' = a'/a\f$
	*/
	Func* Der() override { return new Division(arg->Der(), arg); }
	std::string repr() override;
private:
	Func* arg;
};

/*!
\brief Класс наследующийся от класса Func. Является десятичным логарифмом функции
*/
class Lg : public Func {
public:
	Lg(Func* f) : arg(f) { order = 5; }
	/*!
	\f$lg(a)' = a'/aln10\f$
	*/
	Func* Der() override;
	std::string repr() override;
private:
	Func* arg;
};

// POWER

/*!
\brief Класс наследующийся от класса Func. Является показательно-степенной функцией
*/
class Pow : public Func {
public:
	Pow(Func* f1, Func* f2) : base(f1), arg(f2) { order = 4; }
	/*!
	\f$(a^b)' = ba^{b-1}a'+b'a^blna\f$
	*/
	Func* Der() override;
	std::string repr() override;
private:
	Func* base, * arg;
};

/*!
\brief Класс наследующийся от класса Func. Является квадратным корнем функции
*/
class Sqrt : public Func {
public:
	Sqrt(Func* f) : arg(f) { order = 5; }
	/*!
	\f$sqrt(a)' = a'/2sqrt(a)\f$
	*/
	Func* Der() override;
	std::string repr() override;
private:
	Func* arg;
};

#endif // !FUNCTIONS_FUNCTIONS_H_20221801
