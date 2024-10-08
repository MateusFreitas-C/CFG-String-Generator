#include "../include/cfg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <set>

using namespace std;

// Função para o modo detalhado
void detailed_mode(const string &initial_var, const vector<string> &variables, const map<string, vector<string>> &productions) {
    string chain = initial_var;
    string derivation_path = chain + " -> "; // Inicializamos a cadeia com o símbolo inicial
    cout << "Derivacao:\n" << derivation_path;

    // Loop até que não haja mais variáveis a serem substituídas
    while (any_of(variables.begin(), variables.end(), [&](string v) { return chain.find(v) != string::npos; })) {
        string current_variable;

        // Localiza a primeira variável na cadeia atual
        for (char &c : chain) {
            if (isupper(c)) {
                current_variable = string(1, c);
                break;
            }
        }

        // Exibe as produções possíveis para a variável encontrada
        cout << "\nEscolha a operacao de " << current_variable << ": \n";
        for (auto &rule : productions.at(current_variable)) {
            cout << rule << " ";
        }
        cout << endl;

        string operation;
        cin >> operation;

        // Verifica se a operação escolhida é válida
        if (find(productions.at(current_variable).begin(), productions.at(current_variable).end(), operation) != productions.at(current_variable).end()) {
            // Substitui a variável escolhida pela produção selecionada
            size_t pos = chain.find(current_variable);
            chain.replace(pos, current_variable.length(), operation == "epsilon" ? "" : operation);
            
            // Atualiza o caminho da derivação
            derivation_path += chain + " -> ";
            cout << derivation_path;
        } else {
            cout << "Operacao invalida!" << endl;
        }
    }

    // Exibir o caminho completo da derivação
    cout << "\nCadeia gerada:\n" << chain << endl;
}

void fast_mode(const std::string &initial_var, const std::vector<std::string> &variables, const std::map<std::string, std::vector<std::string>> &productions) {
    std::vector<std::string> chain_possibility;
    std::vector<std::string> ends_possibility;

    for (const auto& [x, prods] : productions) {
        for (const auto& y : prods) {
            if (y == "epsilon") {
                chain_possibility.push_back(x);
                ends_possibility.push_back(y);
                continue;
            }
            bool is_end_possibility = true;
            for (char i : y) {
                if (std::find(variables.begin(), variables.end(), std::string(1, i)) != variables.end()) {
                    is_end_possibility = false;
                    break;
                }
            }
            if (is_end_possibility) {
                chain_possibility.push_back(x);
                ends_possibility.push_back(y);
            }
        }
    }

    bool continue_fast_mode = true;
    int k = 0;

    while (continue_fast_mode) {
        std::string chain;
        std::vector<std::string> chain_path;

        if (k == chain_possibility.size()) {
            k = 0;
        }
        chain_path.push_back(ends_possibility[k]);
        chain_path.push_back(chain_possibility[k]);
        k++;

        int i = 1;
        while (chain_path[i] != initial_var) {
            for (const auto& [x, prods] : productions) {
                for (const auto& y : prods) {
                    if (y.find(chain_path[i]) != std::string::npos) {
                        chain_path.push_back(x);
                        i++;
                        break;
                    }
                }
                if (chain_path[i] == initial_var) {
                    break;
                }
            }
        }

        std::cout << "Derivacao:" << std::endl;
        std::vector<std::string> chain_sub_str;
        chain_sub_str.push_back(chain_path.back() + " -> ");
        chain += chain_sub_str[0];

        int j = 0;
        for (int i = chain_path.size() - 1; i > 0; i--) {
            auto it = productions.find(chain_path[i]);
            for (const auto& x : it->second){
                if (x.find(chain_path[i - 1]) != std::string::npos) {
                    std::regex pattern("(.*?)(?= ->)");
                    std::smatch match;
                    if (std::regex_search(chain_sub_str[j], match, pattern)) {
                        std::string aux = match.str(0);
                        if (x == "epsilon") {
                            aux.replace(aux.find(chain_path[i]), chain_path[i].length(), "");
                        } else {
                            aux.replace(aux.find(chain_path[i]), chain_path[i].length(), x);
                        }
                        chain_sub_str.push_back(aux + " -> ");
                        j++;
                        chain += chain_sub_str[j];
                    }
                    break;
                }
            }
        }

        std::cout << chain.substr(0, chain.length() - 4) << std::endl;
        std::cout << "Cadeia gerada:" << std::endl;
        std::cout << chain_sub_str.back().substr(0, chain_sub_str.back().length() - 4) << std::endl;

        std::cout << "\nDeseja gerar outra cadeia? (s/n)" << std::endl;
        std::string keep;
        std::cin >> keep;
        if (keep != "s" && keep != "S") {
            continue_fast_mode = false;
        }
    }
}