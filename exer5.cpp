#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

using std::cin;
using std::cout;
using std::string;
using std::setw;
using std::left;
using std::right;
using std::setfill;
using std::hex;
using std::uppercase;
using std::ofstream;
using std::ifstream;
using std::stringstream;
using std::map;

string& trim(string &s) {
    if (s.empty())
        return s;

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ")+1);
    return s;
}
string search_op(string &op){
    map <string, string> opcode;
    opcode["ADD"]  = "18";       opcode["AND"]  = "40";        opcode["COMP"] = "28";
    opcode["DIV"]  = "24";       opcode["J"]    = "3C";        opcode["JEQ"]  = "30"; 
	opcode["JGT"]  = "34";       opcode["JLT"]  = "38";        opcode["JSUB"] = "48";
	opcode["LDA"]  = "00";       opcode["LDCH"] = "50";        opcode["LDL"]  = "08";
    opcode["LDX"]  = "04";       opcode["MUL"]  = "20";        opcode["OR"]   = "44";
    opcode["RD"]   = "D8";       opcode["RSUB"] = "4C";        opcode["STA"]  = "0C";
    opcode["STCH"] = "54";       opcode["STL"]  = "14";        opcode["STSW"] = "E8";
    opcode["STX"]  = "10";       opcode["SUB"]  = "1C";        opcode["TD"]   = "E0";
	opcode["TIX"]  = "2C";       opcode["WD"]   = "DC";

    if(opcode.find(op) != opcode.end()){
        return opcode[op];
    }
    else 
        return "";
}
string convert_const(string value){
    stringstream result;
    for (size_t i = 0; i < value.length(); i++) {
        result << uppercase << hex << static_cast<int>(value[i]); 
    }
    // cout << result.str();
    return result.str();
}
string convert_word(string value){
    stringstream result;

    result << hex << setw(6) << setfill('0') << right << uppercase << stoi(value); 
    return result.str();
}
int main(){
    ifstream in_SYM, in_INT;
    bool next_null;
    string str, str_sym, sym_label, sym_addr, tmp, label, opcode, opperand, pc, start_addr, re_op, re_sym, objcode, res_obj = "";
    map<string, string> sym_tab;

    in_SYM.open("SYMTAB");
    in_INT.open("INTFILE");
    
    getline(in_SYM, str_sym);
    str_sym = str_sym.erase(6, 1);
    str_sym = str_sym.erase(12, 1);
    while(getline(in_SYM, str)){
        sym_label = str.substr(0, 7);
        sym_addr = str.substr(7);
        sym_tab[trim(sym_label)] = trim(sym_addr);
    }
    
    while(getline(in_INT, str)){
        objcode = "";
        if(str.length() > 24){
            tmp = str.substr(0, 6);
            pc = trim(tmp);

            tmp = str.substr(7, 8);
            label = trim(tmp);
            
            tmp = str.substr(16, 6);
            opcode = trim(tmp);
            
            tmp = str.substr(24, 18);
            opperand = trim(tmp);
        }
        else {
            tmp = str.substr(0, 6);
            pc = trim(tmp);

            tmp = str.substr(7, 8);
            label = trim(tmp);
            
            tmp = str.substr(16, 6);
            opcode = trim(tmp);

            opperand = "";
        }
        if (opcode == "START") {                                       //head record     
            cout << "H" << str_sym << '\n';
        }
        if( opcode != "END"){
            re_op = search_op(opcode);
            if(re_op != ""){                                           //find opcode
                if(opperand != ""){
                    if (sym_tab.find(opperand) != sym_tab.end()) {    //find  symtab
                        re_sym = sym_tab[opperand];
                    } else if(opperand.find(",X") != string::npos){
                        re_sym = sym_tab[opperand.substr(0, opperand.length() - 2)];
                        int address = stoi(re_sym, nullptr, 16);  
                        address += 0x8000;  
                        stringstream ss;
                        ss << hex << uppercase << setfill('0') << setw(6) << address; 
                        re_sym = ss.str();
                    } else {
                        re_sym = "000000";
                        cout << "Undefined symbol: " << opperand << "\n";
                    }
                } else {
                    re_sym = "000000";
                }
                objcode = re_op + re_sym.substr(2);        
            } else if(opcode == "BYTE"){
                if(opperand.substr(0,1) == "X")
                    objcode = opperand.substr(2, opperand.size() - 3);
                else
                    objcode = convert_const(opperand.substr(2, opperand.size() - 3));
            } else if(opcode == "WORD")
                objcode = convert_word(opperand);
            // if(res_obj == ""){
            //     start_addr = pc;
            // }
            if(objcode == "" && res_obj != "")
                next_null = true;
            else
                next_null = false;
            if((objcode != "" && res_obj.length() + objcode.length() > 60) || next_null){
                cout << "T" << start_addr 
                    << hex << setw(2) << uppercase << setfill('0') << res_obj.length()/2 
                    << res_obj << '\n';
                res_obj = "";
            }
            if(res_obj == ""){
                start_addr = pc;
                // cout << start_addr << '\n';
            }
            res_obj += objcode;
        } else{
		  	if(res_obj != "")
            	cout << "T" << start_addr 
                    << hex << setw(2) << uppercase << res_obj.length()/2 
                    << res_obj << '\n';
            cout << "E" << str_sym.substr(6, 6) << '\n';
        }
    }

    // for (const auto& pair : sym_tab) {
    //     cout << pair.first << " -> " << pair.second << '\n';
    // }
    

    in_SYM.close();
    in_INT.close();
    return 0;
}
