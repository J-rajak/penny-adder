/****************************************************************************
  This version adds 100 coins to the account. The one delay 
  has been replaced by a shorter delay caused by a redundant for loop.
  
  How long will it take to run?
  
  How much money will eventually be stored in the account?
  
  Compile with:

    cc -o penny06 penny06.c time_diff.c time_diff.c -lrt -pthread 

  Dr. Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

#include <stdio.h>
#include "time_diff.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int main(){
  struct timespec start, finish;
  long long int difference;   
  int account = 0;
  int i;

  int n = 100;

  clock_gettime(CLOCK_MONOTONIC, &start);

  void *add_penny();
  pthread_t t[n];
  for(i=0;i<n;i++){
    pthread_create(&t[i], NULL, add_penny, &account);
  }
  for(i=0;i<n;i++){
    pthread_join(t[i], NULL);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);
  printf("accumulated %dp\n", account);
  printf("run lasted %lldns or %9.5lfs\n", difference, difference/1000000000.0);
  return 0;
} 

void *add_penny(int *balance) {
  int b = *balance;
/* cause a short delay */
  int i;
  for(i=0;i<100000;i++){
  }
  b = b + 1;
  *balance = b;
}

