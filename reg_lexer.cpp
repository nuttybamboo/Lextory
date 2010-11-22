#include "reg_lexer.h"

string Lexer::getAction(){
    string value = "";
    for(;peek != '\0'; peek = inbuf.nextChar()){
        value += peek;
    }
    return value;
}
string Lexer::getDifiName(){
    string value = "";
    for(;peek != '\0';peek = inbuf.nextChar()){
        if((peek <= 'Z' && peek >= 'A') || (peek <= 'z' && peek >= 'a') || (peek <= '9' && peek >= '0') || peek == '_'){
            value += peek;
        }
        else{
            break;
        }
    }
    //cout << "after get name " << peek << endl;
    return value;
}
bool Lexer::skipSpace(){
    for( ; ; peek = inbuf.nextChar()){
        if(peek == ' ' || peek == '\t'){
            continue;
        }
        else if(peek == '/'){
            peek = inbuf.nextChar();
            if(peek == '*'){
                for(peek = ' ';;){
                    while(peek != '*' && inbuf.nextChar() != '*');
                    if((peek = inbuf.nextChar()) != '/'){
                        ;
                    }
                    else{
                        peek = inbuf.nextChar();
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        else{
            break;
        }
    }
    //cout << "after space " << peek << endl;
    if(peek == '\n' || peek == '\0'){
        return false;
    }
    else {
        return true;
    }
}
Token* Lexer::dealChar(){
    char value = 0;
    switch(peek){
    case '\\':
        peek = inbuf.nextChar();
        value = peek;
        peek = inbuf.nextChar();
        //cout << "peek = \\" << value << endl;
        return new Symbol(value);
    default :
        value = peek;
        peek = inbuf.nextChar();
        //cout << "peek = " << value << endl;
        return new Token(value);
    }

}
Token* Lexer::dealString(){
    string value = "";
    for(peek = inbuf.nextChar();peek != '\0';peek = inbuf.nextChar()){
        if(peek == '"'){
            peek = inbuf.nextChar();
            break;
        }

        if(peek == '\\'){
            peek = inbuf.nextChar();
            switch(peek){
                case 'n' :
                    value += '\n';
                    peek = inbuf.nextChar();
                    break;
                case 'r' :
                    value += '\r';
                    peek = inbuf.nextChar();
                    break;
                case '0' :
                    value += '\0';
                    peek = inbuf.nextChar();
                    break;
                case 't' :
                    value += '\t';
                    peek = inbuf.nextChar();
                    break;
                case 'v' :
                    value += '\v';
                    peek = inbuf.nextChar();
                    break;
                case 'b' :
                    value += '\b';
                    peek = inbuf.nextChar();
                    break;
                case 'f' :
                    value += '\f';
                    peek = inbuf.nextChar();
                    break;
                case 'a' :
                    value += '\a';
                    peek = inbuf.nextChar();
                    break;
                case '\"' :
                    value += '\"';
                    peek = inbuf.nextChar();
                    break;
                case '\'' :
                    value += '\'';
                    peek = inbuf.nextChar();
                    break;
                case '\\' :
                    value += '\\';
                    peek = inbuf.nextChar();
                    break;
                case '\?' :
                    value += '\?';
                    peek = inbuf.nextChar();
                    break;
                default :
                ;
            }
        }
        else{
            value += peek;
        }
    }

    return new String(value);
}

Token* Lexer::dealSet(){
    set<char> value;
    for(peek = inbuf.nextChar();peek != '\0';peek = inbuf.nextChar()){
        if(peek == ']'){
            peek = inbuf.nextChar();
            break;
        }
        if(peek == '-'){
            if(value.size() != 1){
                cout << "sytax error!" << endl;
            }
            else{
                peek = inbuf.nextChar();
                if(peek > *value.begin()){
                    for(char c = peek; c > *value.begin(); c--){
                        //cout << "insert " << c << endl;
                        value.insert(c);
                    }
                }
                else{
                    cout << "sytax error!" << endl;
                }
                peek = inbuf.nextChar();
                if(peek == ']'){
                    peek = inbuf.nextChar();
                    break;
                }
                else{
                    cout << "error !" << endl;
                    //break;
                }
            }
        }
        if(peek == '\\'){
            peek = inbuf.nextChar();
            switch(peek){
                case 'n' :
                    value.insert('\n');
                    peek = inbuf.nextChar();
                    break;
                case 'r' :
                    value.insert('\r');
                    peek = inbuf.nextChar();
                    break;
                case '0' :
                    value.insert('\0');
                    peek = inbuf.nextChar();
                    break;
                case 't' :
                    value.insert('\t');
                    peek = inbuf.nextChar();
                    break;
                case 'v' :
                    value.insert('\v');
                    peek = inbuf.nextChar();
                    break;
                case 'f' :
                    value.insert('\f');
                    peek = inbuf.nextChar();
                    break;
                default :
                    value.insert(peek);
                    peek = inbuf.nextChar();
                    break;
            }
        }
        else{
            value.insert(peek);
        }
    }
    //cout << "peek = set" << endl;
    return new SymbolSet(value);
}

Token* Lexer::dealDif(){
    string value = "";
    for(peek = inbuf.nextChar();peek != '\0' && peek != '$';peek = inbuf.nextChar()){
        if((peek <= 'Z' && peek >= 'A') || (peek <= 'z' && peek >= 'a') || (peek <= '9' && peek >= '0') || peek == '_'){
            value += peek;
        }
        else{
            break;
        }
    }
    if(peek != '}'){
        cout << "lexer get defination end peek not } : " << peek <<  "error!!!" << endl;
    }
    else{
        peek = inbuf.nextChar();
    }
    //cout << "peek = defi" << endl;
    return new Difination(value);
}

Token* Lexer::next(){
    //cout << "peek = " << peek << endl;
    if(peek == ' ' || peek == '\t' || peek == '\n' || peek == '\0'){
        //cout << "found ' ' " <<endl;
        return new Token('$');
    }
    //skipSpace();
    if(peek == '"'){
        return dealString();
    }
    if(peek == '['){
        return dealSet();
    }
    if(peek == '{'){
        return dealDif();
    }
    return dealChar();
}
