//william barrett, CDA Bonus Assignment
#include <stdio.h>
#include <stdlib.h>

#define CACHE_SIZE 32
#define two_way_tag = CACHE_SIZE / 2
#define four_way_tag = CACHE_SIZE / 4

typedef struct cache{
  int tag;  //tag for when comparing values
  int instances;  //instances for lru
}cache;

void direct_mapping(){
  FILE *ifp = fopen("traces.txt", "r");
  unsigned int num;
  cache cache[CACHE_SIZE];  
  int index;
  unsigned int tag;
  int hit=0, miss=0;

  //init cache block to -1
  for(int i=0; i<CACHE_SIZE; i++){
    cache[i].tag = -11;
    cache[i].instances = 0;
  }
  
  //loop to read each number until EOF
  while(fscanf(ifp, "%x", &num) != EOF){
    //direct caching
    tag = num / CACHE_SIZE;
    index = num % CACHE_SIZE;
    if(cache[index].tag == tag){
      //there is a hit
      hit++;
    }
    else{
      // acccess inc, then go through replacement
      miss++;
      cache[index].tag = tag;
      //no need to update instances as we are not doing an LRU
    }
  }
  int acs = hit+miss;
  float rate = (hit*1.0)/(acs*1.0) * 100;
  printf("DIRECT:\nhits: %d, accesses: %d, rate: %0.2f%%", hit, acs, rate);
  printf("\n\n");
  
  fclose(ifp);
};

void two_way(){
  FILE *ifp = fopen("traces.txt", "r");
  unsigned int num;
  cache cache[CACHE_SIZE / 2][2]; 
  int index;
  unsigned int tag;
  int hit=0, miss=0;
  int offset_bits = 4; //log base 2 of 16
  int lru;
  int accesses = 0;

  while(fscanf(ifp, "%x", &num) != EOF){
    index = num % 8;
    tag = num / 16;
      if(cache[index][0].tag == tag || cache[index][1].tag == tag){
        //there is a hit within the found set
        hit++;
      }
      else{
        //no hit in the set
        miss++;
        if(cache[index][0].instances >= cache[index][1].instances)
          lru = 1;
        else
          lru = 0;

        cache[index][lru].tag = tag;
        for(int i=0; i<2; i++)
          cache[index][i].instances++;
        cache[index][lru].instances = 0;
      }
  }
  int acs = hit+miss;
  float rate = (hit*1.0)/(acs*1.0) * 100;
  printf("2-WAY(LRU):\nhits: %d, accesses: %d, rate: %0.2f%%", hit, acs, rate);
  printf("\n\n");
  fclose(ifp);
}
void four_way(){
  FILE *ifp = fopen("traces.txt", "r");
  unsigned int num;
  cache cache[CACHE_SIZE / 4][4]; 
  int index;
  unsigned int tag;
  int hit=0, miss=0;
  int lru;
  int accesses = 0;

  while(fscanf(ifp, "%x", &num) != EOF){
    index = num % 2;
    tag = num / 8;
      if(cache[index][0].tag == tag || cache[index][1].tag == tag ||
        cache[index][2].tag == tag || cache[index][3].tag == tag){
        //there is a hit within the found set
        hit++;
      }
      else{
        //no hit in the set
        miss++;
        if(cache[index][0].instances >= cache[index][1].instances)
          lru = 1;
        else
          lru = 0;

        cache[index][lru].tag = tag;
        for(int i=0; i<2; i++)
          cache[index][i].instances++;
        cache[index][lru].instances = 0;
      }
  }
  int acs = hit+miss;
  float rate = (hit*1.0)/(acs*1.0) * 100;
  printf("4-WAY(LRU):\nhits: %d, accesses: %d, rate: %0.2f%%", hit, acs, rate);
  printf("\n\n");
  fclose(ifp);
}

void fullyA(){
  //use static arr size 32
  //load value into arr if not found in it already
  //replace using rand
  FILE *ifp = fopen("traces.txt", "r");
  unsigned int num;
  cache cache[CACHE_SIZE];
  int rand_index = rand()%32;
  int hit=0, miss=0, flag = 0;
  int tag;

  for(int i=0; i<CACHE_SIZE; i++){
    cache[i].tag = -1;
  }

  while(fscanf(ifp, "%x", &num) != EOF){
    tag = num / CACHE_SIZE;
    for(int i=0; i<CACHE_SIZE; i++){
      if(cache[i].tag == tag){
        //the number is in the array so inc number of hits
        flag = 1;
      }
    }
    if(flag){
      hit++;
    }
    else{
      miss++;
      cache[rand_index].tag = tag;
    }

  }
  int acs = hit+miss;
  float rate = (hit*1.0)/(acs*1.0) * 100;
  printf("FULL(rand):\nhits: %d, accesses: %d, rate: %0.2f%%", hit, acs, rate);
  fclose(ifp);
}

int main(void) {
  //DIRECT MAPPED
  //-----------------
  //read file
  //load number
  //store based on number mod 32
  //check if already existant in 32 block storage based on index
  //repeate until EOF
  //------------------

  //2-WAY
  //------------------
  //read file
  //load number as a struct holding the tag value
  //put into set based on % 2
  //check next number set if match, check tag value
  //same thing for 4-way ^ but %4
  //------------------

  direct_mapping();
  two_way();
  four_way();
  fullyA();
  
  return 0;
}