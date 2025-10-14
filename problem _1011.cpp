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
    if (method == "trap") {
        int ones = count(segment.begin(), segment.end(), '1');
        // cout << "segment.length(): " << segment.length() << endl;
        if (ones == segment.length()) {
            return TRAP_high;
        } else if (ones == 0) {
            return TRAP_low;
        } else {
            result = ((TRAP_K-1) - ones) * (TRAP_low / ((TRAP_K-1) - 1));


            return result;
        }
    }else if (method == "zerotrap") {
        int zeros = count(segment.begin(), segment.end(), '0');
        // cout << "segment.length(): " << segment.length() << endl;
        if (zeros == segment.length()) {
            return TRAP_high;
        } else if (zeros == 0) {
            return TRAP_low;
        } else {
            result = ((TRAP_K-1) - zeros) * (TRAP_low / ((TRAP_K-1) - 1));


            return result;
        }
    } else if (method == "niah") {
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

        // 計算 '1' 的數量
        // int one_count = count(chromosome_deep_copy.begin(), chromosome_deep_copy.end(), '1');
        // if (one_count < chromosome_deep_copy.length() / 2) {
        //     // cout << "chromosome_deep_copy.length() / 2 = " << chromosome_deep_copy.length() / 2 << endl;   
        //     return 0;
        // }

        int one_count = count(chromosome_deep_copy.begin(), chromosome_deep_copy.end(), '1');
        if (one_count > chromosome_deep_copy.length() / 2) {
            // cout << "chromosome_deep_copy.length() / 2 = " << chromosome_deep_copy.length() / 2 << endl;   
            return 0;
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
        cerr << "Error: maxcut_planted2 requires chromosome size >= 2\n";
        exit(1);
    }

    // ===== 亂數種子：可重現；只印一次 =====
    // 想完全固定重現就用下面兩行，並註解掉上面兩行
    // static const unsigned int seed = 657013513;
    // static std::mt19937 rng(seed);

    static unsigned seed = std::random_device{}();
    static std::mt19937 rng(seed);

    static bool printed = false;
    if (!printed) {
        cout << "=== Planted-Bipartite Max-Cut (exactly 2 optima up to flip) ===\n";
        cout << "Seed = " << seed << "\n\n";
        printed = true;
    }

    // ===== 狀態：若 n 改變就重建圖 =====
    struct GraphState {
        int n_used = -1;
        std::vector<int> part;                   // part[i] ∈ {0,1}；植入的二分側
        std::vector<std::pair<int,int>> edges;   // 無向邊 (u<v)
    };
    static GraphState G;

    auto build_graph = [&](int n_now){
        G.n_used = n_now;
        G.part.assign(n_now, 0);
        G.edges.clear();

        // 隨機打散後等分：|A|=floor(n/2), |B|=ceil(n/2)
        std::vector<int> ids(n_now);
        std::iota(ids.begin(), ids.end(), 0);
        std::shuffle(ids.begin(), ids.end(), rng);

        int a = n_now / 2;
        int b = n_now - a;
        for (int i = 0; i < n_now; ++i) {
            G.part[ids[i]] = (i < a ? 0 : 1);
        }

        // 建 K_{a,b}：只加跨群邊
        for (int u = 0; u < n_now; ++u) {
            for (int v = u + 1; v < n_now; ++v) {
                if (G.part[u] != G.part[v]) G.edges.emplace_back(u, v);
            }
        }

        // （可保留這些列印，便於檢查）
        cout << "Partition sizes: |A|=" << a << ", |B|=" << b
             << "  => MaxCut = |A|*|B| = " << (a * b) << "\n";
        cout << "Edges = " << G.edges.size() << " (= |A|*|B|)\n";
        cout << "A: { ";
        for (int i = 0; i < n_now; ++i) if (G.part[i] == 0) cout << "x" << i << " ";
        cout << "}  B: { ";
        for (int i = 0; i < n_now; ++i) if (G.part[i] == 1) cout << "x" << i << " ";
        cout << "}\n\n";
    };

    if (G.n_used != n) build_graph(n);

    // ===== 評分：不同側（XOR）即計入 cut =====
    int cut = 0;
    for (auto &e : G.edges) {
        int u = e.first, v = e.second;
        bool su = (chromosome[u] == '1');
        bool sv = (chromosome[v] == '1');
        if (su != sv) ++cut;
    }

    return static_cast<double>(cut);
}else if (method == "maxcut_random") {
    int n = chromosome.size();
    if (n < 2) {
        cerr << "Error: maxcut_random requires chromosome size >= 2\n";
        exit(1);
    }

    int num_edges = 8;  // 邊數，可依需求調整

    // 🔑 設定亂數種子，讓實驗可重現（與你原本風格一致）
    static unsigned seed = std::random_device{}();
    static std::mt19937 rng(seed);
    // 若想固定種子以完全重現，改用下列兩行：
    // static const unsigned int seed = 657013513;
    // static std::mt19937 rng(seed);

    // 印出 seed，只印一次
    static bool initialized = false;
    if (!initialized) {
        cout << "=== Random Max-Cut ===" << endl;
        cout << "Seed = " << seed << endl;
        initialized = true;
    }

    // 生成隨機無向圖（無權重），只生成一次
    static std::vector<std::pair<int,int>> edges;
    if (edges.empty()) {
        // 確保 num_edges 不超過最大可能邊數
        long long max_possible = 1LL * n * (n - 1) / 2;
        if (max_possible <= 0) {
            cerr << "Error: not enough vertices for edges\n";
            exit(1);
        }
        if (num_edges > max_possible) num_edges = static_cast<int>(max_possible);

        std::uniform_int_distribution<int> vdist(0, n - 1);
        std::set<std::pair<int,int>> used; // 用來去重（以有序 pair 表示無向邊）

        while ((int)used.size() < num_edges) {
            int u = vdist(rng);
            int v = vdist(rng);
            if (u == v) continue; // 不要自環
            if (v < u) std::swap(u, v); // 規範化 (min,max)
            if (!used.count({u, v})) used.insert({u, v});
        }

        edges.assign(used.begin(), used.end());

        // 印出生成的邊
        cout << "Edges (" << edges.size() << "):" << endl;
        for (auto &e : edges) {
            cout << "(x" << e.first << ", x" << e.second << ")" << endl;
        }
        cout << endl;
    }

    // 計算 cut 值：端點落在不同側則計入
    int cut = 0;
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        bool su = (chromosome[u] == '1');
        bool sv = (chromosome[v] == '1');
        if (su != sv) ++cut;
    }

    return static_cast<double>(cut);
    }else if (method == "max3sat_random") {
        int n = chromosome.size();
        if (n < 2) {
            cerr << "Error: max3sat_random requires chromosome size >= 2\n";
            exit(1);
        }

        int num_clauses = 6;  // 子句數，可依需求調整

        // 🔑 設定亂數種子，讓實驗可重現
        static unsigned seed = std::random_device{}();
        static std::mt19937 rng(seed);
        // static const unsigned int seed = 657013513;
        // static std::mt19937 rng(seed);

        // 印出 seed，只印一次
        static bool initialized = false;
        if (!initialized) {
            cout << "=== Random 3SAT ===" << endl;
            cout << "Seed = " << seed << endl;
            initialized = true;
        }

        std::uniform_int_distribution<int> var_dist(0, n-1);
        std::uniform_int_distribution<int> sign_dist(0, 1);

        // 隨機生成子句
        static std::vector<std::array<std::pair<int,bool>,3>> clauses;
        if (clauses.empty()) {  // 只生成一次
            for (int c = 0; c < num_clauses; ++c) {
                std::array<std::pair<int,bool>,3> clause;
                for (int j = 0; j < 3; j++) {
                    int var = var_dist(rng);
                    bool sign = sign_dist(rng);
                    clause[j] = {var, sign};
                }
                clauses.push_back(clause);
            }

            // 印出隨機生成的子句
            for (auto& cl : clauses) {
                cout << "(";
                for (int i = 0; i < 3; i++) {
                    int idx = cl[i].first;
                    bool pos = cl[i].second;
                    if (!pos) cout << "¬";
                    cout << "x" << idx;
                    if (i < 2) cout << " ∨ ";
                }
                cout << ")" << endl;
            }
            cout << endl;
        }

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
    }

        
    // else if (method == "max3sat_unit_and_random") {
    //     int n = chromosome.size();
    //     if (n < 1) {
    //         cerr << "Error: max3sat_random_unique requires chromosome size >= 1\n";
    //         exit(1);
    //     }

    //     int extra_clauses = n;  // 額外隨機子句數，可自行調整

    //     static unsigned seed = std::random_device{}();
    //     static std::mt19937 rng(seed);

    //     static int built_n = -1;
    //     static std::string target;
    //     static std::vector<std::array<std::pair<int,bool>,3>> clauses;

    //     if (built_n != n) {
    //         built_n = n;
    //         clauses.clear();

    //         // === 生成唯一解 target ===
    //         target.resize(n);
    //         std::uniform_int_distribution<int> bit01(0, 1);
    //         for (int i = 0; i < n; ++i) target[i] = bit01(rng) ? '1' : '0';

    //         // === 強制 unit 子句 (唯一解) ===
    //         for (int i = 0; i < n; ++i) {
    //             bool pos = (target[i] == '1');
    //             clauses.push_back({ std::make_pair(i, pos),
    //                                 std::make_pair(i, pos),
    //                                 std::make_pair(i, pos) });
    //         }

    //         // === 加上隨機 3 子句 ===
    //         std::uniform_int_distribution<int> var_dist(0, n - 1);
    //         for (int c = 0; c < extra_clauses; ++c) {
    //             std::array<std::pair<int,bool>,3> cl;
    //             for (int j = 0; j < 3; ++j) {
    //                 int v = var_dist(rng);
    //                 bool sign = bit01(rng);
    //                 cl[j] = { v, sign };
    //             }
    //             clauses.push_back(cl);
    //         }

    //         // === 印出資訊 ===
    //         cout << "=== Random 3SAT with UNIQUE optimum ===\n";
    //         cout << "Seed = " << seed << "\n";
    //         cout << "n = " << n 
    //             << ", forced clauses = " << n 
    //             << ", extra random clauses = " << extra_clauses << "\n";
    //         cout << "Target = ";
    //         for (char ch : target) cout << ch;
    //         cout << "\n\n";

    //         cout << "=== Clauses (CNF) ===\n";
    //         for (auto& cl : clauses) {
    //             cout << "(";
    //             for (int i = 0; i < 3; ++i) {
    //                 int idx = cl[i].first;
    //                 bool pos = cl[i].second;
    //                 if (!pos) cout << "¬";
    //                 cout << "x" << idx;
    //                 if (i < 2) cout << " ∨ ";
    //             }
    //             cout << ")\n";
    //         }
    //         cout << endl;
    //     }

    //     // === 評估滿足子句數 ===
    //     int sat = 0;
    //     for (auto& cl : clauses) {
    //         bool clause_sat = false;
    //         for (auto& lit : cl) {
    //             bool val = (chromosome[lit.first] == '1');
    //             if ((lit.second && val) || (!lit.second && !val)) {
    //                 clause_sat = true;
    //                 break;
    //             }
    //         }
    //         if (clause_sat) ++sat;
    //     }
    //     return static_cast<double>(sat);
    // }

   
    std::cerr << "Error: the problem does not exist!" << std::endl;
    exit(1);
    return 0.0;
}


