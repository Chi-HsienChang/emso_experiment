#include <iostream>
#include "chromosome.h" 
#include "emso.h" 

using namespace std;

string vecToString(const vector<int>& v) {
    string s;
    s.reserve(v.size());
    for (int bit : v)
        s.push_back(bit ? '1' : '0');
    return s;
}


#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

// // 假設你已有：
// extern vector<vector<int>> emso_enumerations;
// extern vector<vector<int>> T_enumerations;
// extern vector<pair<string, double>> chromosomes;
// extern vector<long long> epi_count; // epi_count[emso_size] 要可累加
// extern int L;

// 把 {0,1,1} -> "011"
// static inline string vecToString(const vector<int>& v) {
//     string s; s.reserve(v.size());
//     for (int b : v) s.push_back(b ? '1' : '0');
//     return s;
// }

std::vector<int> emso(int L, int target_index, const vector<pair<string, double>>& chromosomes){
    cout << "Running EMSO detection..." << endl;
// void run_emso_count(int emso_size) {
    // 先把 chromosomes 變成 map，key: bitstring, value: fitness

     std::vector<std::vector<std::vector<int>>> epi_set(L);
    std::vector<int> epi_count(L, 0); 
    // std::vector<std::vector<int>> combinations;
    std::vector<std::vector<int>> emso_enumerations;
    std::vector<std::vector<int>> T_enumerations;   
    unordered_map<string, double> chromo_map;


    // std::vector<int> epi_count(L, 0); 
    chromo_map.reserve(chromosomes.size() * 2);
    for (const auto& kv : chromosomes) chromo_map.emplace(kv.first, kv.second);
    int emso_size = 1;
    int T_size = L - emso_size;

    // 產生列舉（如果你已經在外面生好，就直接用外面那份）
    emso_enumerations = generateBinarySequences(emso_size);
    T_enumerations    = generateBinarySequences(T_size);

    for (const auto& Tenum : T_enumerations) {
        const string T_str = vecToString(Tenum); // 避免重複轉字串

        for (const auto& emsoTest : emso_enumerations) {
            const string emsoTest_str   = vecToString(emsoTest);
            const string emso_key       = emsoTest_str + T_str;

            // 找 emsoTest_Tenum 的 fitness
            auto it_emso = chromo_map.find(emso_key);
            if (it_emso == chromo_map.end()) {
                // 若查不到，這個 emsoTest_Tenum 視為不成立，直接換下一個
                // 你也可以改成 continue; 這裡直接跳下一個 emsoTest
                continue;
            }
            const double emso_fit = it_emso->second;

            // 檢查是否對所有 others_Tenum 都滿足：emso_fit >= fitness(others_Tenum)
            bool dominates_all = true;
            for (const auto& others : emso_enumerations) {
                const string others_key = vecToString(others) + T_str;

                auto it_other = chromo_map.find(others_key);
                if (it_other == chromo_map.end()) {
                    // 查不到對手的 fitness -> 視為「不成立」或「跳過」
                    // 這裡選擇不成立以保守處理；若想「忽略缺值」，把這段改成 `continue;`
                    dominates_all = false;
                    break;
                }
                const double other_fit = it_other->second;

                if (emso_fit < other_fit) {
                    dominates_all = false;
                    break;
                }
            }

            if (dominates_all) {

                cout << "Found EMSO of size " << emso_size << ": " << emso_key << " with fitness " << emso_fit << endl; 
                ++epi_count[emso_size];
                // 若找到一個就要「換下一個 emsoTest_Tenum」，就不要在這裡繼續比較 others
                // 已經 break 了；如果還沒 break，這裡可以繼續，但無必要
            }
        }
    }

    return epi_count;
}


// std::vector<int> emso(int L, int target_index, const vector<pair<string, double>>& chromosomes)
// {
//     std::vector<std::vector<std::vector<int>>> epi_set(L);
//     std::vector<int> epi_count(L, 0); 
//     // std::vector<std::vector<int>> combinations;
//     std::vector<std::vector<int>> emso_enumerations;
//     std::vector<std::vector<int>> T_enumerations;
    
//     int T_size;
//     for (int emso_size = 1; emso_size < L; emso_size++)
//     {  

//         T_size = L - emso_size;
//         emso_enumerations = generateBinarySequences(emso_size); // enumerations = { [0, 0], [0, 1], [1, 0], [1, 1] }
//         T_enumerations = generateBinarySequences(T_size); // enumerations = { [0, 0], [0, 1], [1, 0], [1, 1] }



//         ///////////

//         // for ( auto& Tenum : T_enumerations ) 
//         // { 

//         //     for ( auto& emsoTest : emso_enumerations ) 
//         //     { 
//         //           for ( auto& others : emso_enumerations ) 
//         //           { 

//         //             string emsoTest_Tenum = vecToString(emsoTest) + vecToString(Tenum);
//         //             string others_Tenum = vecToString(others) + vecToString(Tenum);
//         //             //   emso_test+T_enum 變成字串 去 chromosomes 獲得對應的 fitness

//         //             auto it = find_if(chromosomes.begin(), chromosomes.end(),
//         //             [&](const pair<string, double>& p) {
//         //                 return p.first == emsoTest_Tenum;
//         //             });

//         //             if (it != chromosomes.end()) {
//         //                 cout << "Found: " << emsoTest_Tenum
//         //                     << " → fitness = " << it->second << endl;
//         //             } else {
//         //                 cout << "Not found: " << emsoTest_Tenum << endl;
//         //             }
                    

//         //             emsoTest_Tenum >= others_Tenum 是否對所有 others_Tenum 都成立
//         //             如果有一個不成立，就換下一個 emsoTest_Tenum
//         //             如果全部成立，就找到一個 emso，epi_count[emso_size]++ 
//         //           } 
//         //     }
//         // }

//         ///////////

//         // combinations = generateCombinations(L-1, epi_size, target_index); // combinations = { [1, 2], [1, 3], [2, 3] }

//         // for (auto& combination : combinations) // combination = [1, 2]
//         // { 
//         //     enumerations = generateBinarySequences(epi_size); // enumerations = { [0, 0], [0, 1], [1, 0], [1, 1] }    
//         //     epi_count[epi_size] += check_constrained_optima(target_index, combination, enumerations, chromosomes);         
//         // }
//     }
//     return epi_count;
// }
