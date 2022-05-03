//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A prime-finding program (Parallel version).
//
// Usage: 
//   linux> ./prime N P
//
#include "unistd.h"
#include <iostream>
#include <cmath> 
#include <chrono>
#include <vector>
#include <atomic>
#include <thread>
#include <condition_variable>

using namespace std; 

int main(int argc, char **argv) {
  int N;
  int P;
  if (argc < 2) {
    cout << "Usage: ./prime N P\n"; 
    exit(0);
  }
  if ((N = atoi(argv[1])) < 2) {
    cout << "N must be greater than 1\n"; 
    exit(0);
  }
  if (argc < 3) {
    cout << "Usage: ./prime N P\n, default will assign 1 to P"; 
    P=1;
  }
  if ((P = atoi(argv[2])) < 1) {
    cout << "P must be greater than 0\n"; 
    exit(0);
  }
  
  bool candidate[N+1];
  
  for(int i=0; i<=N; i++){
    candidate[i]=true;
  }
  vector<int>sieve;
  
  thread thd[P];
  for(int k=0; k<P; k++){
    thd[k] = thread(process, sieve, N, P, k);
  }
  
  for (int i = 2; i <= sqrt(N); i++){
    if (candidate[i]){
      unique_lock<mutex> lck(mtx);
      sieve.push_back(i);
      lck.unlock();
      cvar.notify_all();
      for (int j = i+i; j <= sqrtN; j += i){
        candidate[j] = false;
      }
    }
  }
////////////////do part 5!jjjj
  for(int k=0; k < P; k++){
    thd[k].join();
  }
  
  return 0;
} 
