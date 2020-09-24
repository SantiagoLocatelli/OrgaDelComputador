#ifndef __LISTA_H__
#define __LISTA_H__

#include <stdbool.h>
#include <stdlib.h>

typedef struct nodo {
    void* elemento;
    struct nodo* siguiente;
} nodo_t;

typedef struct lista_se {
    nodo_t* inicio;
    size_t  cantidad;
} lista_se_t,lista_t;

/*
 * Crea la estructura de la lista, inicializando
 * los nodos inicio en NULL y reservando la memoria
 * necesaria para la estructura.
 */
lista_se_t* crear_lista();

/*
 * Inserta un elemento luego al final de la lista, reservando la memoria necesaria para este nodo.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int insertar(lista_se_t* lista, void* elemento);

/*
 * Inserta un elemento en la posicion indicada, reservando la memoria necesaria para este nodo.
 * En caso de no existir la posicion indicada, lo inserta al final.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int insertar_en_posicion(lista_se_t* lista, void* elemento, int indice);

/*
 * Borra el elemento que se encuentra en la ultima posición liberando la memoria reservada para el.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int borrar(lista_se_t* lista);

/*
 * Borra el elemento que se encuentra en la posición indicada, liberando la memoria reservada para el.
 * En caso de no existir esa posición se intentará borrar el último elemento.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int borrar_de_posicion(lista_se_t* lista, int indice);

/*
 * Devuelve el elemento en la posicion indice.
 * Si no existe dicha posicion devuelve NULL.
 */
void* elemento_en_posicion(lista_se_t* lista, int indice);

/*
 * Devuelve el último elemento de la lista.
 * Si no existe dicha posicion devuelve NULL.
 */
void* ultimo(lista_se_t* lista);

/*
 * Devuelve true si la lista está vacía o false si no lo está.
 */
bool vacia_lista(lista_se_t* lista);

/*
 * Devuelve la cantidad de elementos en una lista.
 */
size_t elementos(lista_se_t* lista);

/*
 * Libera la memoria reservada por los nodos presentes en la lista y luego la memoria reservada por la estructura.
 * Devuelve 0 si pudo destruirla o -1 si no pudo.
 */
int destruir_lista(lista_se_t* lista);

#endif /* __LISTA_H__ */