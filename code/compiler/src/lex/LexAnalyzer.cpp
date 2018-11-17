#include "LexAnalyzer.h"

#include <iostream>

LexAnalyzer::LexAnalyzer(ifstream &file)
    : file(file)
{
    // Init static data if has not init.
    if(!LexAnalyzer::hasInit)
        LexAnalyzer::initStaticData();

    // TODO check whether is a binary file

    // Location init
    // We start with `1` instead of `0`.
    line = 1;
    column = 0; // getChar will first increase `column`, so starts with `0`.

    // Char init
    // We start ch with ' ' to make sure we call `getChar` at the beginning.
    ch = ' ';
}

LexAnalyzer::~LexAnalyzer()
{
}

bool LexAnalyzer::getChar(char &c){
    if(!file || file.eof())
        return false;
    file.get(c);
    if(!file || file.eof())
        return false;
    column++;
    if(c == '\r')
        file.get(c);
    if(c == '\n'){
        line++;
        column = 0;
    }
    return true;
}

bool LexAnalyzer::isEOF(){
    return file.eof();
}

bool LexAnalyzer::goBack(){
    if(history.size() < 1)
        return false;
    State last_state = history.top();
    history.pop();
    this->loadState(last_state);
    return true;
}

LexAnalyzer::State LexAnalyzer::getCurrentState(){
    State res;
    res.int_value    = int_value;
    res.char_value   = char_value;
    res.string_value = string_value;
    res.symbol       = symbol;
    res.ch           = ch;
    res.line         = line;
    res.column       = column;
    res.pos          = file.tellg();
    return res;
}

void LexAnalyzer::loadState(const State & state){
    int_value    = state.int_value;
    char_value   = state.char_value;
    string_value = state.string_value;
    symbol       = state.symbol;
    ch           = state.ch;
    line         = state.line;
    column       = state.column;
    file.seekg(state.pos);
}

sym::SYMBOL LexAnalyzer::nextSymbol(){
    bool done;
    sym::SYMBOL res;

    do{
        // Skip Space
        while(ch == ' ' || ch == '\n' || ch == '\t'){
            if(!getChar(ch))
                break;
        }

        // Check EOF
        if(isEOF())
            return sym::ENDOFFILE;

        history.push(getCurrentState()); // Save history
        done = _nextSymbol(res); // Read symbol
        if(!done) // Failed, so pop history
            history.pop();
    }while(!done);

    symbol = res;
    return res;
}

bool LexAnalyzer::isAlpha(char c){
    if(c == '_')
        return true;
    if(c >= 'a' && c <= 'z')
        return true;
    if(c >= 'A' && c <= 'Z')
        return true;
    return false; // else
}

bool LexAnalyzer::isNotZeroDigit(char c){
    if(c >= '1' && c <= '9')
        return true;
    return false; // else
}

bool LexAnalyzer::isDigit(char c){
    if(c >= '0' && c <= '9')
        return true;
    return false; // else
}

bool LexAnalyzer::lookUpReserved(const string &name, sym::SYMBOL &sy){
    map<string, sym::SYMBOL>::iterator iter = LexAnalyzer::reservedTable.find(name);
    if(iter == LexAnalyzer::reservedTable.end())
        return false;
    sy = iter->second;
    return true;
}

