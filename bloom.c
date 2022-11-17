#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "bloom.h"

struct bloom_t{
    uint64_t *tab;
    size_t bitSize;
    int numHash;
    hash_function hash;
};

static uint64_t permute64(uint64_t x, uint64_t key){
    x += key;
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9U;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebU;
    x ^= x >> 31;
    return x;
}

BloomFilter create(size_t bitSize, int numHash, hash_function hash){
   BloomFilter filter=(BloomFilter)malloc(sizeof(BloomFilter)); 
   filter->bitSize = bitSize;
   //allocation dynamique de la taille de hash contenir dans hash_function
   filter->hash = (hash_function)malloc(sizeof(hash_function));
   filter->hash = hash;
   filter->numHash = numHash;
   filter->tab = (uint64_t *)malloc(sizeof(uint64_t)*bitSize);
 
   //initialisation du tableau filter->tab
   for(int i=0 ;i<bitSize;i++)
        filter->tab[i]=0;

   return filter;
}


void insert(const char *value, BloomFilter *filter){
  uint64_t key = (**filter).hash(value);
  int index;
  for(int i = 0 ; i < (**filter).numHash ; i++)
  {
        //generate new hash function
        key = permute64(key,key);
        index = key%((**filter).bitSize);
        (**filter).tab[index] = 1;
        //printf("%d-->",index);
  }
    
}

bool search(const char *value, BloomFilter filter){
   // TODO Je dois faire ma part ici
    uint64_t key = (*filter).hash(value);
    int index;
    int count = 0;
    for(int i = 0 ; i < (*filter).numHash ; i++)
    {
        //generate new hash function
        key = permute64(key,key);
        index = key%((*filter).bitSize);
        if((*filter).tab[index] == 1)
            count+=1;
    }
    if(count == filter->numHash)
    return true;
    return false;
}

void release(BloomFilter filter){
   free(filter->tab);
    free(filter);
}
