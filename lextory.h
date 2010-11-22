#include <set>
#include <fstream>
#include "dfa_graph.h"

#define BEGIN  0
#define C_DECLAR  1
#define REGULAR_DEFIN  2
#define TRANSLATION_RULES   3
#define SUPPORTING  4
#define OUT_PUTING  5
#define FINISH  6

class Lextory{
    string tar_file;
    ifstream f_in;
    ofstream f_out;
    DirectTranser dt;
    vector<int> regular_defination;
public :
    Lextory(string filename){
        tar_file = filename;
        f_in.open(tar_file.c_str());
        f_out.open((tar_file+".out.c").c_str());
    }

    void Process();
};

void Lextory::Process(){
    char process_mode = BEGIN;
    string str_current;

    bool b_EFO = false;

    if(process_mode == BEGIN){
        //cout << "begin and efo is " << b_EFO << endl;
        for(;b_EFO = getline(f_in, str_current);){
            cout << "begin to process string " << str_current << endl;
            for(int i = 0; i < str_current.length(); ++i){
                if(str_current[i] == ' ' || str_current[i] == '\t' || str_current[i] == '\0'){
                    continue;
                }
                else if(str_current[i] == '%' && str_current[i + 1] == '{'){
                    process_mode = C_DECLAR;
                    str_current = str_current.substr(i+2);
                    break;
                }
                else {
                    process_mode = REGULAR_DEFIN;
                    str_current = str_current.substr(i);
                    break;
                }
            }
            if(process_mode != BEGIN){
                cout << "entering mode : " << (int)process_mode << endl;
                break;
            }
        }
    }
    if(C_DECLAR == process_mode){
        for(;b_EFO; b_EFO = getline(f_in, str_current)){
            cout << "c declar process " << str_current << endl;
            int i = 0;
            for(; i < str_current.length(); ++i){
                cout << "find char " << (char)str_current[i] << " and " << str_current[ i + 1] << endl;
                if(str_current[i] == '%' && str_current[ i + 1] == '}'){
                    process_mode = REGULAR_DEFIN;
                    break;
                }
            }
            f_out << str_current.substr(0, i) << endl;
            if(REGULAR_DEFIN == process_mode){
                str_current = str_current.substr(i+2);
                break;
            }
        }
    }
    if( REGULAR_DEFIN == process_mode){

            for(;b_EFO && !(str_current[0] == '%' && str_current[1] == '%');b_EFO = getline(f_in, str_current)){
                cout << "defination " << str_current << endl;
                dt.difination(str_current);
            }
            //b_EFO = getline(f_in, str_current);
            process_mode = TRANSLATION_RULES;
    }
    if(TRANSLATION_RULES == process_mode){
        while(b_EFO = getline(f_in, str_current) && !(str_current[0] == '%' && str_current[1] == '%') ){
            //dt.difination(str_current);
            cout << "reu rules " << str_current << endl;
            int start = dt.converse(str_current);
            if(start >= 0){
                regular_defination.push_back(start);
            }
        }
        dt.comput_finish();
        //b_EFO = getline(f_in, str_current);
        process_mode = SUPPORTING;
    }
    if(SUPPORTING == process_mode){
        for(;b_EFO = getline(f_in, str_current);){
            cout << "supporting " << str_current << endl;
            f_out << str_current << "\n" << endl;
        }
        f_out << "int tanns_table[" << dt.DFAGraph.size() << "]["
            << MAX_SYMBOL_NUMBER << "] = {" << endl;;
        for(int i = 0; i < dt.DFAGraph.size(); ++i){
            f_out << "{ ";
            for(int j = 0; j < MAX_SYMBOL_NUMBER; ++j){
                if(j != 0){
                     f_out << ", ";
                }
                if( !(j % 8) ){
                    f_out << endl;
                }
                f_out << dt.DFAGraph[i].Move(j);
            }
            f_out << "\n}";
            if(i != (dt.DFAGraph.size() - 1) ){
                f_out << "," << endl;
            }
        }
        f_out << "\n};\n" << endl;;

        f_out << "int finish_state_table[" << dt.DFAGraph.size() << "] = { ";
        for(int j = 0; j < dt.DFAGraph.size(); ++j){
            if(j != 0){
                f_out << ", ";
            }
            if( !(j % 8) ){
                f_out << endl;
            }
            f_out << dt.finish_state_table[j];
        }
        f_out << "\n}; \n" << endl;

        f_out << "int start_states[" << regular_defination.size() << "] = { ";
        for(int i = 0; i < regular_defination.size(); ++i){
            if(i != 0){
                f_out << ", ";
            }
            if( !(i % 8) ){
                f_out << endl;
            }
            f_out << regular_defination[i];
        }
        f_out << "\n};" << endl;

        f_out << "void lextory_process(){"
        "\n\tint finish_patten = -1; "
        "\n\tchar found_lexem[255]; "
        "\n\tint found_count = 0; "
        "\n\tfor(int i = 0; i < " << regular_defination.size()
        << "; ++i){"
        "\n\t\tint current_state = start_states[i]; "
        "\n\t\tpeek = getchar(); \n\t\tint current_found = 0; "
        "\n\t\twhile(trans_table[current_state][peek] >= 0){"
        "\n\t\t\tcurrent_found ++; "
        "\n\t\t\tcurrent_state = trans_table[current_state][peek]; "
        "\n\t\t\tpeek = getchar();"
        "\n\t\t}"
        "\n\t\tif(finish_state_table[current_state]){"
        "\n\t\t\tif(current_found > found_count){"
        "\n\t\t\t\tfound_count = current_found;"
        "\n\t\t\t\t//here seting lexem..."
        "\n\t\t\t\tfinish_patten = i;"
        "\n\t\t\t}"
        "\n\t\t\telse{"
        "\n\t\t\t\t; //do nothing..."
        "\n\t\t\t}"
        "\n\t\t}"
        "\n\t\telse{"
        "\n\t\t\t; //do nothing..."
        "\n\t\t}"
        "\n\t}"
        "\n\tswitch(finish_patten){"
        "\n\t\tcase -1 :"
        "\n\t\t//error...."
        "\n\t\tbreak;"
        << endl;

        for(int x = 0; x < regular_defination.size(); ++x){
            f_out << "\n\t\tcase " << x
                << " :\n\t\t" <<    dt.actions.find(regular_defination[x])->second << "\n\t\tbreak;" << endl;;
        }

        f_out << "\n\t\tdefault :"
        "\n\t\tbreak;"
        "\n\t}"
        "\n}\n" << endl;

        process_mode = FINISH;
    }
}
