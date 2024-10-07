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

// Função para verificar se a cadeia gerada já foi usada
bool isChainUsed(const string &chain, set<string> &used_chains) {
    if (used_chains.find(chain) != used_chains.end()) {
        return true; // Cadeia já foi gerada
    }
    used_chains.insert(chain); // Armazena a nova cadeia
    return false;
}

// Função auxiliar para substituir uma variável na cadeia
string substituirVariavel(const string &cadeia, size_t posicao, const string &substituicao) {
    string nova_cadeia = cadeia;
    if (substituicao == "epsilon") {
        nova_cadeia.replace(posicao, 1, "");
    } else {
        nova_cadeia.replace(posicao, 1, substituicao);
    }
    return nova_cadeia;
}

void fast_mode(const string &initial_var, const vector<string> &variables, const map<string, vector<string>> &productions) {
    set<string> used_chains; // Para armazenar as cadeias já geradas
    bool continue_fast_mode = true;

    while (continue_fast_mode) {
        string current_chain = initial_var; // Começa a partir da variável inicial
        vector<string> derivation_steps; // Para armazenar os passos da derivação
        bool found_variable = true;

        cout << "--- Gerando uma nova cadeia ---" << endl;

        // Enquanto houver variáveis na cadeia
        while (found_variable) {
            found_variable = false;

            for (size_t i = 0; i < current_chain.size(); i++) {
                char symbol = current_chain[i];
                if (isupper(symbol)) {  // Se o símbolo é uma variável (não terminal)
                    found_variable = true;
                    string variable(1, symbol);  // Converter char para string

                    if (productions.find(variable) != productions.end()) {
                        bool new_chain_found = false;

                        for (const string &replacement : productions.at(variable)) {
                            string new_chain = substituirVariavel(current_chain, i, replacement);

                            // Verifica se essa cadeia já foi gerada
                            if (used_chains.find(new_chain) == used_chains.end()) {
                                current_chain = new_chain; // Atualiza a cadeia atual
                                derivation_steps.push_back(variable + " -> " + replacement);
                                cout << "Produção aplicada: " << variable << " -> " + replacement << endl;
                                new_chain_found = true;
                                break; // Para substituir apenas uma variável por iteração
                            }
                        }

                        // Se não encontrou uma nova cadeia, continuar a busca
                        if (!new_chain_found) {
                            continue; // Tente a próxima variável na sequência
                        }
                    } else {
                        cout << "Erro: Variável " << variable << " não encontrada nas produções." << endl;
                        return;
                    }
                    break; // Para substituir apenas a primeira variável encontrada
                }
            }
        }

        // Exibir a cadeia gerada e os passos da derivação
        cout << "Cadeia final gerada: " << current_chain << endl;
        cout << "Derivação (passos):" << endl;
        for (const string &step : derivation_steps) {
            cout << step << endl;
        }

        // Adiciona a nova cadeia ao conjunto de cadeias usadas
        used_chains.insert(current_chain);

        // Perguntar se o usuário deseja gerar outra cadeia
        cout << "\nDeseja gerar outra cadeia? (s/n)" << endl;
        string keep;
        cin >> keep;
        if (keep != "s" && keep != "S") {
            continue_fast_mode = false;
        }
    }
}