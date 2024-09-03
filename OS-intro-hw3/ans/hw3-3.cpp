#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

mutex mtx;
int global_count = 0;

bool is_prime(int num) {
    if (num == 1) return false;

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void count_primes_local(int start, int end) {
    int local_count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) local_count++;
    }

    mtx.lock();
    global_count += local_count;
    mtx.unlock();
}

int main(int argc, char* argv[]) {
    int n, num_threads = 4; // Default number of threads

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

    cin >> n;

    vector<thread> threads;
    int start, end;
    int range = n / num_threads;

    for (int i = 0; i < num_threads; i++) {
        start = i * range + 1;
        end = (i == num_threads - 1) ? n : (i + 1) * range;
        threads.emplace_back(count_primes_local, start, end);
    }

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    cout << global_count << '\n';
    return 0;
}

