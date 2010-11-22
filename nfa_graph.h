#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>

using namespace std;

#define MAX 500
#define MAX_SYMBOL_NUMBER 0x100
#define MAX_DSTATE_NUMBER 1000
#define MAX_REGSTRING_LENGTH  500

class NState{
    private:
    static int NState_counter;
    public:
    int number;
    vector<int> trans_table[0X100];

    NState();
    vector<int> Move(char symbol);
    void Set(char symbol, int NState){
        trans_table[symbol].push_back(NState);
    }
};

vector<NState> NFAGraph;

int NState::NState_counter = 0;
NState::NState(){
        number = NState_counter++;
        NFAGraph.push_back(*this);
        for(int i = 0; i < 0X100; ++i){
            trans_table[i] = vector<int>();
        }
}

vector<int> NState::Move(char symbol){
    return trans_table[symbol];
}

class Nfa{
    public:
    int start;
    int finish;
    Nfa(int s, int f){
        start = s;
        finish = f;
    }
};

class Lextory{
    private:
    ofstream f_out;
    ifstream f_in;
    map<string, Nfa> exps;

    public:
    Lextory(string filePath){
        f_in.open(filePath.c_str());
        f_out.open("nutty_lexer.c");
    }

    private:
    void filePretreatment();

    void difination(string regularExp){
        int i = 0;

        string name = GetString(regularExp, i);

        char peek;

        for(peek = regularExp[i];peek != '\0' && ( peek == ' ' || peek == '\t' );peek = regularExp[++i]){
            ;
        }
        if(peek == '\0'){
            cout << "error!!!" << endl;
        }

        Nfa reg_exp = E(regularExp, i);

        exps.insert(map<string, Nfa>::value_type(name, reg_exp));
    }

    string GetString(string regularExp, int& i){
        string value;
        char peek;
        for(peek = regularExp[i];peek != '\0';peek = regularExp[++i]){
            if((peek <= 'Z' && peek >= 'A') || (peek <= 'z' && peek >= 'a') || (peek <= '9' && peek >= '0') || peek == '$' || peek == '_'){
                value += peek;
            }
            else{
                break;
            }
        }
        if(peek == '\0'){
            cout << "error!!!" << endl;
        }
        return value;
    }
    Nfa E(string regularExp, int& i){
        char peek = regularExp[i];
        Nfa sub_reg1 = C(regularExp, i);

        if(peek == '\0'){
            return sub_reg1;
        }
        if(sub_reg1.start < 0 || peek != '|'){
            cout << "error" <<endl;
            return Nfa(-1, -1);
        }
        else{
            i++;
        }
        Nfa sub_reg2 = E(regularExp, i);
        if(sub_reg2.start < 0 ){
            cout << "error" <<endl;
            return Nfa(-1, -1);
        }

        return concatenation(sub_reg1, sub_reg2);
    }
    Nfa C(string regularExp, int& i){
        char peek = regularExp[i];
        Nfa sub_reg1 = S(regularExp, i);

        if(peek == '\0'){
            return sub_reg1;
        }
        if(sub_reg1.start < 0 ){
            cout << "error" <<endl;
            return Nfa(-1, -1);
        }
        Nfa sub_reg2 = C(regularExp, i);
        if(sub_reg2.start < 0 ){
            cout << "error" <<endl;
            return Nfa(-1, -1);
        }

        return concatenation(sub_reg1, sub_reg2);
    }
    Nfa S(string regularExp, int& i){
        char peek = regularExp[i];
        Nfa sub_reg = F(regularExp, i);
        if(peek == '*'){
            return closure(sub_reg);
        }
        else{
            return sub_reg;
        }
    }

    Nfa F(string regularExp, int& i){
        Nfa sub_reg(-1, -1);

        char peek = regularExp[i];
        string reg_name;

        switch(peek){
            case '{' :
                ++i;
                reg_name = GetString(regularExp, i);
                if(peek != '}'){
                    cout << "error" << endl;
                }
                else{
                    if(exps.find(reg_name) != exps.end()){
                        sub_reg = exps.find(reg_name) ->second;
                        ++i;
                    }
                    else{
                        cout << "error" << endl;
                    }
                }
                break;
            case '(' :
                ++i;
                sub_reg = E(regularExp, i);
                if(sub_reg.start < 0 || peek != ')'){
                    cout << "error" << endl;
                }
                else{
                    ++i;
                }
                break;
            default:
                break;
        }
        return sub_reg;
    }

    Nfa closure(Nfa reg){
        NState new_start, new_finish;
        new_start.Set('\0', reg.start);
        new_start.Set('\0', new_finish.number);
        NFAGraph[reg.finish].Set('\0', new_finish.number);
        NFAGraph[reg.finish].Set('\0', reg.start);
        return Nfa(new_start.number, new_finish.number);
    }
    Nfa concatenation(Nfa first, Nfa second){
        NFAGraph[first.finish].Set('\0', second.start);
        return Nfa(first.start, second.finish);
    }
    Nfa or_union( Nfa first, Nfa second){
        NState new_start, new_finish;
        new_start.Set('\0', first.start);
        new_start.Set('\0', second.start);
        NFAGraph[first.finish].Set('\0', new_finish.number);
        NFAGraph[second.finish].Set('\0', new_finish.number);
        return Nfa(new_start.number, new_finish.number);
    }
};





