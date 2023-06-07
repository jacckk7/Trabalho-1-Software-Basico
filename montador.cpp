#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>

#define tabela vector<pair<pair<string, int>, pair<bool, vector<int>*>>>
#define printL(x) for(auto a: x) cout<<(a == ""? "nada":a)<<" "; cout<<endl;
#define print(x) cout<<x<<endl;
#define dbg(x, y) cout<<x<<": "<<y<<endl;

using namespace std;

vector<string> linhas;
vector<int> codigo;
tabela tabelaSimbolos;
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

void create_arqv() {
    string filename("preprocess.exc");
    fstream output_fstream;

    output_fstream.open(filename, ios_base::out);
    if (!output_fstream.is_open()) {
        cerr << "Falhou ao criar arquivo " << filename << '\n';
    } else {
        for(int i = 0; i < linhas.size(); i++) {
            output_fstream << linhas[i] << endl;
        }
    }
}

string remove_space_comments(string linha) {
    linha.erase(linha.begin(), find_if(linha.begin(), linha.end(), [](int c) {
        return !isspace(c);
    }));

    size_t comment_pos = linha.find(';');
    if (comment_pos != string::npos) {
        linha.erase(comment_pos);
    }

    linha.erase(find_if(linha.rbegin(), linha.rend(), [](int c) {
        return !isspace(c);
    }).base(), linha.end());

    linha.erase(unique(linha.begin(), linha.end(), [](char a, char b) {
        return isspace(a) && isspace(b);
    }), linha.end());

    return linha;
}

void pre_process()
{
    int i = 0;
    bool data = false;
    vector<string> section_data;

    while(i < linhas.size())
    {
        transform(linhas[i].begin(), linhas[i].end(), linhas[i].begin(), ::toupper);
        linhas[i] = remove_space_comments(linhas[i]);

        if (i == 0 && linhas[i] == "SECTION DATA") {
            data = true;
        }

        if (data) {
            if(linhas[i][0] == '\0') {
                linhas.erase(linhas.begin() + i);
            }else if(section_data.size() > 0 && section_data[section_data.size() - 1].back() == ':') {
                section_data[section_data.size() - 1] = section_data[section_data.size() - 1] + " " + linhas[i];
                linhas.erase(linhas.begin() + i);
            } else{
                section_data.push_back(linhas[i]);
                linhas.erase(linhas.begin() + i);
            }

            if(linhas[i + 1] == "SECTION TEXT") {
                data = false;
            }
        } else {
            if(linhas[i][0] == '\0') {
                linhas.erase(linhas.begin() + i);
            }else if(i > 0 && linhas[i - 1].back() == ':') {
                linhas[i - 1] = linhas[i - 1] + " " + linhas[i];
                linhas.erase(linhas.begin() + i);
            } else {
                i++;
            } 
        }
    }

    for (int i = 0; i <section_data.size(); i++) {
        linhas.push_back(section_data[i]);
    }
}

void open_file(string file)
{
    file = file + ".asm";
    ifstream arqv(file);
    string hold;
    if (arqv.is_open())
    {
        while (arqv.peek() != EOF)
        {
            getline(arqv, hold, '\n');
            linhas.push_back(hold);
        }
    }
    arqv.close();
}

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

bool erroLexico(vector<string> *s){
    //verifica erro lexico
    return false;
}

bool erroSemantico(vector<string> *s){
    //verifica erro semantico
    return false;
}

int findSimbolo(string s){
     for(int i = 0; i < tabelaSimbolos.size(); i++){
        if(tabelaSimbolos[i].first.first == s){
            return i;
        }
     }
     return -1;
}

void tratarPendencias(int valor, vector<int> *listaPendencias){
    int aux;
    for(int i = 0; i < listaPendencias->size(); i++){
        aux = codigo[listaPendencias->at(i)];
        aux = aux + valor;        
        codigo[listaPendencias->at(i)] = aux;
    }
    listaPendencias->clear();
}

