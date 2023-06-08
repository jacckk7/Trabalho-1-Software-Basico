#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>

using namespace std;

vector<int> codigo;
vector<int> relativos;
vector<bool> rel_passed(216, true);
vector<pair<string, int>> uso;
map<string, int> definicao;
vector<int> fator_correcao;

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

void open_file(string file, int fc)
{
    vector<string> objeto;
    file = file + ".obj";
    ifstream arqv(file);
    string hold;
    if (arqv.is_open())
    {
        while (arqv.peek() != EOF)
        {
            getline(arqv, hold, '\n');
            objeto.push_back(hold);
        }
    }
    arqv.close();

    bool is_uso = false;
    bool is_def = false;
    bool is_rel = false;
    bool is_code = false;

    for (int i = 0; i < objeto.size(); i++) {
        if (objeto[i] == "USO") {
            is_uso = true;
            is_def = false;
            is_rel = false;
            is_code = false;
            continue;
        } else if (objeto[i] == "DEF") {
            is_uso = false;
            is_def = true;
            is_rel = false;
            is_code = false;
            continue;
        } else if (objeto[i] == "RELATIVOS") {
            is_uso = false;
            is_def = false;
            is_rel = true;
            is_code = false;
            continue;
        } else if (objeto[i] == "CODE") {
            is_uso = false;
            is_def = false;
            is_rel = false;
            is_code = true;
            continue;
        }

        vector<string> hold = split(objeto[i]);

        if (is_uso) {
            uso.push_back(make_pair(hold[0], stoi(hold[1]) + fc));
        } else if (is_def) {
            definicao[hold[0]] = stoi(hold[1]) + fc;
        } else if (is_rel) {
            for (int j = 0; j < hold.size(); j++) {
                relativos.push_back(stoi(hold[j]) + fc);
            }
        } else if (is_code) {
            for (int j = 0; j < hold.size(); j++) {
                codigo.push_back(stoi(hold[j]));
            }
        }
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

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Nenhum arquivo encontrado\n");
    }
    else
    {

        for (int i = 1; i < argc; i++) {
            fator_correcao.push_back(codigo.size());
            open_file(argv[i], fator_correcao.back());
        }

        for (int i = 0; i < uso.size(); i++) {
            codigo[uso[i].second] += definicao[uso[i].first];
            rel_passed[uso[i].second] = false;
        }
        
        int j = 0;
        for (int i = 0; i < relativos.size(); i++) {
            while (j < fator_correcao.size() - 1 && relativos[i] >= fator_correcao[j + 1]) {
                j++;
            }
            if (rel_passed[relativos[i]]) {
                codigo[relativos[i]] += fator_correcao[j];
            }
        }

        create_arqv(argv[1]);
    }

    return 0;
}