#include "CompilerParser.h"

#include <iostream>


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    Node* temp;

    root = new Node();
    currentNode = root;
    for(auto& i : tokens){
        temp = new Node();
        temp->data = i;
        currentNode->link = temp;
        currentNode = temp;
    }
    temp->link =nullptr;
    currentNode = root->link;
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    ParseTree* result = new ParseTree("class","");

    result->addChild(mustBe("keyword", "class"));
    if(currentNode->data->getType()=="identifier"){
        result->addChild(mustBe("identifier", currentNode->data->getValue()));
    }
    result->addChild(mustBe("symbol", "{"));
    result->addChild(mustBe("symbol", "}"));

    return result;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* result = new ParseTree("class","");

    result->addChild(mustBe("keyword", "class"));
    if(currentNode->data->getType()=="identifier"){
        result->addChild(mustBe("identifier", currentNode->data->getValue()));
    }
    result->addChild(mustBe("symbol", "{"));

    while(currentNode!=nullptr){
        if(have("symbol","}")){
            break;
        }

        if(have("keyword","static")||have("keyword", "field")){
            result->addChild(compileClassVarDec());
        }else if(have("keyword","constructor")||have("keyword", "method")||have("keyword", "function")){
            result->addChild(compileSubroutine());            
        }
    }

    result->addChild(mustBe("symbol", "}"));

    return result;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* result = new ParseTree("classVarDec","");

    if(have("keyword", "static")){
        result->addChild(mustBe("keyword", "static"));
    }else if(have("keyword", "field")){
        result->addChild(mustBe("keyword", "field"));
    }else{
        throw ParseException();
    }

    if(have("keyword", "int")){
        result->addChild(mustBe("keyword", "int"));
    }else if(have("keyword", "char")){
        result->addChild(mustBe("keyword", "char"));
    }else if(have("keyword", "boolean")){
        result->addChild(mustBe("keyword", "boolean"));
    }else{
        throw ParseException();
    }

    while(!have("symbol", ";")){
        if(currentNode->data->getType()=="identifier"){
            result->addChild(mustBe("identifier", currentNode->data->getValue()));

        }else{
            throw ParseException();        
        }
        if(have("symbol", ",")){
            result->addChild(mustBe("symbol", ","));
        }
    }

    result->addChild(mustBe("symbol", ";"));

    return result;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* result = new ParseTree("subroutine","");

    printTokens();

    if(have("keyword", "function")){
        result->addChild(mustBe("keyword", "function"));
    }else if(have("keyword", "constructor")){
        result->addChild(mustBe("keyword", "constructor"));
    }else if(have("keyword", "method")){
        result->addChild(mustBe("keyword", "method"));
    }else{
        throw ParseException();        
    }

    if(have("keyword", "void")){
        result->addChild(mustBe("keyword", "void"));
    }else if(have("keyword", "int")){
        result->addChild(mustBe("keyword", "int"));
    }else if(have("keyword", "char")){
        result->addChild(mustBe("keyword", "char"));
    }else if(have("keyword", "boolean")){
        result->addChild(mustBe("keyword", "boolean"));
    }else if(have("identifier", "Test")){
        result->addChild(mustBe("identifier", "Test"));
    }else{
        throw ParseException();        
    }    

    if(currentNode->data->getType()=="identifier"){
        result->addChild(mustBe("identifier", currentNode->data->getValue()));
    }else{
        throw ParseException();        
    }

    result->addChild(mustBe("symbol", "("));
    result->addChild(compileParameterList());
    result->addChild(mustBe("symbol", ")"));
    result->addChild(compileSubroutineBody());

    return result;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    ParseTree* result = new ParseTree("parameterList","");

    if(currentNode==nullptr || have("symbol",")")){
        return result;
    }

    if(have("keyword", "int")){
        result->addChild(mustBe("keyword", "int"));
    }else if(have("keyword", "char")){
        result->addChild(mustBe("keyword", "char"));
    }else if(have("keyword", "boolean")){
        result->addChild(mustBe("keyword", "boolean"));
    }else if(have("identifier", "Test")){
        result->addChild(mustBe("identifier", "Test"));
    }else{
        throw ParseException();        
    }

    if(currentNode->data->getType()=="identifier"){
        result->addChild(mustBe("identifier", currentNode->data->getValue()));
    }else{
        throw ParseException();        
    }

    while(currentNode!=nullptr){
        std::cout<<"while"<<std::endl;

        if(have("symbol", ",")){
            result->addChild(mustBe("symbol", ","));
        }else if(have("symbol", ")")){
            break;
        }

        if(have("keyword", "int")){
            result->addChild(mustBe("keyword", "int"));
        }else if(have("keyword", "char")){
            result->addChild(mustBe("keyword", "char"));
        }else if(have("keyword", "boolean")){
            result->addChild(mustBe("keyword", "boolean"));
        }else if(have("identifier", "Test")){
            result->addChild(mustBe("identifier", "Test"));
        }else{
            throw ParseException();
        }

        if(currentNode->data->getType()=="identifier"){
            result->addChild(mustBe("identifier", currentNode->data->getValue()));
        }else{
            throw ParseException();        
        }        
    }

    return result;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* result = new ParseTree("subroutineBody","");

    result->addChild(mustBe("symbol", "{"));

    while(!have("symbol", "}")){
        if(have("keyword", "var")){
            result->addChild(compileVarDec());
        }else{
            break;
        } 
    }

    result->addChild(compileStatements());
    result->addChild(mustBe("symbol", "}"));
    
    return result;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    ParseTree* result = new ParseTree("varDec","");

    // printTokens();

    if(currentNode==nullptr||!have("keyword", "var")){
        return result;
    }

    result->addChild(mustBe("keyword", "var"));

    if(have("keyword", "int")){
        result->addChild(mustBe("keyword", "int"));
    }else if(have("keyword", "char")){
        result->addChild(mustBe("keyword", "char"));
    }else if(have("keyword", "boolean")){
        result->addChild(mustBe("keyword", "boolean"));
    }else if(have("identifier", "Test")){
        result->addChild(mustBe("identifier", "Test"));        
    }else{
        throw ParseException();
    }

    if(currentNode->data->getType()=="identifier"){
        result->addChild(mustBe("identifier", currentNode->data->getValue()));
    }else{
        throw ParseException();        
    }    

    while(currentNode!=nullptr){
        if(have("symbol", ",")){
            result->addChild(mustBe("symbol", ","));
        }else{
            break;
        }

        if(currentNode->data->getType()=="identifier"){
            result->addChild(mustBe("identifier", currentNode->data->getValue()));

        }else{
            throw ParseException();        
        }
    }

    result->addChild(mustBe("symbol", ";"));

    return result;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    ParseTree* result = new ParseTree("statements","");

    while(currentNode!=nullptr){
        if(have("keyword", "let")){
            result->addChild(compileLet());
        }else if(have("keyword", "do")){
            result->addChild(compileDo());
        }else if(have("keyword", "return")){
            result->addChild(compileReturn());
        }else if(have("keyword", "while")){
            result->addChild(compileWhile());
        }else if(have("keyword", "if")){
            result->addChild(compileIf());
        }else{
            break;
        }
    }
    
    return result;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    ParseTree* result = new ParseTree("letStatement","");

    result->addChild(mustBe("keyword", "let"));
    result->addChild(mustBe("identifier", currentNode->data->getValue()));
    result->addChild(compileExpressionSkip());
    result->addChild(mustBe("symbol", "="));
    result->addChild(compileExpressionSkip());
    result->addChild(mustBe("symbol", ";"));
    
    return result;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    ParseTree* result = new ParseTree("ifStatement","");

    result->addChild(mustBe("keyword", "if"));
    result->addChild(mustBe("symbol", "("));
    result->addChild(compileExpressionSkip());
    result->addChild(mustBe("symbol", ")"));
    result->addChild(mustBe("symbol", "{"));
    result->addChild(compileStatements());
    result->addChild(mustBe("symbol", "}"));

    if(currentNode!=nullptr){
        if(have("keyword", "else")){
            result->addChild(mustBe("keyword", "else"));
            result->addChild(mustBe("symbol", "{"));
            result->addChild(compileStatements());
            result->addChild(mustBe("symbol", "}"));  
        }      
    }

    return result;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    ParseTree* result = new ParseTree("whileStatement","");

    result->addChild(mustBe("keyword", "while"));
    result->addChild(mustBe("symbol", "("));
    result->addChild(compileExpressionSkip());
    result->addChild(mustBe("symbol", ")"));
    result->addChild(mustBe("symbol", "{"));
    result->addChild(compileStatements());
    result->addChild(mustBe("symbol", "}"));

    return result;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    ParseTree* result = new ParseTree("doStatement","");

    result->addChild(mustBe("keyword", "do"));
    result->addChild(compileExpressionSkip());
    result->addChild(mustBe("symbol", ";"));
    
    return result;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    ParseTree* result = new ParseTree("returnStatement","");

    result->addChild(mustBe("keyword", "return"));
    if(!have("symbol", ";")){
        result->addChild(compileExpressionSkip());
    }
    result->addChild(mustBe("symbol", ";"));
    
    return result;
}

ParseTree* CompilerParser::compileExpressionSkip() {
    ParseTree* result = new ParseTree("expression","");
    if(!(have("symbol", ";")||have("symbol", "=")||have("symbol", ")"))){
        result->addChild(new ParseTree("keyword","skip"));
    }

    while(currentNode!=nullptr&!(have("symbol", ";")||have("symbol", "=")||have("symbol", ")"))){
        next();
    }

    return result;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    ParseTree* result = new ParseTree("expression","");
    /*
    + - * / & | > < = ~
    */

    return result;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}

/**
 * Advance to the next token
 */
void CompilerParser::next(){
    currentNode = currentNode->link;
    return;
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    return currentNode->data;
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    Token* temp = currentNode->data;
    if(temp->getType()==expectedType & temp->getValue()==expectedValue){
        return true;
    }
    return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){
    Token* result;
    if(have(expectedType,expectedValue)){
        result = currentNode->data;
        next();
        return result;
    }else{
        throw ParseException();
        return NULL;
    }
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}




void CompilerParser::printTokens(){
    Node* temp = currentNode;

    while(temp!=nullptr){
        std::cout<<temp->data->getType()<<" "<<temp->data->getValue()<<std::endl;
        temp=temp->link;
    }
}