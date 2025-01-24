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
bool find_op(string& opcode){
    string instruction[] = { "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF", "DIVR",
				"FIX", "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL", "LDS",
				"LDT", "LDX", "LPS", "MUL", "MULF", "MULR", "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR",
				"SIO", "SSK", "STA", "STB", "STCH", "STF", "STI", "STL", "STS", "STSW", "STT", "STX", "SUB", "SUBF",
				"SUBR", "SVC", "TD", "TIO", "TIX", "TIXR", "WD" };
    int size = sizeof(instruction) / sizeof(string);
    for (int i = 0; i < size; ++i) {
        if (instruction[i] == opcode) {
            return true;
        }
    }
    return false;
}
int main(){
    string str, tmp, label, opcode, opperand;
    ifstream in_sym;
    ofstream out_int, out_sym;
    stringstream sym_stream;
    int pc = 0x0, start = 0x0, p_length = 0x0, len;
    bool error;
    map<string, int> sym_tab;

    in_sym.open("SYMTAB");
    out_sym.open("SYMTAB");
    out_int.open("INTFILE");

    while(getline(cin, str)){ 
        
	  	if( str.substr(0, 1) == ".")
            continue;
        else { 

            if(str.length() > 17){
                tmp = str.substr(0, 8);
                label = trim(tmp);
                
                tmp = str.substr(9, 6);
                opcode = trim(tmp);
                
                tmp = str.substr(17, 18);
                opperand = trim(tmp);
            }
            else {
                tmp = str.substr(0, 8);
                label = trim(tmp);
                
                tmp = str.substr(9, 6);
                opcode = trim(tmp);

                opperand = "";
            }

			if (opcode == "START") {
                pc = stoi(opperand, nullptr, 16);
                start = pc;
                out_int << uppercase << hex << setw(6) << right << setfill('0') << pc 
				  		<< ' ' << setfill(' ') << left << setw(8) << label 
						<< ' ' << setfill(' ') << left << setw(6) << opcode 
						<< "  " << opperand << '\n';
                out_sym << setw(7) << left << label 
				  		<< right << setw(6) << setfill('0') << opperand << ' ';
                continue;
            } 
            if (error) {
                out_int << "error >> invalid operation code" << '\n';
        		error = false;
            } else if (opperand == "")
                out_int << uppercase << hex << setw(6) << right << setfill('0') << pc 
				  		<< ' ' << setfill(' ') << left << setw(8) << label 
						<< ' ' << opcode << '\n';
            else
                out_int << uppercase << hex << setw(6) << right << setfill('0') << pc 
				  		<< ' ' << setfill(' ') << left << setw(8) << label 
						<< ' ' << setfill(' ') << left << setw(6) << opcode 
						<< "  " << opperand <<'\n';
            while( opcode != "END"){
                if (label != "") {
                    if (sym_tab.find(label) != sym_tab.end()) 
                        sym_stream << "error >> duplicate symbol\n";
                    else {
                        sym_tab[label] = pc;
                        sym_stream << setfill(' ') << setw(7) << left << label 
						  			<< right << setw(6) << setfill('0') << hex << uppercase << pc 
									<< '\n';
                    }
                }
                if (find_op(opcode))
                    pc += 3;
                else if (opcode == "WORD")   
                    pc += 3;
                else if (opcode == "RESW")
                    pc += 3 * stoi(opperand);
                else if (opcode == "RESB")
                    pc += stoi(opperand);
                else if (opcode == "BYTE") {
                    if(opperand.substr(0,1) == "C")
                        len = opperand.substr(2).length() - 1;
                    else  // X
                        len = (opperand.substr(2).length() - 1) / 2;
                    pc += len;
                } 
                else
                    error = true;
                break;
            }
        }
    }
    p_length = pc - start;
    out_sym << hex << right << setw(6) << setfill('0') << uppercase << p_length << '\n';
    out_sym << sym_stream.str();

    in_sym.close();
    out_sym.close();
    out_int.close();
    return 0;
}
