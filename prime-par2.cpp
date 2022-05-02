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
using namespace std; 

atomic<int> totalPrimes;
vector<int>sieve;
bool maindone=false;

void process(vector<int>sieve, int N, int P, int tid){
  int sqrtN=floor(sqrt(N));
  int range= ((N-sqrtN)/P);
  int offset = sqrtN + (range *tid);
  int end = offset+range;
  int count=0;
 // cout<<"\nsqrtN: "<<sqrtN<<" range: "<<range<<" offset: "<<offset<<" end: "<<end<<"  \n"; 
  bool candidate[N+1];
  for(int i= offset; i<=end; i++){
    candidate[i]=true;
  }
  for(int x: sieve){
    for(int i=offset; i<=end;i++){
      if(i%x==0){
	candidate[i]=false;
	for(int j= i+i; j<=end; j+=i){
	  candidate[j]=false;
	}
      }
    }
  }
  for(int i=offset; i<=end; i++){
    if(candidate[i]){
      count++;
    }
  }
  totalPrimes +=count;
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

  cout << "prime-par2 (parallel) over [2.." << N << "] starting ...\n";
  auto start = chrono::steady_clock::now();
  bool candidate[N+1];
  int sqrtN = sqrt(N);
 
  thread thd[P];
  for(int k=0; k<P; k++){
    thd[k] = thread(process, sieve, N, P, k);
  }
  
  for(int k=0; k < P; k++){
    thd[k].join();
  }
  
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

  cout << "prime-par2 (parallel) found " << totalPrimes <<"  primes in "<<duration.count()<<" ms.\n";
}
