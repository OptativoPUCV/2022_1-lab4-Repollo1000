#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  long index = hash(key,map->capacity);
  while(map->buckets[index] != NULL && map->buckets[index]->key != NULL)
  {
    if(is_equal(map->buckets[index]->key,key)==1) return;
    index = (index + 1) % map->capacity;   
  }
  if(map->buckets[index] != NULL)
  {
    map->buckets[index]->key = key;
    map->buckets[index]->value = value;
  }
  else
    map->buckets[index] = createPair(key,value);
  map->size++;
  
  if((double) map->size/(double) map->capacity > 0.75) enlarge(map);
}

void enlarge(HashMap * map) {
 enlarge_called = 1; //no borrar (testing purposes)
  Pair** old = (Pair**) calloc (map->capacity, sizeof(Pair*));
  for(long i = 0 ; i < map->capacity ; i++)
  {
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      old[i] = createPair(map->buckets[i]->key, map->buckets[i]->value);
    }
  }
  map->capacity = map->capacity * 2;
  Pair** new = (Pair**) calloc (map->capacity, sizeof(Pair*));
  map->buckets = new;
  map->size = 0;
  for(long i = 0 ; i < map->capacity/2 ; i++)
  {
    if(old[i] != NULL && old[i]->key != NULL)
    {
      insertMap(map, old[i]->key, old[i]->value);
    }
  } 

}


HashMap * createMap(long capacity) {

  HashMap *index = (HashMap *)malloc(sizeof(HashMap));
  
  index->size = 0;
  index->capacity = capacity;
  index->current = -1;
  
  index->buckets = (Pair **)calloc(capacity,sizeof(Pair*));
  
  return index;
}

void eraseMap(HashMap * map,  char * key) 
{  
 long index = hash(key,map->capacity);
  while(map->buckets[index] != NULL && map->buckets[index]->key != NULL)
  {
    if(is_equal(map->buckets[index]->key,key)==1) 
    {
      map->current = index;
      map->buckets[index]->key = NULL;
      map->size = map->size - 1;
    }
    index = (index + 1) % map->capacity;   
  }

}

Pair * searchMap(HashMap * map,  char * key) 
{  
  
  long index = hash(key,map->capacity);
  while(map->buckets[index] != NULL && map->buckets[index]->key != NULL)
  {
    if(is_equal(map->buckets[index]->key,key)==1) 
    {
      map->current = index;
      return map->buckets[index];
    }
    index = (index + 1) % map->capacity;   
  }
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
  for(int i = 0 ; i < map->size ; i++)
  {
      if(map->buckets[i] != NULL)
      {
        if(map->buckets[i]->key != NULL)
        {
          map->current = i;
          return map->buckets[i];
        }
      }
  }
    return NULL;
}

Pair * nextMap(HashMap * map) 
{
   for(long i = map->current ; i < map->capacity ; i++)
    {
      if(map->buckets[i] != NULL && map->buckets[i]->key != NULL && i != map->current)
      {
        map->current = i;
        return map->buckets[i];
      } 
    }
  return NULL;
}
