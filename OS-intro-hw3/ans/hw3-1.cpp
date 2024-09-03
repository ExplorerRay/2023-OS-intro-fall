#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

condition_variable cv;
int turn = 0;

// change name avoid ambiguous
void count_hw(int index) {
  static mutex io_mutex;
  unique_lock<mutex> lock(io_mutex);
  int num = 1000000;
  
  while (num--) {}
  {
    // use condition variable and turn to check which thread should output
    cv.wait(lock, [&] { return index == turn; });

    cout << "I'm thread " << index << ", local count: 1000000\n";
    turn+=1;
    
    cv.notify_all();
  }
  
}

int main(void) {
  thread t[100];
  
  for (int i = 0; i < 100; i++){
    t[i] = thread(count_hw, i);
  }

  for (int i = 0; i < 100; i++){
    t[i].join();
  }
  
}
