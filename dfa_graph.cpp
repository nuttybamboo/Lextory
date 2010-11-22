#include <algorithm>
#include "dfa_graph.h"

int DState::DState_counter = 0;
int DState::DTransTable[MAX_DSTATE_NUMBER][MAX_SYMBOL_NUMBER] = {{0,},};
DState* hash_table[5][8] = {{NULL,},};

DState::DState(set<int> pos){
    /*
    DState * hasher = hash_table[pos.size()%5][(*pos.begin()) % 8];
    DState * previous = hasher;

   // cout << "hasher" << endl;

   cout << "new state :   " ;
    for(set<int>::iterator i = pos.begin(); i != pos.end(); i ++){
                  cout << *i << "  ";
    }
    cout << endl;

    cout << "hs : " << hasher << endl;


    for(;hasher != NULL;previous = hasher, hasher = hasher -> next){
        cout << "hasher not null" << endl;
        cout << "hs_s : "<< (hasher -> positions).size() << " pos s : " << pos.size() << " hs_b : "
          << *(hasher -> positions).begin() << " pos b :" << *pos.begin() << endl;
        if((hasher -> positions).size() == pos.size() &&
           *(hasher -> positions).begin() == *pos.begin()){
               bool flag = false;
               for(set<int>::iterator i = pos.begin(); i != pos.end(); i ++){
                   cout << " " << *i<<" ";
                   if((hasher -> positions).count(*i) <= 0){
                       flag = true;
                       break;
                   }
               }
               if(!flag){
                   this -> next = NULL;
                   this -> positions = hasher -> positions;
                   this -> dStateID = hasher -> dStateID;

                   return ;
               }
        }
    }
    //*/

    this -> positions = pos;
    this -> dStateID = DState_counter ++;;
    this -> next = NULL;
    /*
    //cout << this->next << "  " << NULL << endl;
    if(previous != NULL){
        //cout << "cofilition haseher " << endl;

        previous -> next = this;
        //cout << this->next << "  " << NULL << endl;
        this -> next = NULL;
  //cout << this->next << "  " << NULL << endl;
        //char ala;
        //cin >> ala;
    }
    else{
        hash_table[pos.size()%5][(*pos.begin()) % 8] = this;
    }

    cout << "new state : " << dStateID << "  " ;
    for(set<int>::iterator i = positions.begin(); i != positions.end(); i ++){
                  cout << *i << "  ";
    }
    cout << endl;
    //*/
}

int DirectTranser::current_node = 0;
map<string, RegNode> DirectTranser::difis;
set<int> DirectTranser::symble_aso_table[MAX_SYMBOL_NUMBER];
vector<DState> DirectTranser::DFAGraph;
bool DirectTranser::finish_state_table[MAX_DSTATE_NUMBER] = {false,};
map<int, string> DirectTranser::actions;

RegNode DirectTranser::E(){
//cout << "entering constructing E" << endl;
    RegNode sub_reg1 = C();
    if(peek == NULL){
        //cout << " peek null end constructing E" << endl;
        return sub_reg1;
    }
    if(peek->tag != '|'){
        //cout << "error" <<endl;
        return sub_reg1;
    }
    peek = lex.next();
    RegNode sub_reg2 = E();
//cout << "leving constructing E" << endl;
//cout << "reg1.first .size  : " << sub_reg1.first.size() << "reg2 size : "<< sub_reg2.first.size() << endl;
    return or_union(sub_reg1, sub_reg2);
}
RegNode DirectTranser::DirectTranser::C(){
//cout << "enter constructing C" << endl;
    RegNode sub_reg1 = S();
    if(peek == NULL || peek -> tag == '|' || peek -> tag == ')' || peek -> tag == '$'){
        //cout << "peek null end constructing C" << endl;
        return sub_reg1;
    }
    else{
    }
    RegNode sub_reg2 = C();

//cout << "leving constructing C" << endl;
    return concatenation(sub_reg1, sub_reg2);
}
RegNode DirectTranser::S(){
    //cout << "begin constructing S" << endl;
    RegNode sub_reg = F();

    if(peek != NULL && peek->tag == '*'){
        //cout << "Find *" << endl;

        peek = lex.next();
        //cout << "end constructing S current peek -> tag " << (peek -> tag)<< endl;
        return closure(sub_reg);
    }
    else{
        //if(peek == NULL)cout << "peek nulll leving S" << endl;
        //else cout << "end constructing S" << endl;
        return sub_reg;
    }
}

