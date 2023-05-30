#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

#define tabela vector<pair<pair<string, int>, pair<bool, vector<int>*>>>

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
    while(i < linhas.size())
    {
        transform(linhas[i].begin(), linhas[i].end(), linhas[i].begin(), ::toupper);
        linhas[i] = remove_space_comments(linhas[i]);

        if(linhas[i][0] == '\0') {
            linhas.erase(linhas.begin() + i);
        }else if(linhas[i].back() == ':') {
            linhas[i] = linhas[i] + " " + linhas[i + 1];
            linhas.erase(linhas.begin() + i + 1);
            i++;
        } else{
            i++;
        }
    }
}

void open_file(string file)
{
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
            comandos.push_back(lista[i].substr(0, lista[i].size() - 2));
        }else{
            if(i == 0){
                comandos.push_back("");
            }
            if(lista[i] != ",")
                comandos.push_back(lista[i]);
        }
    }
    return comandos;   
}

bool erroLexico(vector<string> *s){
    //verifica erro lexico
}

bool erroSemantico(vector<string> *s){
    //verifica erro semantico
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
    for(int i = 0; i < listaPendencias->size(); i++){
        codigo[listaPendencias->at(i)] = valor;
    }
    listaPendencias->clear();
}

void montador(){
    int contador = 0, i = 0, aux;
    vector<string> comando;

    while(i < linhas.size()){
        comando = listaComando(linhas[i]);  // [simbolo, comando, arg1, agr2] => "" se n tiver simbolo ou agr2

        if(erroLexico(&comando)){
            //informar erro lexico
        }

        else if(erroSemantico(&comando)){
            //informar erro semantico
        }
        else{
            if(comando[0] != ""){
                //se esta na tabela de simbolos
                if((aux = findSimbolo(comando[0]) != -1)){
                    //se ja foi definido
                    if(tabelaSimbolos[aux].second.first == true){ 
                        //erro. declarou duas vezes um mesmo nome

                    }else{
                        //tratar lista de pendencias
                        tratarPendencias(contador, tabelaSimbolos[aux].second.second);
                        tabelaSimbolos[aux].second.first = true;
                    }
                }else{ // nao esta na tabela de simbolos => poe na tabela como definido
                    vector<int> listaPendencias;
                    tabelaSimbolos.push_back({{comando[0], contador}, {true, &listaPendencias}}); 
                }
            }

            if(comando[1] == "SECTION"){
                i++;
                continue;
            }
            
            //-1 => operacao não identificada
            codigo.push_back(opcode[comando[1]] == 0? -1 : opcode[comando[1]]);
                 
            
            if(comando[2] != ""){
                //se estiver na tabela de simbolos
                if((aux = findSimbolo(comando[2])) != -1){
                    //se estiver definido
                    if(tabelaSimbolos[aux].second.first == true){
                        //colocar o valor no codigo
                    }else{
                        //adicionar ocorrencia na lista de pendencias
                    }
                }else{ 
                    //por na tabela de simbolos e por na lista de pendencias
                }
            }

            if(comando[3] != ""){
                //se estiver na tabela de simbolos
                if((aux = findSimbolo(comando[2])) != -1){
                    //se estiver definido
                    if(tabelaSimbolos[aux].second.first == true){
                        //colocar o valor no codigo
                    }else{
                        //adicionar ocorrencia na lista de pendencias
                    }
                }else{ 
                    //por na tabela de simbolos e por na lista de pendencias
                }
            }

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

        create_arqv();
    }

    return 0;
}