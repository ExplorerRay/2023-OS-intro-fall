#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <math.h>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;

void solve(int index, uint64_t current, uint64_t *local_cnt) {
    if (index == m) {
        if (current == (one << n) - 1) {
			//b[bit]=true;
			(*local_cnt)++;
        }
    } else {
        solve(index + 1, current, local_cnt );
        solve(index + 1, current | subsets[index], local_cnt );
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

    vector<uint64_t> subtasks;
    vector<uint64_t*> local_cnt;
    local_cnt.reserve(num_threads);
	for (int i = 0; i < num_threads; i++){
		local_cnt.emplace_back(new uint64_t(0));
	}
	subtasks.resize(num_threads);
    int lg = log2(num_threads);
    threads.emplace_back(solve, lg, 0, local_cnt[0]);
    for (int j = 1; j < num_threads; j++){
        for (int k = 0; k < lg; k++){
            // get specific bit
            if( j & ( 1 << k ) ) subtasks[j] |= subsets[k];
        }
        threads.emplace_back(solve, lg, subtasks[j], local_cnt[j]);
    }
    for (int j = 0; j < num_threads; j++){
        threads[j].join();
    }
    
    for(int j = 0; j < num_threads; j++){
        global_count += *(local_cnt[j]);
    }

    cout << global_count << '\n';
    return 0;
}
