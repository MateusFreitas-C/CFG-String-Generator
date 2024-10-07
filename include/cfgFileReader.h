#ifndef CFG_FILE_H
#define CFG_FILE_H

#include <vector>
#include <string>
#include <map>

std::vector<std::string> split(const std::string &str, char delimiter);

void removeWhitespace(std::string &str);

bool readInputFile(const std::string &file_to_open, std::vector<std::string> &variables, std::string &initial_var, std::vector<std::string> &terminals, std::map<std::string, std::vector<std::string>> &productions);

bool validEntries(const std::vector<std::string> &variables, const std::string &initial_var, const std::vector<std::string> &terminals, const std::map<std::string, std::vector<std::string>> &productions);

#endif // CFG_FILE_H