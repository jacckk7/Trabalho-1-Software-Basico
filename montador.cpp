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
vector<pair<string, int>> tabelaUso;
vector<pair<string, int>> tabelaDef;
vector<int> relativos;
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

void create_arqv_pre_process(string file) {
    string filename(file + "_preprocess.exc");
    fstream output_fstream;

    output_fstream.open(filename, ios_base::out);
    if (!output_fstream.is_open()) {
        cerr << "Falhou ao criar arquivo " << filename << '\n';
    } else {
        for(int i = 0; i < linhas.size() - 1; i++) {
            output_fstream << linhas[i] << endl;
        }
        output_fstream << linhas.back();
    }
}

void create_arqv(string file) {
    string filename(file + ".exc");
    fstream output_fstream;

    output_fstream.open(filename, ios_base::out);
    if (!output_fstream.is_open()) {
        cerr << "Falhou ao criar arquivo " << filename << '\n';
    } else {
        for(int i = 0; i < codigo.size() - 1; i++) {
            output_fstream << codigo[i] << " ";
        }
        output_fstream << codigo.back();
    }
}

void create_arqv_obj(string file) {
    string filename(file + ".obj");
    fstream output_fstream;

    output_fstream.open(filename, ios_base::out);
    if (!output_fstream.is_open()) {
        cerr << "Falhou ao criar arquivo " << filename << '\n';
    } else {
        output_fstream << "USO" << endl;
        for(int i = 0; i < tabelaUso.size(); i++) {
            output_fstream << tabelaUso[i].first << " " << tabelaUso[i].second << endl;
        }

        output_fstream << "DEF" << endl;
        for(int i = 0; i < tabelaDef.size(); i++) {
            output_fstream << tabelaDef[i].first << " " << tabelaDef[i].second << endl;
        }

        output_fstream << "RELATIVOS" << endl;
        for(int i = 0; i < relativos.size() - 1; i++) {
            output_fstream << relativos[i] << " ";
        }
        output_fstream << relativos.back() << endl;

        output_fstream << "CODE" << endl;
        for(int i = 0; i < codigo.size() - 1; i++) {
            output_fstream << codigo[i] << " ";
        }
        output_fstream << codigo.back();
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
    linhas.clear();
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
    vector<int> operacoes = vector<int>(2);
    vector<string> lista = split(s);

    for(int i = 0; i < lista.size(); i++){
        if(lista[i] == "+"){
            if (i == lista.size() - 2 && (lista[0] == "COPY" || lista[1] == "COPY")){
                operacoes[1] = stoi(lista[i + 1]);
            }else{
                operacoes[0] = stoi(lista[i + 1]);
            }
        }
    }
    return operacoes;
}

bool erroLexico(string s){
    //verifica erro lexico
    if (isdigit(s[0])) {
        cout << "tem numero" << s << endl;
        return true;
    }

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '_'){
            continue;
        } else if (isdigit(s[i])) {
            continue;
        } else if (s[i] >= 'A' && s[i] <= 'Z') {
            continue;
        } else {
            return true;
        }
    }

    return false;
}

