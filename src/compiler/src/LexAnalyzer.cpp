#include "LexAnalyzer.h"

#include <iostream>

LexAnalyzer::LexAnalyzer(string filename)
    : file(filename, ios::binary)
{
    // Init static data if has not init.
    if(!LexAnalyzer::hasInit)
        LexAnalyzer::initStaticData();

    // Open source file
    if(!file.is_open() || !file){
        cerr << "File " << filename << " open failed." << endl;
        throw 2;
    }

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
    if(file.is_open()){
        file.close();
    }
}

bool LexAnalyzer::getChar(char &c){
    if(!file || file.eof())
        return false;
    file.get(c);
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
    // Branch
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
            // no value need to be assigned
            return true;
        }
        // Is identifier
        this->string_value = name;
        sy = sym::IDENTIFIER;
        return true;
    }

    return false;
}
