#ifndef CALC2_HPP

#include <cstdio>
#include <iostream>
#include <cmath>
#include "General.hpp"
#include <list>
#include<stdexcept>
#include <string.h>

using namespace std;

namespace Calculator{

     class MathT{
        public:
         MathT(){}
         static int TryRead(char* s, MathT** out, bool wasOperator);
         bool TryGetNumber(double* out);
         virtual bool IsNumber();
         virtual bool IsOperator();
         virtual bool IsExpression();
         virtual bool IsBracket();
         virtual bool IsMultiplyOrDivide();
          virtual ostream& Print(ostream& o) = 0;
    };
    ostream& operator<<(MathT& mt, ostream& o);
    ostream& operator<<(ostream& o, MathT& mt);

     class Number : public MathT{
     protected:
        Number(){}
         public:
         double value;
         Number(int val);
         Number(float val);
         Number(double val);
         virtual double GetValue();
          bool IsNumber();
         static int TryRead(char* s, Number** out);
           ostream& Print(ostream& o);
    };

     class Operator : public MathT{
         public:
         Operator(){}
          bool IsOperator();
         static int TryRead(char* s, Operator** out);
         virtual double Operate(double lhs, double rhs) = 0;
         Number* Operate(Number* lhs, Number* rhs);
    };

     class Add : public Operator{
         public:
         Add(){}
          double Operate(double lhs, double rhs);
          ostream& Print(ostream& o);
    };
     class Subtract : public Operator{
         public:
         Subtract(){}
          double Operate(double lhs, double rhs);
          ostream& Print(ostream& o);
    };
     class Multiply : public Operator{
         public:
         Multiply(){}
          bool IsMultiplyOrDivide();
          double Operate(double lhs, double rhs);
          ostream& Print(ostream& o);
    };
     class Divide : public Operator{
         public:
         Divide(){}
          bool IsMultiplyOrDivide();
          double Operate(double lhs, double rhs);
          ostream& Print(ostream& o);
    };

     class Expression : public Number{
         public:
         list<MathT*> members;
         Expression(){}
         ~Expression();
          bool IsNumber();
          bool IsExpression();
         int TryRead(char* s);
         static int TryRead(char* s, Expression** out);
         //Number* SolveNumber();
         double SolveDouble();
         double GetValue();
          ostream& Print( ostream& o);
    };

    ostream& operator<<(ostream& o, Expression& x);
    ostream& operator<<(Expression& x, ostream& o);

     class Bracket : public Expression{
         public:
         Bracket(){}
          bool IsBracket();
         static int TryRead(char* s, Bracket** out);
          ostream& Print(ostream& o);
    };
     bool IsNullOrEmpty(char* s);
     int PreceedingSpaces(char* s);
     bool IsNumber(char c);
     bool HasNumber(char* c);
}

#define CALC2_HPP
#endif
