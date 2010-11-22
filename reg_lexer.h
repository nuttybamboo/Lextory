#include <map>
#include <set>
#include <iostream>
#include <fstream>

using namespace std;

static const int TOKEN_SET    = 0X100;
static const int TOKEN_STRING     = 0X101;

static const int TOKEN_DIFI= 0X102;
static const int TOKEN_CHAR= 0X103;

static const char FINISH_CHAR = '$';
static const int FINISH_SYMBOL_INDEX = 0X80;

 class Buffer{
    private :
    string buf;
    unsigned int offset;

    public :
    Buffer(){
        buf = "";
    }
    void read(string value){
        offset = 0;
        this -> buf = value;
    }
    char nextChar(){
        return buf[offset++];
    }
};

class Token{
    public :
    int tag;
    Token(){
        tag = -1;
    }

    Token(int tag){
        this->tag = tag;
    }
};
class Symbol : public Token{
    public:
    char value;
    Symbol(char value):Token(TOKEN_CHAR){
        this->value = value;
    }
};
class SymbolSet : public Token{
    public :
    set<char> elem;
    SymbolSet(set<char> el):Token(TOKEN_SET){
        elem = el;
    }
};
class String : public Token{
    public:
    string value;
    String(string val):Token(TOKEN_STRING){
        value = val;
    }
};
class Difination : public Token{
    public :
    string name;
    Difination(string name):Token(TOKEN_DIFI){
        this->name = name;
    }
};
class Lexer{
        private :
        char peek;
        //map<string, Word> words;
        Buffer inbuf;


        Token* dealChar();
        Token* dealSet();
        Token* dealDif();
        Token* dealString();
        Token* dealOperator();

        public :
        string getDifiName();
        string getAction();
        bool skipSpace();
        int line;
        Token* next();
        Lexer(){}
        void init(string value){
            line = 1;
            inbuf.read(value);
            peek = inbuf.nextChar();
        }

    };
