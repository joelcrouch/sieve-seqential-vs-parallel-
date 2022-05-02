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
using namespace std; 

atomic<int> totalPrimes;
void process(vector<int>sieve, int N, int P, int tid){

}
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

  cout << "prime-par1 (parallel) over [2.." << N << "] starting ...\n";
  auto start = chrono::steady_clock::now();
  bool candidate[N+1];
  int sqrtN = sqrt(N);
  for (int i = 2; i <= sqrtN; i++)
    candidate[i] = true;
  vector<int>sieve;   
  for (int i = 2; i <= sqrt(N); i++){
    if (candidate[i]){
      sieve.push_back(i);
      for (int j = i+i; j <= sqrtN; j += i){
        candidate[j] = false;
      }
    }
  }


  auto end = chrono::steady_clock::now();
  auto duration = chrono::duration<double, std::milli> (end-start);
  int count = 0;
  for (int x: sieve){
      count++;
    }
  totalPrimes +=count;

  cout << "prime-par1 (parallel) found " << totalPrimes <<"  primes in "<<duration.count()<<" ms.\n";
}
