#include <iostream>
#include <algorithm>
#include <random>
#include "problem.h" 
#include <unordered_set>
#include <set>

using namespace std;

// #define TRAP_K 5
// #define TRAP_high 5
// #define TRAP_low 4

// #define TRAP_K 4
// #define TRAP_high 4
// #define TRAP_low 3


#define TRAP_K 5
#define TRAP_high 5
#define TRAP_low 4

// #define TRAP_K 5
// #define TRAP_high 1
// #define TRAP_low 0.9

// #define TRAP_K 5
// #define TRAP_high 1
// #define TRAP_low 0.8

static inline double folded_trap6(int u) {
    // constexpr int K = 6;
    // 夾在 [0, K] 範圍內以避免越界
    if (u == 0){
        return 1.0;
    }else if (u == 1){
        return 0.0;
    }else if (u == 2){
        return 0.4;
    }else if (u == 3){
        return 0.8;
    }else if (u == 4){
        return 0.4;
    }else if (u == 5){
        return 0.0;
    }else if (u == 6){
        return 1.0;
    }
   
    return -1000000;
}

// static inline double trap6(int u) {
//     constexpr int K = 6;
//     if (u < 0) u = 0;
//     if (u > K) u = K;
//     return ;
// }

// static inline double trap_linear_deceptive(int u, int K, double high, double low) {
//     if (u == K) return high;
//     // 防止除以 0；K 至少要 2（K=6 會 OK）
//     const double denom = static_cast<double>(K - 1);
//     return low + (high - low) * ((static_cast<double>(K - 1 - u)) / denom);
// }

double calculate_segment_onemax_weak(const string& segment, const string& method) {
        double weak_fiteness = 0;
        if (count(segment.begin(), segment.end(), '1') == 0)
            weak_fiteness = 1.5;
        else
            weak_fiteness = count(segment.begin(), segment.end(), '1');

        return weak_fiteness;
}

double calculate_segment_fitness(const string& segment, const string& method) {
    double result = 0.0;
    // if (method == "trap") {
    //     int ones = count(segment.begin(), segment.end(), '1');
    //     // cout << "segment.length(): " << segment.length() << endl;
    //     if (ones == segment.length()) {
    //         return TRAP_high;
    //     } else if (ones == 0) {
    //         return TRAP_low;
    //     } else {
    //         result = ((TRAP_K-1) - ones) * (TRAP_low / ((TRAP_K-1) - 1));


    //         return result;
    //     }
    if (method == "trap") {
        const int k = static_cast<int>(segment.length());  // 或用 TRAP_K，兩者請一致
        const int u = std::count(segment.begin(), segment.end(), '1');

        if (u == k) {
            return TRAP_high;
        } else {
            // f(u) = TRAP_low - u * TRAP_low / (k-1)
            // 等價於 f(u) = (k-1 - u) * (TRAP_low / (k-1))
            const double slope = static_cast<double>(TRAP_low) / (k - 1); // 避免整數除法
            return TRAP_low - u * slope;
            // 或：return (k - 1 - u) * slope;
        }
    }else if (method == "zerotrap") {
    const int k = static_cast<int>(segment.length());  // 或用 TRAP_K，兩者請一致
    const int z = std::count(segment.begin(), segment.end(), '0');

    if (z == k) {
        return TRAP_high;            // 全 0 → 高峰
    } else {
        // 線性遞減段：f(z) = TRAP_low - z * TRAP_low / (k-1),  z = 0..k-1
        const double slope = static_cast<double>(TRAP_low) / (k - 1);
        return TRAP_low - z * slope;
    }
    }else if (method == "ftrap") {
    int u = std::count(segment.begin(), segment.end(), '1');
    if ((int)segment.size() != 6) {
        cerr << "Error: ftrap expects segment length = 6" << endl;
        exit(1);
    }
    return folded_trap6(u);
    }

    else if (method == "niah") {
        return all_of(segment.begin(), segment.end(), [](char bit) { return bit == '1'; }) ? 1.0 : 0.0;
    }
    return 0.0;
}


double calculate_onemax_weak(const string& segment) {
    double fiteness = 0;
    if (count(segment.begin(), segment.end(), '1') == 0)
        fiteness = 1.5;
    else
        fiteness = count(segment.begin(), segment.end(), '1');

    return fiteness;
}

