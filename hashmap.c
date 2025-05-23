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


void insertMap(HashMap * map, char * key, void * value) {
    long posicion = hash(key, map->capacity);
    long posOriginal = posicion; //guardamos la posicion original para evitar bucles infinitos
    
    while (map->buckets[posicion] != NULL) { //si ya hay un par en esa posicion
        if (map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key, key)) { //si la llave ya existe
            //clave duplicada
            return;
        }
        if (map->buckets[posicion]->key == NULL) { //si la llave es NULL, significa que el espacio esta libre
            break; //salimos del bucle para insertar el nuevo par
        }
        posicion = (posicion + 1) %map->capacity; //buscamos la siguiente posicion
        if (posicion == posOriginal) { 
            return;
        }
    }
    Pair * nuevo = createPair(strdup(key), value); 
    map->buckets[posicion] = nuevo;
    map->size++;
    map->current = posicion;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair ** old_buckets = map->buckets;
    long old_capacity = map->capacity; 
    map->capacity *= 2; 
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *)); //reservamos memoria para los nuevos pares
    map->size = 0; 
    for (long i = 0; i < old_capacity; i++) {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value); 
        }
    }
}


HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity); //reservamos memoria para los pares
    for (int i = 0; i < capacity; i++) { //inicializamos los pares a NULL
        map->buckets[i] = NULL;
    }
    map->capacity = capacity; //asignamos la capacidad
    map->size = 0; //inicializamos el tamaño a 0
    map->current = -1; 
    return map;
}

void eraseMap(HashMap * map,  char * key) {   
    long posicion = hash(key, map->capacity); //obtenemos la posicion de la llave
    long posOriginal = posicion; 
    while (map->buckets[posicion] != NULL) { 
        if (is_equal(map->buckets[posicion]->key, key)) { 
            map->buckets[posicion]->key = NULL;
            map->current = posicion; //guardamos la posicion del par encontrado
            map->size--;
        }
        else {
            posicion = (posicion + 1) % map->capacity; //buscamos la siguiente posicion
            if (posicion == posOriginal) { //si volvimos a la posicion original, significa que no hay mas pares
                return;
            }
        }
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long posicion = hash(key, map->capacity);
    while (map->buckets[posicion] != NULL) { //si la llave no es igual, seguimos buscando
        if (is_equal(map->buckets[posicion]->key, key)) {
            map->current = posicion; //guardamos la posicion del par encontrado
            return map->buckets[posicion]; //si la llave es igual a la que buscamos, devolvemos el par
        }
        posicion = (posicion + 1) % map->capacity; //buscamos la siguiente posicion
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    for (int i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) { //si la posicion no es NULL, devolvemos el par
            map->current = i; //guardamos la posicion del par encontrado
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for(int i = map->current + 1; i < map->capacity; i++) { //partimos desde le siguiente al current
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) { 
            map->current = i; //guardamos la posicion del par encontrado
            return map->buckets[i];
        }
    }
    return NULL;
}
