// #ifndef EMSO_H
// #define EMSO_H

#include <vector>
#include <string>
#include <set>
#include <utility>

using namespace std;

string vecToString(const vector<int>& v);

std::vector<int> emso(
    int L,
    int target_index,
    const std::vector<std::pair<std::string, double>>& chromosomes
);