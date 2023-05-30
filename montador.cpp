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

vector<string> listaComando(string s){
    int i, cont = 0;
    string linha = s, tolken;
    vector<string> lista(4);
    while (linha.size() > 0){
        i = linha.find(" ");
        tolken = linha.substr(0, i);
        if(tolken != ","){
            if (tolken[tolken.size() - 1] == ':'){
                lista.push_back(tolken.substr(0, tolken.size() -1));
            }else{
                if(lista.size() == 0){
                    lista.push_back("");
                }
                lista.push_back(tolken.substr(0, tolken.size()));
            }
            cont++;
        }
        linha = linha.substr(i + 1, (linha.size() - (tolken.size() + 1)));
    }
    return lista;   
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

int getOpcode(string op){
    
}

void montador(){
    int contador = 0, i = 0, aux;
    vector<string> comando;

    while(i < linhas.size()){
        comando = listaComando(linhas[i]);  // [simbolo, comando, arg1, agr2] => "" se n tiver simbolo ou agr2

        if(erroLexico(&comando)){
            //tratar erro lexico
        }

        else if(erroSemantico(&comando)){
            //tratar erro semantico
        }
        else{
            if(comando[0] != ""){
                if((aux = findSimbolo(comando[0]) != -1)){  //se esta na tabela de simbolos
                    if(tabelaSimbolos[aux].second.first == true){ //se ja foi definido
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
            codigo.push_back(comando[1]);     
            

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