RegNode DirectTranser::F(){
    //cout << "begin constructing F" << endl;
    RegNode sub_reg(vector<int>(0), vector<int>(0), true);

   // Token * peek = lex.next();
    switch(peek -> tag){
        case TOKEN_DIFI:
            //cout << "difi" << endl;
            if(difis.find(((Difination *)peek) -> name) != difis.end()){
                //cout << "difination find"<<endl;
                sub_reg = difis.find(((Difination *)peek) -> name) ->second;
            }
            else{
                //cout << "no difination for" << ((Difination *)peek) -> name << endl;
            }
            break;
        case '(' :
            //cout << "Find ( " << endl;
            peek = lex.next();
            sub_reg = E();
            if( peek -> tag != ')'){
                cout << "error" << endl;
            }
            else{
                ;
            }
            break;
        default:
            sub_reg.first.push_back(current_node);
            sub_reg.nullable = false;
            sub_reg.last.push_back(current_node);
            if(peek -> tag == TOKEN_SET){
                for(set<char>::iterator i = ((SymbolSet *)peek)->elem.begin(); i != ((SymbolSet *)peek)->elem.end(); i ++)
                {
                    symble_aso_table[(int)*i].insert(current_node);
                }
            }
            else if(peek -> tag == TOKEN_CHAR){
                symble_aso_table[(int)((Symbol*)peek)->value].insert(current_node);
            }
            else if(peek -> tag == FINISH_CHAR){
                symble_aso_table[FINISH_SYMBOL_INDEX].insert(current_node);
            }
            else{
                symble_aso_table[peek -> tag].insert(current_node);
            }
            //cout << "Find " << (char)(peek -> tag) << endl;
            current_node++;
            break;
    }
    if(peek -> tag == '$'){
        //cout << "peek null" << endl;
        peek = NULL;
    }
    else{
        peek = lex.next();
    }

    //cout << "end constructing F" << endl;
    return sub_reg;
}

RegNode DirectTranser::closure(RegNode sub){
    //cout << "in closure" << endl;
    for(vector<int>::iterator i = sub.last.begin(); i != sub.last.end(); i++){
        vector<int> temp(follows[*i].size()+sub.first.size());
       // follows[*i] = set<int>((int)temp.size()+sub.first.size());
        set_union((follows[*i]).begin(), (follows[*i]).end(), sub.first.begin(), sub.first.end(),temp.begin());
        follows[*i] = temp;
    }
    //cout << "end closure" << endl;
    return RegNode(sub.first, sub.last, true);
}
RegNode DirectTranser::concatenation(RegNode left, RegNode right){
    //cout << "in concatenation " << endl;
    vector<int> new_first(left.first.size()+right.first.size()), new_last(left.last.size()+right.last.size());
    //cout << "in concatenation left" << endl;
    if(left.nullable){
        set_union(left.first.begin(), left.first.end(), right.first.begin(), right.first.end(), new_first.begin());
    }
    else{
        new_first = left.first;
    }
    //cout << "in concatenation right" << endl;
    if(right.nullable){
        set_union(left.last.begin(), left.last.end(), right.last.begin(), right.last.end(), new_last.begin());
    }
    else{
        new_last = right.last;
    }
    for(vector<int>::iterator i = left.last.begin(); i != left.last.end(); i++){
        vector<int> temp = follows[*i];
        follows[*i] = vector<int>(follows[*i].size()+right.first.size());
        //follows[*i] = set<int>(temp.size()+right.first.size());
        set_union(temp.begin(), temp.end(), right.first.begin(), right.first.end(), follows[*i].begin());
    }
    //cout << "end concatation"<< endl;
    return RegNode(new_first, new_last, left.nullable && right.nullable);
}
RegNode DirectTranser::DirectTranser::or_union( RegNode left, RegNode right){
    vector<int> new_first(left.first.size()+right.first.size()), new_last(left.last.size()+right.last.size());
    //set<int> new_first(left.first.size()+right.first.size()), new_last(left.last.size()+right.last.size());
    set_union(left.first.begin(), left.first.end(), right.first.begin(), right.first.end(), new_first.begin());
    set_union(left.last.begin(), left.last.end(), right.last.begin(), right.last.end(), new_last.begin());
    return RegNode(new_first, new_last, left.nullable || right.nullable);
}

