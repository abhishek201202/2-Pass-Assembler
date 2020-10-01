#include<bits/stdc++.h>
using namespace std;

class Symbol{
public:
	string symbol, address;
	Symbol(string symbol, string address){
		this -> symbol = symbol;
		this -> address = address;
	}
};

class MOT{
public:
	string mnemonic, Class, code, size;
	MOT(string mnemonic, string Class, string code, string size){
		this -> mnemonic = mnemonic;
		this -> Class = Class;
		this -> code = code;
		this -> size = size;
	}
};
string n_to_base_k(int n,int r){
    string s = "";
    while(n != 0){
        int rem = n % r;
        if(rem <= 9){
             s = (char)('0' + rem) + s;
        }else{
            s = (char)('A' + rem - 10) + s;
        }
        n /= r;
    }
    while(s.size() != 2) s = '0' + s;
    return s;
}
string to_hex(string s){
	string L = s.substr(0, 2);
	string R = s.substr(2, 2);
	R = n_to_base_k(stoi(R), 16);
	return L + R;
}

class TwoPassAssembler{
private:
	int initial_address;
	vector<string> source_code;
	vector<string> source_code_address;
	vector<vector<string>> machine_code;
	vector<Symbol> symbols;
	vector<MOT> mot;
public:
	TwoPassAssembler(vector<string> source_code, int initial_address){
		this -> source_code = source_code;
		this -> initial_address = initial_address;
	}
private:
	void GenerateMachineOpcodeTable(){
		fstream my_file;
    	my_file.open("opcode_table.txt", ios :: in);
	    if(my_file.is_open()){
	        string s;
	        while(getline(my_file, s)){
	        	stringstream ss(s);
	        	string mnemoniccc, classs, codeee, sizeee;
	        	while(ss >> mnemoniccc >> classs >> codeee >> sizeee){
	        		mot.push_back(MOT(mnemoniccc, classs, codeee, sizeee));
	        	}
	        }
	        my_file.close();
	    }
	}

	void FirstPass(){
		for(int i = 0; i < source_code.size(); i++){
			string s = source_code[i];
			stringstream ss(s);
			string temp, curr;
			string mn = "$";
			while(ss >> temp){
				if(mn == "$") mn = temp;
				if(temp.back() == ':'){
					symbols.push_back(Symbol(temp.substr(0, temp.size() - 1), to_string(initial_address)));
				}else{
					curr += temp + " ";
				}
			}
	        source_code_address.push_back(to_string(initial_address));
	        int sz = -1;
	        for(int j = 0; j < mot.size(); j++){
	        	if(mot[j].mnemonic == mn){
	        		sz = stoi(mot[j].size);
	        		break;
	        	}
	        }
	        if(sz == -1) sz = 3;
	        initial_address += sz;
			source_code[i] = curr;
		}
	}


	void SecondPass(){
		for(int i = 0; i < source_code.size(); i++){
            string s = source_code[i];
            vector<string> res;
            res.push_back(source_code_address[i]);
            stringstream ss(s);
            string mn, classss;
            ss >> mn >> classss;
            bool okk = false;
            for(int j = 0; j < mot.size(); j++){
                if(mot[j].mnemonic == mn){
                    if(mot[j].Class == classss || mot[j].Class == "-"){
                        res.push_back(mot[j].code);
                        okk = true;
                        break;
                    }else if(mot[j].Class == "Label"){
                        for(int k = 0; k < symbols.size(); k++){
                            if(symbols[k].symbol == classss){
                                res.push_back(mot[j].code);
                                res.push_back(symbols[k].address);
                                okk = true;
                                break;
                            }
                        }
                        if(okk) break;
                    }else if(mot[j].Class == "Address"){
                    	res.push_back(mot[j].code);
                    	res.push_back(classss.substr(2, 2));
                    	res.push_back(classss.substr(0, 2));
                    	okk = true;
                    	break;
                    }
                }
            }
            if(!okk){
                int idx = 0;
                while(idx < s.size() && s[idx] != ' ') idx++;
                string mn = s.substr(0, idx);
                idx++;
                s = s.substr(idx);
                idx = 0;
                while(idx < s.size() && s[idx] != ',') idx++;
                string F = s.substr(0, idx);
                string S = s.substr(idx + 1, 4);
                // S.pop_back();
                for(int j = 0; j < mot.size(); j++){
                    if(mot[j].mnemonic == mn && mot[j].Class == F){
                        res.push_back(mot[j].code);
                        break;
                    }
                }
                res.push_back(S.substr(2));
                res.push_back(S.substr(0, 2));
            }
            machine_code.push_back(res);
        }
	}
	void ShowMachineCode(){
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "~~~~~~~~  Second Pass Completed ...... ~~~~~~~~~~" << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		for(int i = 0; i < machine_code.size(); i++){
			// for(int j = 0; j < machine_code[i].size(); j++){
			// 	if(j == 0){
			// 		cout << to_hex(machine_code[i][j]) << " ";
			// 	}else{
			// 		cout << machine_code[i][j] << " ";
			// 	}
			// }
			// cout << endl;
			for(string s : machine_code[i]){
				cout << s << " " ;
			}
			cout << endl;
		}
	}


	void ShowSymbols(){
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "~~~~~~~~~~~~ First Pass Completed .... ~~~~~~~~~~" << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		for(int i = 0; i < symbols.size(); i++){
			cout << symbols[i].symbol << " " << symbols[i].address << endl;
		}
		cout << endl << endl;

	}
	void ShowMot(){
		cout << "MOT :: " << endl;
		for(int i = 0; i < mot.size(); i++){
			cout << mot[i].mnemonic << " " << mot[i].Class << " " << mot[i].code << " " << mot[i].size << endl; 
		}
	}
	void SOURCE_CODE_ADDRESS(){
		cout << "source_code_address :: " << endl;
		for(int i = 0; i < source_code_address.size(); i++){
			cout << source_code_address[i] << endl;
		}
	}

	void ShowSourceCode(){
		for(int i = 0; i < source_code.size(); i++){
			cout << source_code[i] << endl;
		}
	}
public:
	void Run(){
		GenerateMachineOpcodeTable();
		// ShowSourceCode();
		FirstPass();
		// ShowSourceCode();
		ShowSymbols();
		// ShowMot();
		// SOURCE_CODE_ADDRESS();
		SecondPass();
		ShowMachineCode();
	}
};

void info(){
	cout << "Abhishek Soni ==> 2018UCO1673" << endl;
	cout << "Himanshu Pal  ==> 2018UCO1682" << endl;
	cout << endl;
}



int32_t main(){
	info();
	int init_address = 2000;
	fstream my_file;
	my_file.open("input.txt", ios :: in);
	vector<string> source_code;
	if(my_file.is_open()){
		string s; 
		while(getline(my_file, s)){
			source_code.push_back(s);
		}
		my_file.close();
	}
	TwoPassAssembler MyAssembler(source_code, init_address);
	MyAssembler.Run();
}

/*
LXI H,8000H
MOV C,M
INX H
MOV B,M
DCR C
loop: INX H
MOV A,M
CMP B
JNC skip
MOV B,A
skip: DCR C
JNZ loop
LXI H,9000H
MOV M,B
HLT
*/

/*
LDA 2050
MOV H,A	
LDA 2051	
ADD H	
MOV L,A	
MVI A 00		
ADC A	
MOV H,A	
HLT
*/