#include<bits/stdc++.h> 
#define dbg(str, x) cout << str << ": " << x << endl;
using namespace std;

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
    vector<int> operacoes;

    operacoes.clear();
    cout << "oi\n";
    
    return 0;
}