void DirectTranser::difination(string difiString){
    lex.init(difiString);
    if(!lex.skipSpace()){
        return;
    }
    string name = lex.getDifiName();
    //cout << "difi name : " << name << endl;
    lex.skipSpace();
    peek = lex.next();
    //cout << "now peek = "<< (char)peek->tag << endl;

    RegNode reg_root = E();

    difis.insert(map<string, RegNode>::value_type(name, reg_root));
/*
    if(difis.find("letter") == difis.end()){
        cout << name << " not insert  " << difis.size() << endl;
    };
    //*/
}
int DirectTranser::converse(string reg){
    //cout << "begin converse dt" << endl;
    lex.init(reg);
    if(!lex.skipSpace()){
        return -1;
    }
    peek = lex.next();

    cout << "get first peek tag = " << (int)(peek -> tag) << endl;
    int i = DFAGraph.size();

    RegNode reg_root = E();
    cout << "converse regular finished " << endl;
    peek = new Token(FINISH_CHAR);
    RegNode reg_end = F();
    reg_root = concatenation(reg_root, reg_end);

    //cout << "after constructing root" << endl;
    vector<int> prev = reg_root.first;
    set<int> prev_set;
    for(vector<int>::iterator i = prev.begin(); i != prev.end(); i ++){
        prev_set.insert(*i);
    }
    DState * start = CheckState(prev_set);

    //cout << prev_set.size();
    /*cout << "new state : " << start -> dStateID << "  " ;
    for(set<int>::iterator ir = (start -> positions).begin(); ir != (start -> positions).end(); ir ++){
                  cout << *ir << "  ";
    }
    cout << endl;
    //*/
    int start_state = start -> dStateID;

    for(; i < (int)DFAGraph.size(); i++){
        for(int j = 0; j < MAX_SYMBOL_NUMBER; j ++){
            vector<int> next;
            vector<int>::iterator end = next.begin();
           /*
            if(j == FINISH_SYMBOL_INDEX){
                cout << "symb size $ : " << symble_aso_table[j].size() << "  " <<*symble_aso_table[j].begin() << endl;
                //for(vector<int>::iterator m = follows['a'].begin(); m != follows['a'].end(); m++)
                    //cout << "   " << *m << "   ";
            }
            //*/
            for(set<int>::iterator it = symble_aso_table[j].begin(); it != symble_aso_table[j].end(); it ++){
                if(DFAGraph.at(i).positions.count(*it) > 0){
                    vector<int>temp = next;
                    next = vector<int>(next.size()+follows[*it].size());
    //                next = set<int>(temp.size() + follows[*it].size());
                    end = set_union(temp.begin(), temp.end(), follows[*it].begin(), follows[*it].end(), next.begin());
                    //cout << "union end - begin " << (int)(end - next.begin())  << endl;
                }
            }
            //cout << " next  " << j << endl;
            //cout << (int)(end - next.begin()) << endl;

            set<int> next_set;
            for(vector<int>::iterator it = next.begin(); it != end; it ++){

                next_set.insert(*it);
                //cout << "insert " << *it << endl;

            }
            //char hkjk;
               // cin >> hkjk;
            //cout << "next set " << endl;
            if(next_set.size() == 0){
                DFAGraph.at(i).SetTrans(j, -1);
            }
            else{
                DState * nextDS = CheckState(next_set);
                DFAGraph.at(i).SetTrans(j, nextDS -> dStateID);
                //cout << "trans  " << i << "  " << j  << "  "<< nextDS->dStateID << endl;
                //cout << (char)j << "  ";
            }

        }

    }


    //regular_dfas.insert(map<int, set<int>>(start_state, finish));

    lex.skipSpace();
    //string act = lex.getAction();
    actions.insert(map<int,string>::value_type(start_state, lex.getAction()));

    return start_state;
    /*
    char input_char;
    do{
        cout << "current state : " << current_state << "  " << *DFAGraph[current_state].positions.begin() << endl;

        cin >> input_char;
        if(input_char != 'q')current_state = DFAGraph[current_state].Move(input_char);
    }while(current_state >= 0 && input_char != 'q');

    if(finish.count(current_state) > 0){
        cout << "finishi" << endl;
    }
    else{
        cout << "error" << endl;
    }
    //*/
}
void DirectTranser::comput_finish(){
    for(set<int>::iterator i = symble_aso_table[FINISH_SYMBOL_INDEX].begin() ; i != symble_aso_table[FINISH_SYMBOL_INDEX].end(); ++i){
        for(int j = 0; j < DFAGraph.size(); ++j)
            if(DFAGraph[j].positions.count(*i) > 0){
                finish_state_table[j] = true;
            }
    }
}
DState * DirectTranser::CheckState(set<int> pos){
    DState * hasher = hash_table[pos.size()%5][(*pos.begin()) % 8];
    DState * previous = hasher;


    for(;hasher != NULL;previous = hasher, hasher = hasher -> next){
       // cout << "hasher not null" << endl;
       // cout << "hs_s : "<< (hasher -> positions).size() << " pos s : " << pos.size() << " hs_b : "
        //  << *(hasher -> positions).begin() << " pos b :" << *pos.begin() << endl;
        if((hasher -> positions).size() == pos.size() &&
           *(hasher -> positions).begin() == *pos.begin()){
               bool flag = false;
               for(set<int>::iterator i = pos.begin(); i != pos.end(); i ++){
                //   cout << " " << *i<<" ";
                   if((hasher -> positions).count(*i) <= 0){
                       flag = true;
                       break;
                   }
               }
               if(!flag){
                   return hasher;
               }
        }
    }

    DState * state = new DState(pos);

    if(previous != NULL){
        previous -> next = state;
        //cout << this->next << "  " << NULL << endl;
        state -> next = NULL;
    }
    else{
        hash_table[pos.size()%5][(*pos.begin()) % 8] = state;
        state -> next = NULL;
    }
    DFAGraph.push_back(*state);
    return state;
}
