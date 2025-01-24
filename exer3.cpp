#include <iostream>
#include <iomanip>
#include <string>

using std::cin;
using std::cout;
using std::string;
using std::setw;
using std::left;

string& trim(string &s) {
    if (s.empty())
        return s;

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ")+1);
    return s;
}
string& toupper(string &s) {
    for (size_t i = 0; i < s.length(); ++i) {
        s[i] = static_cast<char>(toupper(s[i])); 
    }
    return s;
}
int main(){
    string str, tmp, label, opcode, opperand;
    
    while(getline(cin, str)){ 
    
        if( str.substr(0, 1) == ".")
            continue;
        else { 

            if(str.length() > 17){
                tmp = str.substr(0, 8);
                label = toupper(trim(tmp));
                
                tmp = str.substr(9, 6);
                opcode = toupper(trim(tmp));
                
                tmp = str.substr(17, 18);
                opperand = trim(tmp);
                if ((opperand.find("'") != string::npos) && ((opperand.substr(0, 1) == "c") || (opperand.substr(0, 1) == "C"))){ 
                    tmp = opperand.substr(0, opperand.find("'"));
                    opperand = toupper(tmp) + opperand.substr(opperand.find("'"));
                } else 
                    opperand = toupper(opperand);
            }
            else {
                tmp = str.substr(0, 8);
                label = toupper(trim(tmp));
                
                tmp = str.substr(9, 6);
                opcode = toupper(trim(tmp));

                opperand = "";
            }

			if (opperand == "")
			  cout << left << setw(8) << label << ' ' << opcode <<'\n';
			else
			  cout << left << setw(8) << label << ' ' << left << setw(6) << opcode << "  " << opperand <<'\n';
        }
    }
        
    return 0;
}
