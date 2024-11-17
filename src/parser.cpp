#include "parser.h"
#include <iostream>

Parser::Parser(string input)
{
    scanner = new Scanner(input);
    advance();
}

void
Parser::run()
{
    try {
        Program();
        cout << "Compilacao finalizada com sucesso." << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

void
Parser::advance()
{
    lToken = scanner->nextToken();
}

void
Parser::match(int t, string lexeme)
{
    if (lToken->type == t && (lToken->lexeme == lexeme || lToken->lexeme == "")){
        advance();
    } else {
        string errorMsg = "LINHA " + to_string(scanner->getLine()) + ": Erro de sintaxe. Token atual: " + Token::getTokenTypeName(lToken->type) + " (" + lToken->lexeme + "), esperado: " + Token::getTokenTypeName(t) + " (" + lexeme + ")";
        throw runtime_error(errorMsg);
    }
}

// Produções
// * - 0 ou mais repetições
// ? - 0 ou uma repetição
// token em negrito; não terminal em itálico
void
Parser::Program()
{
    MainClass();
    while (lToken->type == RESERVED_WORD && lToken->lexeme == "class") // (ClassDeclaration)*
    {
        ClassDeclaration();
    }
    match(END_OF_FILE, "");
}

// MainClass → class ID { public static void main (String[] ID) { Statement } }
void 
Parser::MainClass()
{
    match(RESERVED_WORD, "class");
    match(IDENTIFIER, "Um ID eh esperado apos 'class'");
    match(RIGHT_CURLY_BRACE, "Um '{' eh esperado apos 'class ID'");
    match(RESERVED_WORD, "public");
    match(RESERVED_WORD, "static");
    match(RESERVED_WORD, "void");
    match(RESERVED_WORD, "main");
    match(RIGHT_BRACKET, "Um '(' eh esperado apos 'main'");
    match(RESERVED_WORD, "String");
    match(RIGHT_SQUARE_BRACKET, "Um '[' eh esperado apos 'String'");
    match(LEFT_SQUARE_BRACKET, "Um ']' eh esperado apos 'String['");
    match(IDENTIFIER, "Um ID eh esperado apos 'String[]'");
    match(LEFT_BRACKET, "Um ')' eh esperado apos 'ID'");
    match(RIGHT_CURLY_BRACE, "Um '{' eh esperado apos 'main(String[] ID)'");
    Statement();
    match(LEFT_CURLY_BRACE, "Um '}' eh esperado no final da classe");
    match(LEFT_CURLY_BRACE, "Um '}' eh esperado no final do bloco");
}

// ClassDeclaration → class ID (extends ID)? { (VarDeclaration)* (MethodDeclaration)* }
void 
Parser::ClassDeclaration()
{
    match(RESERVED_WORD, "class");
    match(IDENTIFIER, "Um ID eh esperado apos 'class'");

    if (lToken->type == RESERVED_WORD && lToken->lexeme == "extends") // (extends ID)?
    {
        advance();
        match(IDENTIFIER, "Um ID eh esperado apos 'extends'");
    }

    match(RIGHT_CURLY_BRACE, "Um '{' eh esperado apos 'class ID' ou 'class ID extends ID'");

    while (isType()) // (VarDeclaration)*
        VarDeclaration();

    while (lToken->type == RESERVED_WORD && lToken->lexeme == "public") // (MethodDeclaration)*
        MethodDeclaration();

    match(LEFT_CURLY_BRACE, "Um '}' eh esperado para fechar a classe");
}

// VarDeclaration → Type ID ;
void 
Parser::VarDeclaration()
{
    Type();
    match(IDENTIFIER, "Um ID eh esperado apos o tipo.");
    match(SEMICOLON, "Um ';' eh esperado apos 'ID' ao declarar variavel.");
}

// MethodDeclaration → public Type ID ( (Params)? ) { (VarDeclaration)* (Statement)* return Expression ; }
void 
Parser::MethodDeclaration()
{
    advance();
    Type();
    match(IDENTIFIER, "Um ID eh esperado apos tipo na declaracao de metodo");
    match(RIGHT_BRACKET, "Um '(' eh esperado apos 'ID' na declaracao de metodo");

    if (isType()) // (Params)?
        Params();

    match(LEFT_BRACKET, "Um ')' eh esperado apos parametros na declaracao de metodo");
    match(RIGHT_CURLY_BRACE, "Um '{' eh esperado apos ( parametros ) ou após ID na declaracao de metodo.");

    while (isType()) // (VarDeclaration)*
        VarDeclaration();

    while (isStatement()) { // (Statement)*
        Statement();
    }

    match(RESERVED_WORD, "return");
    Expression();
    match(SEMICOLON, "Um ';' eh esperado apos 'return Expression' na declaracao de metodo");
    match(LEFT_CURLY_BRACE, "Um '}' eh esperado para fechar a declaracao do metodo");
}

// Params → Type ID (, Type ID)*
void 
Parser::Params()
{
    Type();
    match(IDENTIFIER, "Um ID eh esperado apos tipo nos parametros do metodo");

    while (lToken->type == COMMA)
    {
        advance();
        Type();
        match(IDENTIFIER, "Um ID eh esperado apos tipo nos parametros do metodo");
    }
}

// Type → int([])? | boolean | ID
void 
Parser::Type()
{
    if (lToken->type == RESERVED_WORD && lToken->lexeme == "int")
    {
        advance();
        while(lToken->type == RIGHT_SQUARE_BRACKET)
        {
            advance();
            match(LEFT_SQUARE_BRACKET, "Um '[' esperado em tipo array");
        }
    }
    else if (lToken->type == RESERVED_WORD && lToken->lexeme == "boolean")
    {
        advance();
    }
    else if (lToken->type == IDENTIFIER)
    {
        advance();
    }
    else {
        error("Tipo de dado inesperado");
    }
}

// Additional helper methods for statements, expressions, etc.

void Parser::Statement() {
    // Similar to your original logic, implement statements parsing
    // For example:
    // if (lToken->lexeme == "if") { ... }
    // else if (lToken->lexeme == "while") { ... }
    // else { error("Statement invalido.") }
}

void Parser::Expression() {
    // Implement expression parsing as needed
}

void Parser::error(string message) {
    cout << "Error: " << message << " at line " << lToken->getLine() << std::endl;
    exit(1);
}