bool LexAnalyzer::_nextSymbol(sym::SYMBOL &sy){
    // identifier  |  reserved
    if(isAlpha(ch)){
        string name = "";
        name.push_back(ch);
        while(getChar(ch)){
            if(!isAlpha(ch) && !isDigit(ch))
                break;
            name.push_back(ch);
        }
        if(lookUpReserved(name, sy)){
            // Is reserved
            // sy has been assigned in lookUpReserved
            this->string_value = name;
        }
        else{
            // Is identifier
            sy = sym::IDENTIFIER;
            this->string_value = name;
        }
        return true;
    }
    // unsigned_integer
    else if(isNotZeroDigit(ch) || ch == '0'){
        sy = sym::UNSIGNED_INTEGER;
        if(ch == '0'){
            // Just a zero
            this->int_value = 0;
            // Get next char for next call.
            getChar(ch);
            return true;
        }
        else{
            // Start with not_zero_digit
            string name = "";
            name.push_back(ch);
            while(getChar(ch)){
                if(!isDigit(ch))
                    break;
                name.push_back(ch);
            }
            this->int_value = stoi(name);
        }
        return true;
    }
    // character
    else if(ch == '\''){
        string error_head = "character: ";
        // Check internal
        if(!getChar(ch)){
            errorRepo(error_head + "get internal character failed.");
            return false;
        }
        if(!(
            isAlpha(ch)|| isDigit(ch)
            || ch == '+'
            || ch == '-'
            || ch == '*'
            || ch == '/'
            )){
            string tmp = "";
            tmp.push_back(ch);
            errorRepo(error_head + "internal character '" + tmp + "' is invalid.");
            return false;
        }
        char temp = ch; // Temporarily save
        // Check right
        if(!getChar(ch)){
            errorRepo(error_head + "get right ' failed.");
            return false;
        }
        if(ch != '\''){
            errorRepo(error_head + "lack ' at right.");
            return false;
        }
        // Get next char for next call.
        getChar(ch);
        // All right, save & return
        sy = sym::CHARACTER;
        this->char_value = temp;
        return true;
    }
    // string
    else if(ch == '\"'){
        string error_head = "string: ";
        string name = "";
        while(getChar(ch)){
            if(ch == '\"')
                break;
            if(
                ch != 32
                && ch != 33
                && !(ch >= 35 && ch <= 126)
            ){
                string tmp = "";
                tmp.push_back(ch);
                errorRepo(error_head + "'" + tmp + "' is invalid internal character.");
                return false;
            }
            name.push_back(ch);
        }
        // Get next char for next call.
        getChar(ch);
        // save & return
        sy = sym::STRING;
        this->string_value = name;
        return true;
    }

    // operator
    bool op_is = true;
    bool op_get_for_next = true;
    string op_error_head = "opeator: ";
    switch(ch){
        case '+': sy = sym::PLUS; break;
        case '-': sy = sym::MINUS;break;
        case '*': sy = sym::MULTI;break;
        case '/': sy = sym::DIV;  break;
        case '<':
            sy = sym::LESS;
            if(getChar(ch)){
                if(ch == '=')
                    sy = sym::LESSOREQUAL;
                else
                    op_get_for_next = false;
            }
            break;
        case '>':
            sy = sym::MORE;
            if(getChar(ch)){
                if(ch == '=')
                    sy = sym::MOREOREQUAL;
                else
                    op_get_for_next = false;
            }
            break;
        case '!':
            if(getChar(ch)){
                if(ch != '='){
                    string tmp;
                    tmp.push_back(ch);
                    errorRepo(op_error_head + "Should be !=, but we get !" + tmp);
                    return false;
                }
                sy = sym::NOTEQUAL;
            }
            else{
                errorRepo(op_error_head + "Should be !=, but we only get !");
                return false;
            }
            break;
        case '=':
            sy = sym::ASSIGN;
            if(getChar(ch)){
                if(ch == '=')
                    sy = sym::EQUAL;
                else
                    op_get_for_next = false;
            }
            break;
        default: op_is = false;// Not operator
    }
    if(op_is){// Is operator
        if(op_get_for_next)// Get next char for next call
            getChar(ch);
        // No value need to be assigned
        return true;
    }

    // separator
    bool sep_is = true;
    switch(ch){
        case '(': sy = sym::LEFT_ROUND;     break;
        case ')': sy = sym::RIGHT_ROUND;    break;
        case '[': sy = sym::LEFT_SQUARE;    break;
        case ']': sy = sym::RIGHT_SQUARE;   break;
        case '{': sy = sym::LEFT_BRACE;     break;
        case '}': sy = sym::RIGHT_BRACE;    break;
        case ';': sy = sym::SEMICOLON;      break;
        case ':': sy = sym::COLON;          break;
        case ',': sy = sym::COMMA;          break;
        default: sep_is = false;// Not separator
    }
    if(sep_is){// Is separator
        getChar(ch);// Get next char for next call
        // No value need to be assigned
        return true;
    }

    string tmp = "";
    tmp.push_back(ch);
    errorRepo("Invalid character '" + tmp + "'.");
    getChar(ch);
    return false;
}

void LexAnalyzer::errorRepo(string reason){
    cerr << "Lex Error at line[" << line << "] column[" << column << "]--" << endl;
    cerr << "\t" << reason << endl;
}