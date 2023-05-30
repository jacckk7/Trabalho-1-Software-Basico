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

vector<string> listaPaises(string *line){   //retorna uma lista com os paises na mesma linha
    int i;
    string linha = *line, pais;
    vector<string> lista;
    while (linha.size() > 0){
        i = linha.find(" ");
        cout << i << endl;
        pais = linha.substr(0, i);
        if (pais[pais.size() - 1] == ':' | pais[pais.size() - 1] == ';' | pais[pais.size() - 1] == ','){
            lista.push_back(pais.substr(0, pais.size() -1));
        }
        linha = linha.substr(i + 1, (linha.size() - (pais.size() + 1)));
    }

    return lista;    
}

vector<string> listaComando(string s){
    int i, cont = 0;
    string linha = s, tolken;
    vector<string> lista(4);
    while (linha.size() > 0){
        i = linha.find(" ");
        tolken = linha.substr(0, i);
        cout << i << endl;
        cout << tolken << endl;
        if(tolken != ","){
            if (tolken[tolken.size() - 1] == ':'){
                lista[0] = tolken.substr(0, tolken.size() -1);
            }else{
                lista[cont] = tolken.substr(0, tolken.size());
            }
            cont++;
        }
        linha = linha.substr(i + 1, (linha.size() - (tolken.size() + 1)));
    }
    return lista;   
}
vector<pair<pair<string, int>, pair<bool, vector<int>*>>> v;
int findSimbolo(string s){
     for(int i = 0; i < v.size(); i++){
        if(v[i].first.first == s){
            return i;
        }
     }
     return -1;
}

vector<string> split(string s){
    int i = 0;
    string t = "";
    vector<string> lista;
    
    while(i < s.size()){
        
        if(s[i] == ' '){
            cout << "achou espaco\n";
            lista.push_back(t);
            t = "";
        }else{
            cout << "achou letra\n";
            t = t + s[i];
        }
        i++;
    }
    lista.push_back(t);
    return lista;
}

int main(){
    string s, palavra;cin>>s;
    stringstream ss(s);
    vector<string> lista;

    lista = split(s);

    for(auto c : lista) cout << c << endl;

    return 0;
}