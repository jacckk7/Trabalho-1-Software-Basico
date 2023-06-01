#include<bits/stdc++.h> 
#define dbg(str, x) cout << str << ": " << x << endl;
using namespace std;

map<string, int> opcode = {
    {"ADD", 1},
    {"SUB", 2},
    {"MUL", 3},
    {"DIV", 4},
    {"JMP", 5},
    {"JMPN", 6},
    {"JMPP", 7},
    {"JMPZ", 8},
    {"COPY", 9},
    {"LOAD", 10},
    {"STORE", 11},
    {"INPUT", 12},
    {"OUTPUT", 13},
    {"STOP", 14}
};



vector<string> split(string s){
    int i = 0;
    string t = "";
    vector<string> lista;
    
    while(i < s.size()){
        
        if(s[i] == ' '){
            lista.push_back(t);
            t = "";
        }else{
            t = t + s[i];
        }
        i++;
    }
    lista.push_back(t);
    return lista;
}

vector<int> argOP(string s){
    int contMais = 0;
    vector<int> operacoes = vector<int>(2);
    vector<string> lista = split(s);

    for(int i = 0; i < lista.size(); i++){
        if(lista[i] == "+"){
            contMais++;
            if (i == lista.size() - 2){
                if(contMais == 2){
                    operacoes[1] = stoi(lista[i + 1]);
                }else{
                    operacoes[0] = stoi(lista[i + 1]);
                }
            }else{
                operacoes[0] = stoi(lista[i + 1]);            
            }
        }
    }
    return operacoes;
}

vector<string> listaComando(string s){
    vector<string> comandos;
    vector<string> lista = split(s);
    
    for(int i = 0; i < lista.size(); i++){
        if(lista[i][lista[i].size() - 1] == ':'){
            comandos.push_back(lista[i].substr(0, lista[i].size() - 1));
        }else{
            if(lista[i] == "+"){
                i = i + 2;
                continue;
            }
            if(i == 0){
                comandos.push_back("");
            }
            if(lista[i] != ",")
                comandos.push_back(lista[i]);
        }
    }
    if(comandos.size() < 4){
        int n = comandos.size();
        for(int i = 0; i < 4 - n; i++){
            comandos.push_back("");
        }
    }
    return comandos;   
}

int main(){
    string s = "LOAD OLD_DATA + 2";
    vector<int> operacoes = vector<int>(2);

    for(auto c: argOP(s)){
        cout << c << endl;
    }

    
    return 0;
}