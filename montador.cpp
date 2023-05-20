#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

vector<string> linhas;

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