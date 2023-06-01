#ifndef COMPILERPARSER_H
#define COMPILERPARSER_H

#include <list>
#include <exception>

#include "ParseTree.h"
#include "Token.h"

class CompilerParser {
    private:
        struct Node {
            Token* data;
            Node* link;
        };
        Node* root;
        Node* currentNode;

    public:
        CompilerParser(std::list<Token*> tokens);

        ParseTree* compileProgram();
        ParseTree* compileClass();
        ParseTree* compileClassVarDec();
        ParseTree* compileSubroutine();
        ParseTree* compileParameterList();
        ParseTree* compileSubroutineBody();
        ParseTree* compileVarDec();

        ParseTree* compileStatements();
        ParseTree* compileLet();
        ParseTree* compileIf();
        ParseTree* compileWhile();
        ParseTree* compileDo();
        ParseTree* compileReturn();

        ParseTree* compileExpressionSkip();
        ParseTree* compileExpression();
        ParseTree* compileTerm();
        ParseTree* compileExpressionList();
        
        void next();
        Token* current();
        bool have(std::string expectedType, std::string expectedValue);
        Token* mustBe(std::string expectedType, std::string expectedValue);

        //test
        void printTokens();
};

class ParseException : public std::exception {
    public:
        const char* what();
};

#endif /*COMPILERPARSER_H*/