bool erroSintatico(vector<string> *s){
    //verifica erro sintatico
    int cnt = 0;
    for (int i = 0; i < s->size(); i++) {
        if (s->at(i).back() == ':') {
            cnt++;
        }
    }

    if (cnt > 1) {
        return true;
    } else {
        return false;
    }
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

void montador(bool temModulo, string file){
    int i = 0, aux;
    bool temText = false;
    bool temExtern = false;
    bool temPublic = false;
    bool temBegin = false;
    bool temEnd = false;
    vector<int> linhasPublic;
    vector<int> linhasExtern;
    vector<string> externos;
    vector<string> publicos;
    vector<string> comando;
    vector<pair<string, int>> labelsData;
    vector<pair<string, int>> labelsText;
    vector<int> operacoesArgumentos;

    if(temModulo){
        tabelaUso.clear();
        tabelaDef.clear();
        tabelaSimbolos.clear();
        relativos.clear();
        codigo.clear();
    }

    while(i < linhas.size()){

        comando = listaComando(linhas[i]);  // [simbolo, comando, arg1, agr2] => "" se n tiver simbolo ou agr2
        operacoesArgumentos = argOP(linhas[i]); //[fator1, fator2] para ser somado ao rotulo

        vector<string> linha = split(linhas[i]);
        if(linha[0].back() == ':' && erroLexico(comando[0])){
            //informar erro lexico
            printf("Erro léxico na linha %d: Rótulo inválido.", i + 1);
            break;

        } else if(erroSintatico(&linha)){
            //informar erro sintatico
            printf("Erro sintático na linha %d: Dois rótulos na mesma linha.", i + 1);
            break;
        
        }else{
            if(comando[1] == "BEGIN"){
                vector<int> * listaPendencias4 = new vector<int>;
                tabelaSimbolos.push_back({{comando[0], 0}, {true, listaPendencias4}});
                temBegin = true;
                i++;
                continue;

            }if(comando[1] == "END"){
                temEnd = true;
                i++;
                continue;
                
            }if(comando[0] == "EXTERN"){
                externos.push_back(comando[1]);
                vector<int> * listaPendencias3 = new vector<int>;
                tabelaSimbolos.push_back({{comando[1], -2}, {false, listaPendencias3}});
                temExtern = true;
                linhasExtern.push_back(i + 1);
                i++;
                continue;

            } if(comando[1] == "PUBLIC"){
                publicos.push_back(comando[2]);
                temPublic = true;
                linhasPublic.push_back(i + 1);
                i++;
                continue;

            }if(comando[0] != ""){
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
                    tabelaSimbolos.push_back({{comando[0], codigo.size()}, {true, listaPendencias1}}); 
                }
            }

            if(comando[1] == "SECTION"){
                if(comando[2] == "TEXT"){
                    temText = true;
                }
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
                if(comando[2].size() >= 2 && comando[2].at(1) == 'X'){
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
                        relativos.push_back(codigo.size() - 1);
                    }else{
                        //adicionar ocorrencia na lista de pendencias
                        codigo.push_back(operacoesArgumentos[0]);
                        relativos.push_back(codigo.size() - 1);
                        tabelaSimbolos[aux].second.second->push_back(codigo.size() - 1);
                    }
                }else{ 
                    //por na tabela de simbolos e por na lista de pendencias
                    vector<int> * listaPendencias2 = new vector<int>;
                    codigo.push_back(operacoesArgumentos[0]);
                    relativos.push_back(codigo.size() - 1);
                    listaPendencias2->push_back(codigo.size() - 1);
                    tabelaSimbolos.push_back({{comando[2], -2}, {false, listaPendencias2}});

                    if(comando[1] == "JMP" || comando[1] == "JMPN" || comando[1] == "JMPP" || comando[1] == "JMPZ"){
                        labelsText.push_back({comando[2], (i + 1)});
                    }else{
                        labelsData.push_back({comando[2], (i + 1)});
                    }
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
                        relativos.push_back(codigo.size() - 1);
                    }else{
                        //adicionar ocorrencia na lista de pendencias
                        codigo.push_back(operacoesArgumentos[1]);
                        relativos.push_back(codigo.size() - 1);
                        tabelaSimbolos[aux].second.second->push_back(codigo.size() - 1);
                    }
                }else{ 
                    //por na tabela de simbolos e por na lista de pendencias
                    vector<int> * listaPendencias3 = new vector<int>;
                    codigo.push_back(operacoesArgumentos[1]);
                    relativos.push_back(codigo.size() - 1);
                    listaPendencias3->push_back(codigo.size() - 1);
                    tabelaSimbolos.push_back({{comando[3], -2}, {false, listaPendencias3}});

                    labelsData.push_back({comando[3], (i + 1)});
                }
            }

            i++;
        }
    }

    //fazer as tabelas de uso e de definicoes
    if(temModulo){
        int aux;
        for(string a: externos){
            aux = findSimbolo(a);
            for(auto b : *tabelaSimbolos[aux].second.second){
                tabelaUso.push_back({a, b});
            }
        }

        for(string a: publicos){
            aux = findSimbolo(a);
            tabelaDef.push_back({a, tabelaSimbolos[aux].first.second});
        }
    }

    //verifica se tem TEXT
    if(!temText){
        printf("Erro semantico no arquivo %s.asm: Secao TEXT faltante.\n", file.c_str());
    }

    //verifica se tem public e extern onde n tem begin e end
    if((temExtern || temPublic) && (!temBegin || !temEnd)){
        for(auto a : linhasExtern){
            printf("Erro semantico na linha %d do arquivo %s: EXTERN sem ter BEGIN e END.\n", a, file.c_str());
        }

        for(auto a : linhasPublic){
            printf("Erro semantico na linha %d do arquivo %s: PUBLIC sem ter BEGIN e END.\n", a, file.c_str());
        }
    }

    //verifica labels de TEXT n definidas
    bool achou;
    for(i = 0; i < labelsText.size(); i++){
        achou= false;
        aux = findSimbolo(labelsText[i].first);

        for(string e : externos){
            if(labelsText[i].first == e) achou = true;
        }

        if(!achou && tabelaSimbolos[aux].second.first == false){
            printf("Erro semantico na linha %d no arquivo %s.asm: Rotulo %s nao definido na secao TEXT.\n", labelsText[i].second, file.c_str(), labelsText[i].first.c_str());
        }
    }

    //verifica labels de DATA n definidas
    for(i = 0; i < labelsData.size(); i++){
        achou= false;
        aux = findSimbolo(labelsData[i].first);

        for(string e : externos){
            if(labelsData[i].first == e) achou = true;
        }

        if(!achou && tabelaSimbolos[aux].second.first == false){
            printf("Erro semantico na linha %d no arquivo %s.asm: Dado %s nao definido na secao DATA.\n", labelsData[i].second, file.c_str(), labelsData[i].first.c_str());
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
        for(int i = 1; i < argc; i++){

            open_file(argv[i]);
            pre_process();
            create_arqv_pre_process(argv[i]);

            montador(argc > 2, argv[i]);

            if (argc <= 2) {
                create_arqv(argv[1]);
            } else {
                create_arqv_obj(argv[i]);
            }
        }
    }

    return 0;
}