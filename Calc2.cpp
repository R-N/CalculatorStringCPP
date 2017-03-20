#include "Calc2.hpp"
using namespace std;

namespace Calculator{
    //MathT
    int MathT::TryRead(char* s, MathT** out, bool wasOperator){
        if (IsNullOrEmpty(s))
            return 0;
        int ret;
        if (wasOperator){
            if (ret = Number::TryRead(s, (Number**)out)){
                return ret;
            }
            if (ret = Bracket::TryRead(s, (Bracket**)out)){
                return ret;
            }
            if (ret = Operator::TryRead(s, (Operator**)out)){
                return ret;
            }
        }else{
            if (ret = Operator::TryRead(s, (Operator**)out)){
                return ret;
            }
            if (ret = Number::TryRead(s, (Number**)out)){
                return ret;
            }
            if (ret = Bracket::TryRead(s, (Bracket**)out)){
                return ret;
            }
        }
        return 0;
    }
    bool MathT::TryGetNumber(double* out){
        if (IsNumber()){
            *out = ((Number*)(this))->value;
            return true;
        }
        return false;
    }
    bool MathT::IsNumber(){
        return false;
    }
    bool MathT::IsOperator(){
        return false;
    }
    bool MathT::IsExpression(){
        return false;
    }
    bool MathT::IsBracket(){
        return false;
    }
    bool MathT::IsMultiplyOrDivide(){
        return false;
    }
    ostream& operator<<(MathT& mt, ostream& o){
        return mt.Print(o);
    }
    ostream& operator<<(ostream& o, MathT& mt){
        return mt.Print(o);
    }
    //Number
    Number::Number(int val){
        value = (double)val;
    }
    Number::Number(float val){
        value = (double)val;
    }
    Number::Number(double val){
        value = val;
    }
    bool Number::IsNumber(){
        return true;
    }
    double Number::GetValue(){
        return value;
    }
    int Number::TryRead(char* s, Number** out){
        if (IsNullOrEmpty(s)){
            return 0;
        }
        int ps = PreceedingSpaces(s);
        if (ps > 0)
            s = &(s[ps]);
        if (!Calculator::HasNumber(s)){
            return 0;
        }
        double lf;
        int n;
        if (sscanf(s, "%lf%n", &lf, &n) != 1){
            return 0;
        }
        *out = new Number(lf);
        return n + ps;
    }
    ostream& Number::Print(ostream& o){
        return o << value;
    }
    //Operator
    bool Operator::IsOperator(){
        return true;
    }
    int Operator::TryRead(char* s, Operator** out){
        if (IsNullOrEmpty(s))
            return 0;

        int ps = PreceedingSpaces(s);
        if (ps > 0)
            s = &(s[ps]);
        switch(s[0]){
            case '+':{
                *out = (Operator*)(new Add());
                return ps + 1;
            }
            case '-':{
                *out = (Operator*)(new Subtract());
                return ps + 1;
            }
            case '*':{
                *out = (Operator*)(new Multiply());
                return ps + 1;
            }
            case '/':{
                *out = (Operator*)(new Divide());
                return ps + 1;
            }
        }
        return 0;
    }
    Number* Operator::Operate (Number* lhs, Number* rhs){
        return new Number(Operate(lhs->value, rhs->value));
    }
    //Add
    double Add::Operate(double lhs, double rhs){
        return lhs + rhs;
    }
    ostream& Add::Print(ostream& o){
        return o << '+';
    }
    //Subtract
    double Subtract::Operate(double lhs, double rhs){
        return lhs-rhs;
    }
    ostream& Subtract::Print(ostream& o){
        return o << '-';
    }
    //Multiply
    bool Multiply::IsMultiplyOrDivide(){
        return true;
    }
    double Multiply::Operate(double lhs, double rhs){
        return lhs*rhs;
    }
    ostream& Multiply::Print(ostream& o){
        return o << '*';
    }
    //Divide
    bool Divide::IsMultiplyOrDivide(){
        return true;
    }
    double Divide::Operate(double lhs, double rhs){
        return lhs/rhs;
    }
    ostream& Divide::Print(ostream& o){
        return o << '/';
    }
    //Expression
    bool Expression::IsExpression(){
        return true;
    }
    bool Expression::IsNumber(){
        return false;
    }
    Expression::~Expression(){
        for (std::list<MathT*>::iterator iterator = members.begin(), end = members.end(); iterator != end; ++iterator) {
            delete &(*iterator);
        }
    }

