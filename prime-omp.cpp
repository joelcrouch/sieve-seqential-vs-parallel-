//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A prime-finding program (Parallel OpenMP version).
//
// Usage: 
//   linux> ./prime-omp N P
//
#include "unistd.h"
#include <iostream>
#include <cmath> 
#include <chrono>
#include <omp.h>
using namespace std; 

int main(int argc, char **argv) {
  int N;
  int P;
  if (argc < 2) {
    cout << "Usage: ./prime N\n"; 
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
  cout << "prime (seq) over [2.." << N << "] starting ...\n";
  auto start = chrono::steady_clock::now();
  bool candidate[N+1];
//  omp_set_number_threads(2);
#pragma omp parallel for num_threads(P)
  for (int i = 2; i <= N; i++)
    candidate[i] = true;
    
  int sqrtN = sqrt(N);
  for (int i = 2; i <= sqrt(N); i++)
    if (candidate[i]) 
#pragma omp parallel for num_threads(P)
      for (int j = i+i; j <= N; j += i)
        candidate[j] = false;

  auto end = chrono::steady_clock::now();
  auto duration = chrono::duration<double, std::milli> (end-start);
  int totalPrimes = 0;

#pragma omp parallel for num_threads(P) reduction(+:totalPrimes)
  for (int i = 2; i <= N; i++)
    if (candidate[i]) 
      totalPrimes++;

  cout << "prime (seq) found " << totalPrimes <<"  primes in "<<duration.count()<<" ms.\n";
}
