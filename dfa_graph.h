#include "reg_lexer.h"
#include <vector>

#define MAX 500
#define MAX_SYMBOL_NUMBER 0x81
#define MAX_DSTATE_NUMBER 1000
#define MAX_REGSTRING_LENGTH  500

class DState{
    //static DState * hash_table[5][8];
    static int DTransTable[MAX_DSTATE_NUMBER][MAX_SYMBOL_NUMBER];
    static int DState_counter;

    public:
    int dStateID;
    set<int> positions;
    DState * next;

    DState(set<int> pos);

    int Move(int symbol){
        return DTransTable[dStateID][(int)symbol];
    }
    void SetTrans(int symbol, int tarDStateID){
        DTransTable[dStateID][(int)symbol] = tarDStateID;
    }
};

class RegNode{
    public :
    vector<int> first;
    vector<int> last;
    bool nullable;

    RegNode(vector<int> first, vector<int> last, bool ifnull){
        this->first = first;
        this->last = last;
        this->nullable = ifnull;
    }
};

class DirectTranser{
    static set<int> symble_aso_table[MAX_SYMBOL_NUMBER];
    static int current_node;
    static map<string, RegNode> difis;

public:
    static vector<DState> DFAGraph;
    static map<int, string> actions;
    static bool finish_state_table[MAX_DSTATE_NUMBER];

private :
    Lexer lex;
    Token * peek;
    vector<int> follows[MAX_DSTATE_NUMBER];
    RegNode E();
    RegNode C();
    RegNode S();
    RegNode F();
    RegNode closure(RegNode sub);
    RegNode concatenation(RegNode left, RegNode right);
    RegNode or_union( RegNode left, RegNode right);

    DState * CheckState(set<int>);
public:
    void comput_finish();
    void difination(string difiSring);
    int converse(string regString);
    DirectTranser(){
         cout << "during constructing dt" << endl;
    }
};
