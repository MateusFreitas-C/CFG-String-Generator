#include <iostream>
#include <fstream>
#include "./include/cfgFileReader.h"
#include "./include/cfg.h"

using namespace std;
// Função para o menu principal
void runGrammarGenerator(const string &file_to_open) {
    vector<string> variables, terminals;
    string initial_var;
    map<string, vector<string>> productions;

    if (!readInputFile(file_to_open, variables, initial_var, terminals, productions)) {
        return;
    }

    bool running = true;
    while (running) {
        cout << "\n----- Gerador de Cadeias para Gramaticas Livres de Contexto -----\n";

        if (validEntries(variables, initial_var, terminals, productions)) {
            cout << "Producoes:\n";
            for (auto &prod : productions) {
                cout << prod.first << ": ";
                for (auto &rule : prod.second) {
                    cout << rule << " ";
                }
                cout << endl;
            }

            cout << "\nSelecione o modo como a GLC gera:\n";
            cout << "1. Modo rapido\n";
            cout << "2. Modo detalhado\n";
            cout << "3. Sair\n";

            string menu;
            cin >> menu;

            if (menu == "1") {
                cout << "\n--- Modo Rapido ---" << endl;
                fast_mode(initial_var, variables, productions);
            } else if (menu == "2") {
                cout << "\n--- Modo Detalhado ---" << endl;
                detailed_mode(initial_var, variables, productions);
            } else if (menu == "3") {
                running = false;
            }
        } else {
            cout << "Existem erros no arquivo de entrada!" << endl;
            running = false;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        cout << "Passe o caminho do arquivo de teste corretamente." << endl;
        return 1;
    }

    string file_to_open = argv[1];
    runGrammarGenerator(file_to_open);

    return 0;
}
