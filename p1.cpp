#include <iostream>
#include <math.h>
#include <string>
#include <cassert>
using namespace std;
struct Transformer;
struct Number;
struct BinaryOperation;
struct FunctionCall;
struct Variable;
struct Expression
{
	virtual ~Expression() { }
	virtual double evaluate() const = 0;
	virtual Expression* transform(Transformer* tr) const = 0;
	virtual string print() const = 0;
};

struct Transformer //pattern Visitor
{
	virtual ~Transformer() { }
	virtual Expression* transformNumber(Number const*) = 0;
	virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
	virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
	virtual Expression* transformVariable(Variable const*) = 0;
};

struct Number : Expression
{
	Number(double value) : value_(value) {}//конструктор
	double value() const { return value_; }// метод чтения значения числа
	double evaluate() const { return value_; }// реализация виртуального метода «вычислить»
	Expression* transform(Transformer* tr) const {return tr->transformNumber(this);}
	string print() const {return to_string(value_);}
private:
	double value_;
};

struct BinaryOperation : Expression
{
	enum {
		PLUS = '+',
		MINUS = '-',
		DIV = '/',
		MUL = '*'
	};
	BinaryOperation(Expression const* left, int op, Expression const* right) 
		: left_(left), op_(op), right_(right)
	{assert(left_ && right_);}
	~BinaryOperation() {
		delete left_;
		delete right_;
	}
	double evaluate() const {
		double left = left_->evaluate(); // вычисляем левую часть
		double right = right_->evaluate(); // вычисляем правую часть
		switch (op_) // в зависимости от вида операции, складываем, вычитаем, умножаем
					 // или делим левую и правую части

		{
		case PLUS: return left + right;
		case MINUS: return left - right;
		case DIV: return left / right;
		case MUL: return left * right;
		}
	}

	Expression* transform(Transformer* tr) const {return tr->transformBinaryOperation(this);}

	Expression const* left() const {return left_;}

	Expression const* right() const {return right_;}
	int operation() const {return op_;}

	std::string print() const {
		return left_->print() + string(1, op_) + this->right_->print();
	}


private:
	Expression const* left_;
	Expression const* right_;
	int op_;
};

struct FunctionCall : Expression// структура «Вызов функции»
{
	// в конструкторе  имя функции и ее аргумент
	FunctionCall(std::string const& name, Expression const* arg) : name_(name), arg_(arg) 
	{ assert(arg_);
	assert(name_ == "sqrt" || name_ == "abs"); } // разрешены только вызов
												//sqrt и abs
	~FunctionCall() {delete arg_;}// освобождаем память в деструкторе
	double evaluate() const // реализация виртуального метода
                                     //«вычислить»
	{
		if (name_ == "sqrt") {
			return sqrt(arg_->evaluate());
		}
		if (name_ == "abs") {
			return abs(arg_->evaluate());
		}
		return arg_->evaluate();
	}
	Expression* transform(Transformer* tr) const {
		return tr->transformFunctionCall(this);
	}
	string const& name() const {
		return name_;
	}
	Expression const* arg() const {
		return arg_;
	}

	string print() const {
		return name_ + "(" + arg_->print() + ")";
	}
private:
	string const name_;
	Expression const* arg_;
};

struct Variable : Expression // структура «Переменная»
{
	Variable(string const name) : name_(name) {}//имя переменной
	string const& name() const // чтение имени переменной
	{return name_;}
	double evaluate() const {// реализация виртуального метода «вычислить»
		return 0.0;
	}
	Expression* transform(Transformer* tr) const {
		return tr->transformVariable(this);
	}

	string print() const {
		return name_;
	}

private:
	string const name_;
};

/**
* реализуйте все необходимые методы класса
* вы можете определять любые вспомогательные
* методы, если хотите
*/

struct CopySyntaxTree : Transformer
{
	Expression* transformNumber(Number const* number)
	{
		return new Number(number->evaluate());
	}
	Expression* transformBinaryOperation(BinaryOperation const* binop)
	{
		return new BinaryOperation(binop->left()->transform(this), binop->operation(), binop->right()->transform(this));
	}
	Expression* transformFunctionCall(FunctionCall const* fcall)
	{
		return new FunctionCall(fcall->name(), fcall->arg()->transform(this));
	}
	Expression* transformVariable(Variable const* var)
	{
		return new Variable(var->name());
	}
};
int main() {
	//Проверьте работу CopySyntaxTree,
	//например, на таком коде :
	Number* n32 = new Number(32.0);
	Number* n16 = new Number(16.0);
	BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
	FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
	Variable* var = new Variable("var");
	BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
	FunctionCall* callAbs = new FunctionCall("abs", mult);
	CopySyntaxTree CST;
	Expression* newExpr = callAbs->transform(&CST);
	cout << newExpr->print();
}