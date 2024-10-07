#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>


using namespace std;

vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void removeWhitespace(string &str) {
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}

// Função para ler o arquivo de entrada e processar as variáveis, terminais e produções
bool readInputFile(const string &file_to_open, vector<string> &variables, string &initial_var, vector<string> &terminals, map<string, vector<string>> &productions) {
    ifstream file(file_to_open);
    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        removeWhitespace(line);
        if (!line.empty() && line != "producoes") {
            lines.push_back(line);
        }
    }
    file.close();

    vector<string> prod_list;

    for (auto &line : lines) {
        if (line.find("variaveis") == 0) {
            line = line.substr(line.find(':') + 1);
            variables = split(line, ',');
        } else if (line.find("inicial") == 0) {
            initial_var = line.substr(line.find(':') + 1);
        } else if (line.find("terminais") == 0) {
            line = line.substr(line.find(':') + 1);
            terminals = split(line, ',');
        } else {
            prod_list.push_back(line);
        }
    }

    for (auto &prod : prod_list) {
        vector<string> parts = split(prod, ':');
        productions[parts[0]].push_back(parts[1]);
    }

    return true;
}

// Função para validar as entradas do arquivo de gramática
bool validEntries(const vector<string> &variables, const string &initial_var, const vector<string> &terminals, const map<string, vector<string>> &productions) {
    bool output = true;

    if (variables.empty()) {
        cout << "Erro nas variaveis." << endl;
        output = false;
    } else {
        for (auto &v : variables) {
            if (v.length() != 1 || !isupper(v[0])) {
                cout << "Erro nas variaveis." << endl;
                output = false;
            }
        }
    }

    if (initial_var.empty() || !isupper(initial_var[0]) || find(variables.begin(), variables.end(), initial_var) == variables.end()) {
        cout << "Erro no nao terminal inicial." << endl;
        output = false;
    }

    if (terminals.empty()) {
        cout << "Erro nos terminais." << endl;
        output = false;
    } else {
        for (auto &t : terminals) {
            if (!islower(t[0]) && !isdigit(t[0])) {
                cout << "Erro nos terminais." << endl;
                output = false;
            }
        }
    }

    for (auto &prod : productions) {
        if (find(variables.begin(), variables.end(), prod.first) == variables.end()) {
            cout << "Nao terminal do lado esquerdo nao presente em variaveis declaradas." << endl;
            output = false;
        }
    }

    if (productions.size() != variables.size()) {
        cout << "Erro nas produções." << endl;
        output = false;
    }

    return output;
}
