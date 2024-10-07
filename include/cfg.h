#ifndef CFG_H
#define CFG_H

#include <vector>
#include <string>
#include <map>
#include <set>

void detailed_mode(const std::string &initial_var, const std::vector<std::string> &variables, const std::map<std::string, std::vector<std::string>> &productions);

bool isChainUsed(const std::string &chain, std::set<std::string> &used_chains);

void fast_mode(const std::string &initial_var, const std::vector<std::string> &variables, const std::map<std::string, std::vector<std::string>> &productions);

#endif // CFG_H