double calculate_fitness(const string& chromosome, const string& method) {
    if (method == "onemax") {
        return count(chromosome.begin(), chromosome.end(), '1');
    } else if (method == "trap") {
        return calculate_segment_fitness(chromosome, "trap");
    } else if (method == "niah") {
        return calculate_segment_fitness(chromosome, "niah");
    } else if (method == "ctrap" || method == "cniah") {

        if (chromosome.length() % TRAP_K != 0) {
            cerr << "Error: Chromosome length must be a multiple of TRAP_K for " << method << endl;
            exit(1);
        }
        // Calculate fitness in segments of TRAP_K
        size_t num_segments = chromosome.length() / TRAP_K;
        double total_fitness = 0.0;
        for (size_t i = 0; i < num_segments; ++i) {
            string segment = chromosome.substr(i * TRAP_K, TRAP_K);
            total_fitness += calculate_segment_fitness(segment, method.substr(1));
        }
        return total_fitness;
    } else if (method == "cyctrap") {
        if (chromosome.length() % (TRAP_K-1) != 0) {
            cerr << "Error: Chromosome length must be a multiple of TRAP_K-1 for cyctrap" << endl;
            exit(1);
        }

        if (chromosome.length() <= TRAP_K)
        {
            cerr << "Error: Chromosome length must be greater than TRAP_K for cyctrap" << endl;
            exit(1);
        }
        
      
        
        int segment_length = TRAP_K;
        int overlap = 1;
        double total_fitness = 0.0;
        for (size_t i = 0; i < chromosome.length(); i += segment_length - overlap) {
            string segment;
            if (i + segment_length <= chromosome.length()) {
                segment = chromosome.substr(i, segment_length);
            } else {
                segment = chromosome.substr(i) + chromosome.substr(0, segment_length - (chromosome.length() - i));
            }

            total_fitness += calculate_segment_fitness(segment, "trap");
        }
        return total_fitness;

    } else if (method == "1-0_cyctrap") {
            

        static bool printed = false;  
        if (!printed) {
            if (chromosome.length() % (TRAP_K-1) != 0) {
                cerr << "Error: Chromosome length must be a multiple of TRAP_K-1 for cyctrap" << endl;
                exit(1);
            }

            cout << "k = "<< TRAP_K << endl;
            cout << "m = "<< chromosome.length() / (TRAP_K-1) << endl;

            printed = true; // 之後就不會再印
        }



        string& chromosome_deep_copy = const_cast<string&>(chromosome);
        // chromosome_deep_copy[0] = '1';

        int segment_length = TRAP_K;
        int overlap = 1;
        double total_fitness = 0.0;
        int time = 0;
        for (size_t i = 0; i < chromosome_deep_copy.length(); i += segment_length - overlap) {
            string segment;
            if (i + segment_length <= chromosome_deep_copy.length()) {
                segment = chromosome_deep_copy.substr(i, segment_length);
            } else {
                segment = chromosome_deep_copy.substr(i) + chromosome_deep_copy.substr(0, segment_length - (chromosome_deep_copy.length() - i));
            }

            if (time % 2 == 0) {
                total_fitness += calculate_segment_fitness(segment, "trap");
            }else if(time % 2 == 1)  {
                total_fitness += calculate_segment_fitness(segment, "zerotrap");
            }
            time++;

            if (i + segment_length >= chromosome_deep_copy.length() + overlap) {
                break;
            }
        }
        return total_fitness;

    }else if (method == "1-0_cyctrap_half") {
        int div;
        static bool printed = false;  
        if (!printed) {
            if (chromosome.length() % (TRAP_K-1) != 0) {
                cerr << "Error: Chromosome length must be a multiple of TRAP_K-1 for cyctrap" << endl;
                exit(1);
            }

            cout << "k = "<< TRAP_K << endl;
            cout << "m = "<< chromosome.length() / (TRAP_K-1) << endl;
            div = 2; // 只允許一半以上的 '1'

            cout << "div = "<< div << endl;

            printed = true; // 之後就不會再印
        }



        string& chromosome_deep_copy = const_cast<string&>(chromosome);
        // chromosome_deep_copy[0] = '1';
        // 計算 '1' 的數量
        // A
        // int one_count = count(chromosome_deep_copy.begin(), chromosome_deep_copy.end(), '1');
        // if (one_count > chromosome_deep_copy.length() / div) {
        //     // cout << "chromosome_deep_copy.length() / 2 = " << chromosome_deep_copy.length() / 2 << endl;   
        //     return -1;
        // }

        // B
        int zero_count = count(chromosome_deep_copy.begin(), chromosome_deep_copy.end(), '0');
        if (zero_count > chromosome_deep_copy.length() / div) {
            // cout << "chromosome_deep_copy.length() / 2 = " << chromosome_deep_copy.length() / 2 << endl;   
            return -1;
        }


        int segment_length = TRAP_K;
        int overlap = 1;
        double total_fitness = 0.0;
        int time = 0;
        for (size_t i = 0; i < chromosome_deep_copy.length(); i += segment_length - overlap) {
            string segment;
            if (i + segment_length <= chromosome_deep_copy.length()) {
                segment = chromosome_deep_copy.substr(i, segment_length);
            } else {
                segment = chromosome_deep_copy.substr(i) + chromosome_deep_copy.substr(0, segment_length - (chromosome_deep_copy.length() - i));
            }

            if (time % 2 == 0) {
                total_fitness += calculate_segment_fitness(segment, "trap");
            }else if(time % 2 == 1)  {
                total_fitness += calculate_segment_fitness(segment, "zerotrap");
            }
            time++;

            if (i + segment_length >= chromosome_deep_copy.length() + overlap) {
                break;
            }
        }
        return total_fitness;
        
    }else if (method == "leadingones") {
        double leading_ones = 0;
        for (char bit : chromosome) {
            if (bit == '1') {
                leading_ones++;
            } else {
                break;
            }
        }
        return leading_ones;
    } else if (method == "leadingtraps") {
        int segment_length = 4;

        std::vector<int> L(chromosome.length(), 0); 
        L[0] = 1;
        std::vector<double> segment_fitness_record(chromosome.length(), 0); 
        double segment_fitness = 0.0;
        for (size_t i = 0; i < chromosome.length(); i += segment_length) {
            string segment = chromosome.substr(i, min(segment_length, static_cast<int>(chromosome.length() - i)));
            segment_fitness = calculate_segment_fitness(segment, "trap");
            segment_fitness_record[i] += segment_fitness;

            if (i == 0){
                continue;
            }
           
            if (segment_fitness_record[i-4] == 4 && L[i-4] == 1) {
                L[i] = 1;
            }
            
        }

        double total_fitness = 0.0;
        for (size_t i = 0; i < chromosome.length(); i += segment_length) {
            total_fitness += L[i] * segment_fitness_record[i]; 
        }

        return total_fitness;
    
    } else if (method == "weak_test") {
        double fiteness = 0;
        if (chromosome == "111")
            fiteness = 3;
        else if (chromosome == "100")
            fiteness = 2;
        else if (chromosome == "001")
            fiteness = 1;
        else
            fiteness = 0;
        return fiteness;
    } else if (method == "onemax_weak") {
        return calculate_onemax_weak(chromosome);
    }else if (method == "maxcut_2") {
    int n = chromosome.size();
    if (n < 2) {
        cerr << "Error: maxcut_random requires chromosome size >= 2\n";
        exit(1);
    }

    // 🔑 設定亂數種子（可重現），並印出 seed（只印一次）
    // static unsigned seed = std::random_device{}();
    // static std::mt19937 rng(seed);
    static const unsigned int seed = 657013513;
    static std::mt19937 rng(seed);

    // 若想完全固定重現，改用：
    static bool initialized = false;
    if (!initialized) {
        cout << "=== Planted-Bipartite Max-Cut (unique optimum up to flip) ===" << endl;
        cout << "Seed = " << seed << endl;
        initialized = true;
    }

    // 只生成一次的「二分分割」與「邊集合」
    static std::vector<int> part;                       // part[i] = 0/1 表示 i 所在群
    static std::vector<std::pair<int,int>> edges;       // 無向邊 (u,v), u<v
    if (edges.empty()) {
        // 隨機決定一個分割（大小盡量各半），確保最佳解唯一（互為全翻面）
        part.resize(n);
        std::vector<int> ids(n);
        std::iota(ids.begin(), ids.end(), 0);
        std::shuffle(ids.begin(), ids.end(), rng);

        int a = n / 2;          // A 的大小
        int b = n - a;          // B 的大小
        for (int i = 0; i < n; ++i) {
            if (i < a) part[ids[i]] = 0;  // 前 a 個進 A
            else        part[ids[i]] = 1; // 其餘進 B
        }

        // 建完整二分圖：只連 A-B 之間的邊
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                if (part[u] != part[v]) edges.emplace_back(u, v);
            }
        }

        // 印出分割與邊（可視需要保留/註解）
        int cntA = 0, cntB = 0;
        for (int i = 0; i < n; ++i) (part[i] == 0 ? cntA : cntB)++;
        cout << "Partition sizes: |A|=" << cntA << ", |B|=" << cntB << endl;

        cout << "A: { ";
        for (int i = 0; i < n; ++i) if (part[i] == 0) cout << "x" << i << " ";
        cout << "}\nB: { ";
        for (int i = 0; i < n; ++i) if (part[i] == 1) cout << "x" << i << " ";
        cout << "}\n";

        cout << "Edges (" << edges.size() << "):" << endl;
        for (auto &e : edges) cout << "(x" << e.first << ", x" << e.second << ")\n";
        cout << endl;
        // 性質：只有 (A|B) 與 (B|A) 兩個最佳解；其他任何移動都會減少 cut 值
    }

    // 計算 cut 值：端點在不同側即計入
    int cut = 0;
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        bool su = (chromosome[u] == '1');
        bool sv = (chromosome[v] == '1');
        if (su != sv) ++cut;
    }
    return static_cast<double>(cut);
}else if (method == "maxcut_2_half") {
    int n = chromosome.size();
    if (n < 2) {
        cerr << "Error: maxcut_2_half requires chromosome size >= 2\n";
        exit(1);
    }

    // === Hamming 距離門檻 === A
    // {
    //     static const std::string base = "10101100";
    //     int d = 0;
    //     for (int i = 0; i < n; ++i) {
    //         char target = base[i % base.size()];
    //         if (chromosome[i] != target) ++d;
    //     }
    //     if (d > n / 2) return -10000;   // 距離太大直接淘汰
    // }

    // === Hamming 距離門檻 === B
    {
        static const std::string base = "01010011";
        int d = 0;
        for (int i = 0; i < n; ++i) {
            char target = base[i % base.size()];
            if (chromosome[i] != target) ++d;
        }
        if (d > n / 2) return -10000;   // 距離太大直接淘汰
    }

    // === 固定亂數種子（可重現） ===
    static const unsigned int seed = 657013513;
    static std::mt19937 rng(seed);

    static bool initialized = false;
    if (!initialized) {
        cout << "=== Planted-Bipartite Weighted Max-Cut (two optima) ===" << endl;
        cout << "Seed = " << seed << endl;
        initialized = true;
    }

    // === 狀態 ===
    static std::vector<int> part;                         // 0/1 表示所屬群
    static std::vector<std::pair<int,int>> edges;         // 無向邊 (u,v)
    static std::vector<int> weights;                      // 對應邊的權重
    if (edges.empty()) {
        // 隨機決定分割（盡量各半）
        part.resize(n);
        std::vector<int> ids(n);
        std::iota(ids.begin(), ids.end(), 0);
        std::shuffle(ids.begin(), ids.end(), rng);
        int a = n / 2;
        int b = n - a;
        for (int i = 0; i < n; ++i)
            part[ids[i]] = (i < a ? 0 : 1);

        // 建完整二分圖並給權重 −10~10
        std::uniform_int_distribution<int> wdist(-10, 10);
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                if (part[u] != part[v]) {
                    edges.emplace_back(u, v);
                    weights.push_back(wdist(rng));
                }
            }
        }

        // === 列印分割與邊 ===
        int cntA = 0, cntB = 0;
        for (int x : part) (x == 0 ? cntA : cntB)++;
        cout << "Partition sizes: |A|=" << cntA << ", |B|=" << cntB << endl;

        cout << "A: { ";
        for (int i = 0; i < n; ++i) if (part[i] == 0) cout << "x" << i << " ";
        cout << "}\nB: { ";
        for (int i = 0; i < n; ++i) if (part[i] == 1) cout << "x" << i << " ";
        cout << "}\n";

        cout << "Edges (" << edges.size() << "):" << endl;
        for (size_t i = 0; i < edges.size(); ++i) {
            auto &e = edges[i];
            cout << "(x" << e.first << ", x" << e.second << ")  w=" << weights[i] << "\n";
        }
        cout << endl;
    }

    // === 計算 cut 值（跨側邊權重總和） ===
    int cut = 0;
    for (size_t i = 0; i < edges.size(); ++i) {
        int u = edges[i].first, v = edges[i].second;
        bool su = (chromosome[u] == '1');
        bool sv = (chromosome[v] == '1');
        if (su != sv) cut += weights[i];
    }

    return static_cast<double>(cut);

}else if (method == "max3sat_unit_and_random") {
        int n = chromosome.size();
        if (n < 3) {
            cerr << "Error: max3sat_random requires chromosome size >= 3\n";
            exit(1);
        }

        int num_clauses = 15;  // 子句數，可依需求調整

        // // 🔑 設定亂數種子，讓實驗可重現
        // static unsigned seed = std::random_device{}();
        // static std::mt19937 rng(seed);
        // // 若要固定種子以完全重現，改用下列兩行：
        // static const unsigned int seed = 657013513;
        // static std::mt19937 rng(seed);

        // 印出 seed，只印一次
        // static bool initialized = false;
        // if (!initialized) {
        //     cout << "=== Random 3SAT ===" << endl;
        //     cout << "Seed = " << seed << endl;
        //     initialized = true;
        // }

        // std::uniform_int_distribution<int> var_dist(0, n-1);
        // std::uniform_int_distribution<int> sign_dist(0, 1);

        // // 隨機生成子句（確保同一個 clause 中變數索引不重複）
        // static std::vector<std::array<std::pair<int,bool>,3>> clauses;
        // if (clauses.empty()) {  // 只生成一次
        //     clauses.reserve(num_clauses);
        //     for (int c = 0; c < num_clauses; ++c) {
        //         std::array<std::pair<int,bool>,3> clause;

        //         // 先抽 3 個不同的變數索引
        //         std::unordered_set<int> chosen;
        //         while (static_cast<int>(chosen.size()) < 3) {
        //             int var = var_dist(rng);
        //             chosen.insert(var); // set 會自動去重
        //         }

        //         // 將 set 內容放入 clause，並各自隨機正負號
        //         int j = 0;
        //         for (int var : chosen) {
        //             bool sign = static_cast<bool>(sign_dist(rng)); // true=正, false=否定
        //             clause[j++] = {var, sign};
        //         }

        //         // （可選）打亂子句內文字順序，避免固定順序造成偏差
        //         std::shuffle(clause.begin(), clause.end(), rng);

        //         clauses.push_back(clause);
        //     }

            // clause 支援可變長度
// static std::vector<std::vector<std::pair<int,bool>>> clauses = {
//     { {0,true},  {1,true},  {7,false} },
//     { {0,false}, {1,false}, {4,true}  },
//     { {1,true},  {2,true},  {5,false} },
//     { {1,false}, {2,false}, {4,false} },
//     { {2,true},  {3,true},  {0,false} },
//     { {2,false}, {3,false}, {6,true}  },
//     { {3,true},  {4,true},  {6,true}  },
//     { {3,false}, {4,false}, {7,true}  },
//     { {4,true},  {5,true},  {7,true}  },
//     { {4,false}, {5,false}, {0,false} },
//     { {5,true},  {6,true},  {0,false} },
//     { {5,false}, {6,false}, {1,true}  },
//     { {6,true},  {7,true},  {1,false} },
//     { {6,false}, {7,false}, {5,true}  },
//     { {0,true},  {0,true},  {0,true}  } // 最後的單文字 clause
// };

static std::vector<std::vector<std::pair<int,bool>>> clauses = {
    // { {0,true},  {2,false}, {3,true} },
    // { {1,true}, {2,true}, {1,true} }, // 2 
    // { {1,false}, {2,false}, {2,false} }, // 2 
    // { {2,true},  {2,true},  {2,true} }, // 2

    // { {3,true},  {3,true},  {3,true} }, // 3  
    // { {3,false}, {4,true},  {6,false} }, // 3   
    // { {2,false}, {3,false}, {4,false} }, // 3     

    // { {6,true},  {7,false}, {5,false} }, // 7
    // { {7,true},  {7,true},  {7,true} }, // 7

    // // { {0,true},  {2,true},  {2,true} }, // 2
    // // { {5,true},  {6,false},  {6,false} },
    // { {4,true},  {5,false},  {5,false} },
    
    //  { {3,true}, {4,false}, {4,false} }, // 3   
    //   { {2,true}, {3,false}, {3,false} }, // 3  


    // { {4,true},  {2,false}, {3,true} },
    // { {1,true}, {2,true}, {4,true} }, // 2 
    // { {1,false}, {2,false}, {2,false} }, // 2 
    // { {2,true},  {2,true},  {2,true} }, // 2
    // { {3,true},  {3,true},  {4,true} }, // 3  
    // { {3,false}, {0,true},  {6,false} }, // 3   
    // { {2,false}, {3,false}, {0,false} }, // 3     
    // { {6,true},  {7,false}, {5,false} }, // 7
    // { {7,true},  {7,true},  {7,true} }, // 7
    // { {0,true},  {5,false},  {5,false} },
    // { {3,true}, {0,false}, {0,false} }, // 3   
    // { {2,true}, {3,false}, {4,false} }, // 3 
    //  { {3,true}, {4,false}, {4,false} }, // 3   


    // { {4,true},  {2,false}, {2,false}},
    // { {1,true}, {2,true}, {2,true} }, // 2 
    // { {1,false}, {2,false}, {2,false} }, // 2 
    // { {2,true},  {2,true},  {2,true} }, // 2
    // { {3,true},  {3,true},  {3,true} }, // 3  
    // { {3,false}, {0,true},  {6,false} }, // 3   
    // { {2,false}, {3,false}, {0,false} }, // 3     
    // { {6,true},  {7,false}, {5,false} }, // 7
    // { {7,true},  {7,true},  {7,true} }, // 7
    // { {0,true},  {5,false},  {5,false} },
    // { {3,true}, {0,false}, {0,false} }, // 3   
    // { {2,true}, {3,false}, {3,false} }, // 3 


    // { {4,true},  {2,false}, {2,false}}, // 重複 1

    // { {1,true}, {2,true}, {2,true} }, // 重複 1

    // { {1,false}, {2,false}, {2,false} }, // 重複 1

    // { {2,true},  {2,true},  {2,true} }, // 重複 2

    // { {3,true},  {3,true},  {3,true} }, // 重複 2 

    // { {3,false}, {0,true},  {6,false} }, // ok  

    // { {2,false}, {3,false}, {0,false} }, // ok    

    // { {6,true},  {7,false}, {5,false} }, // ok 

    // { {7,true},  {7,true},  {7,true} }, // 重複 2
 
    // { {0,true},  {5,false},  {5,false} }, // 重複 1

    // { {3,true}, {0,false}, {0,false} }, // 重複 1   

    // { {2,true}, {3,false}, {3,false} }, // 重複 1

    ////// final
  
    // { {0,false}, {2,false}, {3,false} },      // ok（原 {2,false},{3,false},{0,false}）
    // { {0,false}, {0,false}, {3,true}  },      // 重複 1（原 {3,true},{0,false},{0,false}）
    // { {0,true},  {3,false}, {6,false} },      // ok（原 {3,false},{0,true},{6,false}）
    // { {0,true},  {5,false}, {5,false} },      // 重複 1

    // { {1,false}, {2,false}, {2,false} },      // 重複 1
    // { {1,true},  {2,true},  {7,false} },      // 重複 1

    // { {2,true},  {2,true},  {2,true} },       // 重複 2
    // { {2,true},  {3,false}, {3,false} },      // 重複 1

    // { {3,true},  {3,true},  {3,true} },       // 重複 2

    // { {4,false}, {4,false}, {4,false} },      // 重複 1

    // { {5,false}, {6,true},  {7,false} },      // ok（原 {6,true},{7,false},{5,false}）

    // { {7,true},  {7,true},  {7,true} }        // 重複 2

    /////////

    



{ {0,true},  {2,true},  {3,true}  },      // ok（原 {2,true}, {3,true}, {0,true}）
{ {0,true},  {0,true},  {3,false} },      // 重複 1（原 {3,false},{0,true},{0,true}）
{ {0,false}, {3,true},  {6,true}  },      // ok（原 {3,true}, {0,false}, {6,true}）
{ {0,false}, {5,true},  {5,true}  },      // 重複 1

{ {1,true},  {2,true},  {2,true}  },      // 重複 1
{ {1,false}, {2,false}, {7,true}  },      // 重複 1

{ {2,false}, {2,false}, {2,false} },      // 重複 2
{ {2,false}, {3,true},  {3,true}  },      // 重複 1

{ {3,false}, {3,false}, {3,false} },      // 重複 2

{ {4,true},  {4,true},  {4,true}  },      // 重複 1

{ {5,true},  {6,false}, {7,true}  },      // ok（原 {6,false},{7,true},{5,true}）

{ {7,false}, {7,false}, {7,false} }       // 重複 2




    ////// ok
    // { {1,true}, {2,true}, {7,false} }, // 重複 1

    // { {1,false}, {2,false}, {2,false}}, // 重複 1

    // { {2,true},  {2,true},  {2,true} }, // 重複 2

    // { {7,true},  {7,true},  {7,true} }, // 重複 2
 
    // { {4,false},  {4,false}, {4,false}}, // 重複 1

    // { {3,true},  {3,true},  {3,true} }, // 重複 2 


    // { {3,false}, {0,true},  {6,false} }, // ok  

    // { {2,false}, {3,false}, {0,false} }, // ok    

    // { {6,true},  {7,false}, {5,false} }, // ok 

    // { {0,true},  {5,false},  {5,false} }, // 重複 1

    // { {3,true}, {0,false}, {0,false} }, // 重複 1   

    // { {2,true}, {3,false}, {3,false} }, // 重複 1


    //////
    // { {0,true},  {5,false},  {2,false} }, // ok 

    // { {3,true}, {0,false}, {7,false} }, // ok 

    // { {2,true}, {3,false}, {7,false} }, // ok 

    // { {3,false}, {0,true},  {6,false} }, // ok  

    // { {2,false}, {3,false}, {0,false} }, // ok    

    // { {6,true},  {7,false}, {5,false} }, // ok 

    // { {4,true},  {2,false}, {7,false}}, // ok 

    // { {1,false}, {2,false}, {7,false} }, // ok

    // { {1,true}, {2,true},  {2,true}},  // 重複 2

    // { {2,true},  {2,false},  {2,true} }, // 重複 2

    // { {3,true},  {3,true},  {3,true} }, // 重複 2 

    // { {7,true},  {7,true},  {7,true} }, // 重複 2
 
};



            // 印出隨機生成的子句
            for (auto& cl : clauses) {
                cout << "(";
                for (int i = 0; i < cl.size(); i++) {
                    int idx = cl[i].first;
                    bool pos = cl[i].second;
                    if (!pos) cout << "¬";
                    cout << "x" << idx;
                    if (i < 2) cout << " ∨ ";
                }
                cout << ")" << endl;
            }
            cout << endl;
        // }

        // 計算滿足的子句數
        int sat = 0;
        for (auto& cl : clauses) {
            bool clause_sat = false;
            for (auto& lit : cl) {
                bool val = (chromosome[lit.first] == '1');
                if ((lit.second && val) || (!lit.second && !val)) {
                    clause_sat = true;
                    break;
                }
            }
            if (clause_sat) ++sat;
        }

        return static_cast<double>(sat);
    }else if (method == "cycftrap") {
    const int FTRAP_K = 6;
    if (chromosome.length() % (FTRAP_K - 1) != 0) {
        cerr << "Error: Chromosome length must be a multiple of FTRAP_K-1 for cycftrap" << endl;
        exit(1);
    }
    if (chromosome.length() <= FTRAP_K) {
        cerr << "Error: Chromosome length must be greater than FTRAP_K for cycftrap" << endl;
        exit(1);
    }

    int segment_length = FTRAP_K;
    int overlap = 1;
    double total_fitness = 0.0;

    for (size_t i = 0; i < chromosome.length(); i += segment_length - overlap) {
        string segment;
        if (i + segment_length <= chromosome.length()) {
            segment = chromosome.substr(i, segment_length);
        } else {
            segment = chromosome.substr(i) +
                      chromosome.substr(0, segment_length - (chromosome.length() - i));
        }
        total_fitness += calculate_segment_fitness(segment, "ftrap");
    }
    return total_fitness;
    }else if (method == "cycftrap_half") {
    const int FTRAP_K = 6;
    static bool printed = false;
    int div = 2; // 只允許「一半以上」的 0（或 1）門檻，與你 cyctrap_half 一致

    if (!printed) {
        if (chromosome.length() % (FTRAP_K - 1) != 0) {
            cerr << "Error: Chromosome length must be a multiple of FTRAP_K-1 for cycftrap_half" << endl;
            exit(1);
        }
        cout << "k = " << FTRAP_K << endl;
        cout << "m = " << chromosome.length() / (FTRAP_K - 1) << endl;
        cout << "div = " << div << endl;
        printed = true;
    }

    string& chromosome_deep_copy = const_cast<string&>(chromosome);

    // 與你 B 版本一致：限制 0 的數量不得超過 length / div；超過就直接 -1
    int zero_count = count(chromosome_deep_copy.begin(), chromosome_deep_copy.end(), '0');
    if (zero_count > static_cast<int>(chromosome_deep_copy.length()) / div) {
        return -1;
    }

    int segment_length = FTRAP_K;
    int overlap = 1;
    double total_fitness = 0.0;

    for (size_t i = 0; i < chromosome_deep_copy.length(); i += segment_length - overlap) {
        string segment;
        if (i + segment_length <= chromosome_deep_copy.length()) {
            segment = chromosome_deep_copy.substr(i, segment_length);
        } else {
            segment = chromosome_deep_copy.substr(i) +
                      chromosome_deep_copy.substr(0, segment_length - (chromosome_deep_copy.length() - i));
        }
        total_fitness += calculate_segment_fitness(segment, "ftrap");
    }
    return total_fitness;
    }else if (method == "1-0_cycftrap") {
    const int FTRAP_K = 6;
    static bool printed = false;

    if (!printed) {
        if (chromosome.length() % (FTRAP_K - 1) != 0) {
            cerr << "Error: Chromosome length must be a multiple of FTRAP_K-1 for 1-0_cycftrap" << endl;
            exit(1);
        }
        cout << "k = " << FTRAP_K << endl;
        cout << "m = " << chromosome.length() / (FTRAP_K - 1) << endl;
        printed = true;
    }

    string& chromosome_deep_copy = const_cast<string&>(chromosome);

    int segment_length = FTRAP_K;
    int overlap = 1;
    double total_fitness = 0.0;
    int time = 0;

    for (size_t i = 0; i < chromosome_deep_copy.length(); i += segment_length - overlap) {
        string segment;
        if (i + segment_length <= chromosome_deep_copy.length()) {
            segment = chromosome_deep_copy.substr(i, segment_length);
        } else {
            segment = chromosome_deep_copy.substr(i) +
                      chromosome_deep_copy.substr(0, segment_length - (chromosome_deep_copy.length() - i));
        }

        if (time % 2 == 0) {
            total_fitness += calculate_segment_fitness(segment, "ftrap");
        } else {
            total_fitness += calculate_segment_fitness(segment, "zerotrap");
        }
        time++;

        if (i + segment_length >= chromosome_deep_copy.length() + overlap) {
            break;
        }
    }
    return total_fitness;
    }else if (method == "1-0_cycftrap_half") {
    const int FTRAP_K = 6;
    static bool printed = false;
    int div = 2; // 與 1-0_cyctrap_half 相同的門檻

    if (!printed) {
        if (chromosome.length() % (FTRAP_K - 1) != 0) {
            cerr << "Error: Chromosome length must be a multiple of FTRAP_K-1 for 1-0_cycftrap_half" << endl;
            exit(1);
        }
        cout << "k = " << FTRAP_K << endl;
        cout << "m = " << chromosome.length() / (FTRAP_K - 1) << endl;
        cout << "div = " << div << endl;
        printed = true;
    }

    string& chromosome_deep_copy = const_cast<string&>(chromosome);

    // 半數限制（沿用你先前的 B 版本）
    int zero_count = count(chromosome_deep_copy.begin(), chromosome_deep_copy.end(), '0');
    if (zero_count > static_cast<int>(chromosome_deep_copy.length()) / div) {
        return -1;
    }

    int segment_length = FTRAP_K;
    int overlap = 1;
    double total_fitness = 0.0;
    int time = 0;

    for (size_t i = 0; i < chromosome_deep_copy.length(); i += segment_length - overlap) {
        string segment;
        if (i + segment_length <= chromosome_deep_copy.length()) {
            segment = chromosome_deep_copy.substr(i, segment_length);
        } else {
            segment = chromosome_deep_copy.substr(i) +
                      chromosome_deep_copy.substr(0, segment_length - (chromosome_deep_copy.length() - i));
        }

        if (time % 2 == 0) {
            total_fitness += calculate_segment_fitness(segment, "ftrap");
        } else {
            total_fitness += calculate_segment_fitness(segment, "zerotrap");
        }
        time++;

        if (i + segment_length >= chromosome_deep_copy.length() + overlap) {
            break;
        }
    }
    return total_fitness;
    }else if (method == "ftrap") {

    // int u = std::count(segment.begin(), segment.end(), '1');

    string& chromosome_deep_copy = const_cast<string&>(chromosome);
    if ((int)chromosome_deep_copy.size() % 6 != 0) {
        cerr << "Error: Chromosome length must be a multiple of 6 for ftrap" << endl;
        exit(1);
    }

    size_t num_segments = chromosome.length() / 6;
    double total_fitness = 0.0;
    for (size_t i = 0; i < num_segments; ++i) {
        string segment = chromosome.substr(i * 6, 6);

        int u = std::count(segment.begin(), segment.end(), '1');
        total_fitness += folded_trap6(u);
    }
    return total_fitness;
    }




    



   
    std::cerr << "Error: the problem does not exist!" << std::endl;
    exit(1);
    return 0.0;
}