    int Expression::TryRead(char* s){

    if (IsNullOrEmpty(s)){
        return 0;
    }
    s = &(s[PreceedingSpaces(s)]);
    if (!Calculator::IsNumber(s[0]) && s[0] != '('){
        return 0;
    }
    int n;
    int nt = 0;
    bool wasOperator = true;
    int d;
    MathT* t;
    char c;
    int asd  = 0;
    while(s[0] != '\0'){
        ++asd;
        if (n = MathT::TryRead(s, &t, wasOperator)){
            if (wasOperator){
                if (t->IsOperator()){
                    return 0;
                }
                wasOperator = false;
            }else{
                if (t->IsOperator()){
                    wasOperator=true;
                }else{
                    if (t->IsNumber() && ((Number*)t)->value < 0){
                        members.push_back(new Subtract());
                        ((Number*)t)->value *= -1;
                    }else
                        members.push_back(new Multiply());
                    wasOperator = false;
                }
            }
        }else{
            return 0;
        }
        members.push_back(t);
        s=&(s[n]);
        nt += n;
    }
    if (wasOperator){
        return 0;
    }
    int size = members.size();
    if (size == 1)
        return nt;
    if ((size-1)%2!=0){
        return 0;
    }
    std::list<MathT*>::iterator iterator = members.begin();
    std::list<MathT*>::iterator lhsi = iterator;
    MathT* lhs = *iterator;
    ++iterator;
    int i = 1;
        for (std::list<MathT*>::iterator end = members.end(); iterator != end;) {
            MathT* op = *iterator;
            ++iterator;
            ++i;
            if (iterator == end){
                throw(runtime_error("Bad expression 0"));
            }
            if (op->IsMultiplyOrDivide()){
                MathT* rhs = *iterator;
                Expression* x2 = new Expression();
                x2->members.push_back(lhs);
                x2->members.push_back(op);
                x2->members.push_back(rhs);
                std::list<MathT*>::iterator rhsi = iterator;
                int e = 2;
                while(iterator != end){
                    ++iterator;
                    if (iterator == end)
                        break;
                    op = *iterator;
                    if (!op->IsMultiplyOrDivide())
                        break;
                    ++iterator;
                    if (iterator == end)
                        throw (runtime_error("Bad expression 3"));
                    x2->members.push_back(op);
                    rhsi = iterator;
                    x2->members.push_back(*iterator);
                    e+=2;
                }

                iterator = members.erase(lhsi, ++rhsi);
                iterator = members.insert(iterator, static_cast<MathT*>(x2));
                lhs = (MathT*)x2;
                std::list<MathT*>::iterator itercpy = iterator;
                lhsi = --iterator;
                i-=e;
            }else{
                lhsi = iterator;
                lhs = *iterator;
                ++iterator;
                ++i;
            }
            if (iterator == end)
                break;
        }
        return nt;
    }
    int Expression::TryRead(char* s, Expression** out){
        if (IsNullOrEmpty(s)){
            return 0;
        }
        int ps = PreceedingSpaces(s);
        if (ps > 0)
            s = &(s[ps]);
        if (!HasNumber(s) && s[0] != '('){
            return 0;
        }
        Expression* x = new Expression();
        int n;
        if (!(n = x->TryRead(s))){
            delete x;
            return 0;
        }
        *out = x;
        return n+ps;
    }

    double Expression::SolveDouble(){
        int count = members.size();
        if (count < 3){
            MathT* f = members.front();
            if (f->IsNumber() || f->IsExpression())
                return (static_cast<Number*>(f))->GetValue();
            throw(runtime_error("Bad Expression 1"));
        }
        std::list<MathT*>::iterator iterator = members.begin();
        double lhs = (static_cast<Number*>(*iterator))->value;
        ++iterator;
        for (std::list<MathT*>::iterator end = members.end(); iterator != end;) {
            Operator* op = (static_cast<Operator*>(*iterator));
            ++iterator;
            if (iterator == end)
                throw(runtime_error("Bad expression 2"));
            double rhs = (static_cast<Number*>(*iterator))->GetValue();
            ++iterator;
            lhs = op->Operate(lhs, rhs);
        }
        return lhs;
    }
    double Expression::GetValue(){
        return SolveDouble();
    }


    ostream& Expression::Print(ostream& o){
        for (std::list<MathT*>::iterator iterator = members.begin(), end = members.end(); iterator != end; ++iterator) {
            o << *((MathT*)(*iterator));
        }
        return o;
    }
    ostream& operator<<(ostream& o, Expression& x){
        return x.Print(o);
    }
    ostream& operator<<(Expression& x,ostream& o){
        return x.Print(o);
    }



    //Bracket
    bool Bracket::IsBracket(){
        return true;
    }
    int Bracket::TryRead(char* s, Bracket** out){
        if (IsNullOrEmpty(s))
            return 0;
        int ps = PreceedingSpaces(s);
        if (ps > 0)
            s = &(s[ps]);
        if (s[0] != '(')
            return 0;
        char* s2 = &(s[1]);
        s = s2;
        if (!Calculator::IsNumber(s[PreceedingSpaces(s)]))
            return 0;
        int n = 0;
        int b = 1;
        while(s[0] != '\0'){
            if (s[0] == '(')
                ++b;
            if (s[0] == ')')
                --b;
            if (b == 0)
                break;
            ++n;
            s = &(s[1]);
        }
        if (b != 0)
            return false;
        char s3[n+1];
        strncpy(s3, s2, n);
        s3[n] = '\0';
        Bracket *br = new Bracket();
        if (((Expression*)br)->TryRead(&(s3[0])) == 0){
            delete br;
            return false;
        }
        *out = br;

        return ps + n + 2;
    }
    ostream& Bracket::Print(ostream& o){
        o << '(';
        Expression::Print(o);
        return o << ')';
    }
    //Misc
    int PreceedingSpaces(char* s){
        int i = 0;
        while(s[0] == ' ')
            s = &s[1];
        return i;
    }
    bool IsNumber(char c){
        return (c >= '0' && c <= '9');
    }
    bool HasNumber(char* s){
        return (IsNumber(s[0]) || (s[0] == '-' && IsNumber(s[1])));
    }
    bool IsNullOrEmpty(char* s){
        return (s==null || s[0] == '\0' || s[PreceedingSpaces(s)] == '\0');
    }
}