void montador(){
    int i = 0, aux;
    vector<string> comando;
    vector<int> operacoesArgumentos;

    while(i < linhas.size()){

        comando = listaComando(linhas[i]);  // [simbolo, comando, arg1, agr2] => "" se n tiver simbolo ou agr2
        operacoesArgumentos = argOP(linhas[i]); //[fator1, fator2] para ser somado ao rotulo

        printL(comando)

        if(erroLexico(&comando)){
            //informar erro lexico
            break;
        }

        else if(erroSemantico(&comando)){
            //informar erro semantico
            break;
        }
        else{
            if(comando[0] != ""){
                //se esta na tabela de simbolos
                aux = findSimbolo(comando[0]);
                if(aux != -1){
                    //se ja foi definido
                    if(tabelaSimbolos[aux].second.first == true){ 
                        //erro. declarou duas vezes um mesmo nome
                    }else{
                        //tratar lista de pendencias
                        tratarPendencias(codigo.size(), tabelaSimbolos[aux].second.second);
                        tabelaSimbolos[aux].second.first = true; //definido
                        tabelaSimbolos[aux].first.second = codigo.size(); //valor atualizado
                    }
                }else{ // nao esta na tabela de simbolos => poe na tabela como definido
                    vector<int> * listaPendencias1 = new vector<int>;
                    tabelaSimbolos.push_back({{comando[0], codigo.size() - 1}, {true, listaPendencias1}}); 
                }
            }

            if(comando[1] == "SECTION"){
                i++;
                continue;
            }
            
            else if(comando[1] == "SPACE"){
                if(comando[2] == ""){
                    codigo.push_back(0);
                }else{
                    for(int j = 0; j < stoi(comando[2]); j++){
                        codigo.push_back(0);
                    }
                }
                i++;
                continue;
            }

            else if(comando[1] == "CONST"){
                //se for em hexadecimal
                if(comando[2].size() >= 2 && comando[2].at(1) == 'x'){
                    //converte para decimal
                    int intHex;
                    stringstream ss;
                    ss << hex << comando[2];
                    ss >> intHex;
                    codigo.push_back(intHex);
                }else{
                    codigo.push_back(stoi(comando[2]));
                }
                i++;
                continue;
            }
            
            //-1 => operacao não identificada
            else{
                codigo.push_back(opcode[comando[1]] == 0? -1 : opcode[comando[1]]);
            }    
            
            if(comando[2] != ""){
                aux = findSimbolo(comando[2]);

                //se estiver na tabela de simbolos
                if(aux != -1){
                    //se estiver definido
                    if(tabelaSimbolos[aux].second.first == true){
                        //colocar o valor no codigo
                        aux = tabelaSimbolos[aux].first.second + operacoesArgumentos[0];
                        codigo.push_back(aux);
                    }else{
                        //adicionar ocorrencia na lista de pendencias
                        codigo.push_back(operacoesArgumentos[0]);
                        tabelaSimbolos[aux].second.second->push_back(codigo.size() - 1);
                    }
                }else{ 
                    //por na tabela de simbolos e por na lista de pendencias
                    vector<int> * listaPendencias2 = new vector<int>;
                    codigo.push_back(operacoesArgumentos[0]);
                    listaPendencias2->push_back(codigo.size() - 1);
                    tabelaSimbolos.push_back({{comando[2], -2}, {false, listaPendencias2}});
                }
            }
            
            if(comando[3] != ""){
                aux = findSimbolo(comando[3]);
                
                //se estiver na tabela de simbolos
                if(aux != -1){
                    //se estiver definido
                    if(tabelaSimbolos[aux].second.first == true){
                        //colocar o valor no codigo
                        aux = tabelaSimbolos[aux].first.second + operacoesArgumentos[1];
                        codigo.push_back(aux);
                    }else{
                        //adicionar ocorrencia na lista de pendencias
                        codigo.push_back(operacoesArgumentos[1]);
                        tabelaSimbolos[aux].second.second->push_back(codigo.size() - 1);
                    }
                }else{ 
                    //por na tabela de simbolos e por na lista de pendencias
                    vector<int> * listaPendencias3 = new vector<int>;
                    codigo.push_back(operacoesArgumentos[1]);
                    listaPendencias3->push_back(codigo.size() - 1);
                    tabelaSimbolos.push_back({{comando[2], -2}, {false, listaPendencias3}});
                }
            }

            i++;
        }
        
    }

    
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Nenhum arquivo encontrado\n");
    }
    else
    {
        open_file(argv[1]);
        pre_process();

        for (int i = 0; i < linhas.size(); i++)
        {
            cout << linhas[i] << endl;
        }
        cout << endl;

        /* montador();

        cout << endl << "codigo feito:" << endl;
        for(auto c: codigo){
            cout << c << endl;
        }

        cout << endl << "tabela de simbolos:" << endl;

        for(auto a : tabelaSimbolos){
            cout<<a.first.first << " " << a.first.second << " " << a.second.first << " [";
            cout << a.second.second->size() << " ";
            cout<<"]\n";
        } */

        create_arqv();
    }

    return 0;
}