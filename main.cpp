#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <set>
#include <random>

#include "chromosome.h" 
#include "problem.h" 
#include "eg.h"  
#include "weak.h" 

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 7) {
        printf ("./main problem ell detect_eg detect_epi detect_weak show_fitness\n");
        printf ("problem: \n");
        printf ("    ONEMAX           :  0\n");
        printf ("    LEADINGONES      :  1\n");
        printf ("    CTRAP            :  2\n");
        printf ("    CYCTRAP          :  3\n");
        printf ("    CNIAH            :  4\n");
        printf ("    LEADINGONES      :  5\n");
        printf ("    LEADINGTRAPS     :  6\n");
        printf ("    ==========================\n");
        printf ("    FRTAP            :  7\n"); 
        printf ("    CYCFRTAP         :  8\n");
        printf ("    CYCFRTAP HALF    :  9\n");
        printf ("    ==========================\n");
        printf ("    1-0 CYCFRTAP     :  10\n");
        printf ("    1-0 CYCFRTAP HALF:  11\n");

        // printf ("     MAXCUT_RANDOM  :  11\n");
        // printf ("     MAXCUT_TWO     :  12\n");
        // printf ("     MAXCUT_TWO HALF:  13\n");
        // printf ("     1-0 CYCTRAP    :  14\n");
        // printf ("     1-0 CYCTRAP HALF:  15\n");
        // printf ("     MAX3SAT_TEST   :  16\n");
        // printf ("     MAX3SAT_RANDOM :  17\n");
        return 1;
    }

    int problem = stoi(argv[1]);
    int ell = stoi(argv[2]);
    int detect_eg = stoi(argv[3]);
    int detect_epi = stoi(argv[4]);
    int detect_weak = stoi(argv[5]);
    int show_fitness = stoi(argv[6]);

    int n = pow(2, ell);

    vector<pair<string, double>> all_chromosomes;
    switch (problem) {
        case 0: all_chromosomes = generate_chromosomes(ell, "onemax"); break;
        case 1: all_chromosomes = generate_chromosomes(ell, "leadingones"); break;
        case 2: all_chromosomes = generate_chromosomes(ell, "ctrap"); break;
        case 3: all_chromosomes = generate_chromosomes(ell, "cyctrap"); break;
        case 4: all_chromosomes = generate_chromosomes(ell, "cniah"); break;
        case 5: all_chromosomes = generate_chromosomes(ell, "leadingones"); break;
        case 6: all_chromosomes = generate_chromosomes(ell, "leadingtraps"); break;
        case 7: all_chromosomes = generate_chromosomes(ell, "ftrap"); break;
        case 8: all_chromosomes = generate_chromosomes(ell, "cycftrap"); break;
        case 9: all_chromosomes = generate_chromosomes(ell, "cycftrap_half"); break;
        case 10: all_chromosomes = generate_chromosomes(ell, "1-0_cycftrap"); break;
        case 11: all_chromosomes = generate_chromosomes(ell, "1-0_cycftrap_half"); break;

        // case 7: all_chromosomes = generate_chromosomes(ell, "max3sat_unit_and_random"); break;
        // case 8: all_chromosomes = generate_chromosomes(ell, "max3sat_random"); break;
        // case 9: all_chromosomes = generate_chromosomes(ell, "1-0_cyctrap"); break;
        // case 10: all_chromosomes = generate_chromosomes(ell, "1-0_cyctrap_half"); break;
        // case 11: all_chromosomes = generate_chromosomes(ell, "maxcut_random"); break;
        // case 12: all_chromosomes = generate_chromosomes(ell, "maxcut_2"); break;
        // case 13: all_chromosomes = generate_chromosomes(ell, "maxcut_2_half"); break;
        default:
            cerr << "Unknown problem: " << problem << endl;
            return 1;
    }



    auto chromosomes = sample_chromosomes(all_chromosomes, n);


        if (detect_eg) {
        cout << "====================" << endl;
        cout << "EG" << endl;
        cout << "====================" << endl;

        // Step 1: 建立 ell x ell 矩陣
        std::vector<std::vector<int>> epi_matrix(ell, std::vector<int>(ell, 0));

        for (int target_index = 0; target_index < ell; target_index++) {
            std::vector<int> epi_count_results = eg(ell, target_index, chromosomes);

            for (int i = 0; i < ell; i++) {
                epi_matrix[target_index][i] = epi_count_results[i];
            }
        }

        // Step 0: 讓對角線為 0
        for (int i = 0; i < ell; i++) {
            epi_matrix[i][i] = 0;
        }

        // // Step 1: 轉置前輸出
        // cout << "EG Matrix (before transpose):" << endl;
        // for (int i = 0; i < ell; i++) {
        //     for (int j = 0; j < ell; j++) {
        //         cout << epi_matrix[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        // Step 2: 轉置輸出
        int total_epi = 0;
        int one_epi = 0;
        int two_epi = 0;
        for (int i = 0; i < ell; i++) {
            for (int j = 0; j < ell; j++) {
                if (i != j) {
                    total_epi += epi_matrix[i][j];
                }

                if (epi_matrix[i][j] == 1) {
                    one_epi += epi_matrix[i][j];
                }

                if (epi_matrix[i][j] == 2) {
                    two_epi += epi_matrix[i][j];
                }
                cout << epi_matrix[j][i] << " ";
            }
            cout << endl;
        }
        cout << endl;

        cout << "Total loci pairs with 1-epistasis: " << one_epi << endl;
        cout << "Total loci pairs with 2-epistasis: " << two_epi/2 << endl;
        cout << "Total epistasis (excluding diagonal): " << total_epi << endl;
        cout << endl;
    }



    if (detect_epi) {
        cout << "====================" << endl;
        cout << "Epistasis counts" << endl;
        cout << "====================" << endl;

        for (int target_index = 0; target_index < ell; target_index++) {
            cout << "S -> " << target_index << endl;

            std::vector<int> epi_count_results = epistasis(ell, target_index, chromosomes);
            cout << "--- Epistasis ---" << endl;
            for (int i = 1; i < epi_count_results.size(); i++) {
                cout << "#order_" << i << ": " << epi_count_results[i] << endl;
            }
            cout << "-----------------" << endl;
            cout << endl;
        }
        cout << endl;
    }


    if (detect_weak) {
        cout << "====================" << endl;
        cout << "Weak counts" << endl;
        cout << "====================" << endl;

        bool isWeakProblem = false;
        for (int target_index = 0; target_index < ell; target_index++) {
            cout << "S -> " << target_index << endl;
            std::vector<int> weak_count_results = weak(ell, target_index, chromosomes);

            bool isWeak = false;
            for (int i = 2; i < weak_count_results.size(); i++) {
                if (weak_count_results[i] > 0) {
                    isWeak = true;
                    isWeakProblem = true;
                    break;
                }
            }

            if (!isWeak) {
                cout << "--- No weak ---" << endl;
                cout << endl;
            }else {
                cout << "--- Weak ---" << endl;
                for (int i = 2; i < weak_count_results.size(); i++) {
                    cout << "#order_" << i << ": " << weak_count_results[i] << endl;
                }
                cout << endl;
            }

        }

        if (isWeakProblem) {
            cout << "##### Exist weak #####" << endl;
        } else {
            cout << "##### No weak #####" << endl;
        }
        cout << endl;
    }

    if (show_fitness) {
        sort(chromosomes.begin(), chromosomes.end(), [](const auto& a, const auto& b) {
            return a.second > b.second; 
        });


        
        cout << "====================" << endl;
        cout << "chromosomes & fitness" << endl;
        cout << "====================" << endl;

        int show_count = 0; 
        for (const auto& chom : chromosomes) {
            cout << chom.first << " " << chom.second << endl;
            show_count++;
            if (show_count >= 10000) break;
        }
        cout << endl;

        if (chromosomes[0].second > chromosomes[1].second) {
            cout << "global optimal is unique" << endl;
        } else {
            cout << "global optimal is not unique" << endl;
        }
    }


    
    return 0;
}
