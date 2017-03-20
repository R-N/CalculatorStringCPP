#include <iostream>
#include <string>
#include "Calc2.hpp"
using namespace std;
int main(){
    int d = 2;
    float f = 5.2f;
    double lf = 3.5;
    bool b = true;
    while(true){
    printf("Input : ");
    char inputBuffer[256];
    char* input = gets(inputBuffer);
    Calculator::Expression* x;
    try{
        if (!Calculator::Expression::TryRead(input, &x)){
            cout << "Bad expression n" << endl;
            continue;
        }
    }catch(exception ex){
        cout << ex.what() << endl;
        continue;
    }
    cout <<"Input interpretation : " << *x << endl;
    cout <<"Result : " << x->SolveDouble() << endl;
    delete x;
    }
    return 0;
}
