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


int main(){
    
    cout << opcode["ola"] << " " << opcode["ADD"];


    return 0;
}