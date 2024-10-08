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

bool is_terminal(const std::string& str, const std::vector<std::string>& variables) {
    // Verifica se todos os caracteres da string `str` não estão presentes na lista de variáveis
    return std::all_of(str.begin(), str.end(), [&](char c) {
        // Cria uma string de um caractere e verifica se ela está nas variáveis
        return std::find(variables.begin(), variables.end(), std::string(1, c)) == variables.end();
    });
}

void fast_mode(const std::string &initial_var, const std::vector<std::string> &variables, const std::map<std::string, std::vector<std::string>> &productions) {
    std::vector<std::string> chain_possibility;     // Armazena as variáveis que podem gerar uma cadeia
    std::vector<std::string> ends_possibility;      // Armazena as produções terminais ou epsilon que são possíveis cadeias finais

    // Para cada produção da gramática
    for (const auto& [x, prods] : productions) {
        for (const auto& y : prods) {
             // Se a produção for epsilon ou uma produção terminal, adiciona às listas
            if (y == "epsilon" || is_terminal(y, variables)) {
                chain_possibility.push_back(x); // Adiciona a variável que gera a produção
                ends_possibility.push_back(y); // Adiciona a produção
            }
        }
    }

    bool continue_fast_mode = true;
    int k = 0; // Contador para alternar entre as produções

    while (continue_fast_mode) {
        std::string current = initial_var; // A cadeia começa com a variável inicial
        std::cout << "Derivacao:" << std::endl;
        std::cout << current;

        bool derivation_complete = false;
        while (!derivation_complete) {

            derivation_complete = true; // Assume que a derivação está completa até que uma variável seja encontrada

            // Procura por variáveis (não terminais) na cadeia atual
            for (const auto& var : variables) {
                size_t pos = current.find(var);     // Encontra a posição da variável na cadeia

                if (pos != std::string::npos) {
                    derivation_complete = false;    // Ainda há variáveis a serem substituídas
                    
                    // Encontra a variável na lista de possibilidades
                    auto it = std::find(chain_possibility.begin(), chain_possibility.end(), var);

                    // Obtém as produções dessa variável
                    const auto& prods = productions.at(*it);
                    
                    // Seleciona uma produção baseada no valor de k (itera entre as produções)
                    std::string replacement = prods[k % prods.size()];

                    // Se a produção for epsilon, remove a variável
                    if (replacement == "epsilon") replacement = ""; 

                    // Substitui a variável encontrada pela produção
                    current.replace(pos, var.length(), replacement);
                    std::cout << " -> " << current;
                    
                    k++; // Incrementa o contador para alternar entre as produções
                    break;
                }
            }
        }

        std::cout << "\nCadeia gerada:" << std::endl;
        std::cout << current << std::endl;

        std::cout << "\nDeseja gerar outra cadeia? (s/n)" << std::endl;
        std::string keep;
        std::cin >> keep;
        if (keep != "s" && keep != "S") {
            continue_fast_mode = false;
        }
    }
}