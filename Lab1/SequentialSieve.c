#include <stdio.h>
#include <time.h>
#define N 1000000
static int sieve[N+1];

void eratosthenes(){
  for (long long int i = 0; i<=N; i++ ){
    if (i % 2 == 0){
      sieve[i] = 0;
    }else{
      sieve[i] = 1;
    }
  }
  sieve[1] = 0;
  sieve[2] = 1;

  for(long long int i=3; i<=N; i+=2){
    if(sieve[i]){
      for(long long int j=i*i; j <=N; j+=i){
        sieve[j] = 0;
      }
    }
  }
}

int main(){
  eratosthenes();
  return 0;
}
