#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    ID,//1
    PLUS,//2
    MINUS,//3
    MULT,//4
    DIV,//5
    NUMBER,//6
    LPAREN,//7
    RPAREN,//8
    //Continuar
    //Nomes e atributos dos tokens da linguagem
    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};
