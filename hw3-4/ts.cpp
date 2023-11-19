#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>

using namespace std;

mutex mtx;
int lg;
int n, m;
vector<uint64_t> subsets;
vector<uint64_t> dp1, dp2;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;

// bool solve_local(unsigned int counter, int bit, uint64_t current) {
//     for (int j = 0 ; j < (1 << counter); j++){
        
//     }
// }

// void solve(int bit, uint64_t current) {
//     // bit means how many bits have been dealt with
//     int local_count = 0;
//     for (unsigned int i = (1 << bit); i < (1 << m); i++){
//         if (solve_local(i, bit, current)) local_count++;
//     }

//     mtx.lock();
//     global_count+=local_count;
//     mtx.unlock();
// }
void build_dp1(int start, int end){
    for(int i = start; i < end; i++){
        for (int j = 0; j < m; j++){
            if (i & (1 << j)) dp1[i] = subsets[j] | dp1[i ^ (1 << j)];
        }
    }
}
void build_dp2(int start, int end){
    for(int i = start; i < end; i++){
        for (int j = 0; j < m; j++){
            if (i & (1 << j)) dp2[i] = subsets[j] | dp2[i ^ (1 << j)];
        }
    }
}

int main(int argc, char* argv[]) {
    int num_threads = 4; // Default number of threads

    // dealing with "-t" argument
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "-t") {
                if (i + 1 < argc) {
                    num_threads = stoi(argv[i + 1]);
                    if(num_threads < 1 || num_threads > 8){
                        cerr << "Error: only 1~8 threads\n";
                        return 1;
                    }
                    else break;
                }
                else {
                    cerr << "Error: Specify the number of threads after -t" << endl;
                    return 1;
                }
            }
        }
    }

    vector<thread> threads;
    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    dp1.resize(1 << (m/2));
    dp2.resize(1 << (m-m/2));
    for (int i = 0; i < num_threads; i++){
        // threads.emplace_back(build_dp1, \
        //         i*(1 << (m/2))/num_threads, \
        //         (i+1)*(1 << (m/2))/num_threads);
		cout << i*(1 << (m/2 + 1))/num_threads << '\n';
        if (i < num_threads/2) {
            threads.emplace_back(build_dp1, \
                i*(1 << (m/2 + 1))/num_threads, \
                (i+1)*(1 << (m/2 + 1))/num_threads);
        }
        else {
            threads.emplace_back(build_dp2, \
                (i-num_threads/2)*(1 << (m - m/2 + 1))/num_threads, \
                (i+1-num_threads/2)*(1 << (m - m/2 + 1))/num_threads);
        }
    }
    for (int j = 0; j < num_threads; j++){
        threads[j].join();
    }

    for (int i = 0; i < (1 << (m/2)); i++){
        for (int j = 0; j < (1 << (m-m/2)); j++){
			cout << dp2[i] << '\n';
            if((dp1[i] | dp2[j]) == ((one << n) - 1)) 
                global_count++;
        }
    }

    // vector<uint64_t> subtasks;
    // subtasks.resize(num_threads);
    // lg = log2(num_threads);
    // threads.emplace_back(solve, lg, 0);
    // for (int j = 1; j < num_threads; j++){
    //     for (int k = 0; k < lg; k++){
    //         // get specific bit
    //         if( (j & ( 1 << k )) >> k ) subtasks[j] |= subsets[k];
    //     }
    //     //solve(lg,subtasks[j]);
    //     threads.emplace_back(solve, lg, subtasks[j]);
    // }
    //solve(lg,subtasks[0]);
    // for (int j = 0; j < num_threads; j++){
    //     threads[j].join();
    // }
    //solve(0, 0);

    cout << global_count << '\n';
    return 0;
}
