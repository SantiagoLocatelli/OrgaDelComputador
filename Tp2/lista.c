#include "lista.h"
#include <stdio.h>

/*
 * Crea la estructura de la lista, inicializando
 * los nodos inicio y actual en NULL y reservando la memoria
 * necesaria para la estructura.
 */
lista_se_t* crear_lista(){
    lista_se_t* nueva_lista = malloc(sizeof(lista_se_t));
    if(!nueva_lista)
        return NULL;
    nueva_lista->cantidad = 0;
    nueva_lista->inicio = NULL;
    return nueva_lista;
}

/*
 * Inserta un elemento luego al final de la lista, reservando la memoria necesaria para este nodo.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int insertar(lista_se_t* lista, void* elemento){
    nodo_t* nuevo_nodo;
    nodo_t* ultimo_nodo;
    if(!lista || !elemento)
        return -1;
    nuevo_nodo = malloc(sizeof(nodo_t));
    if(!nuevo_nodo)
        return -1;
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;
    if(vacia_lista(lista)){
        lista->inicio = nuevo_nodo;
    }else{
        ultimo_nodo = lista->inicio;
        while(ultimo_nodo->siguiente){
            ultimo_nodo = ultimo_nodo->siguiente;
        }
        ultimo_nodo->siguiente = nuevo_nodo;
    }
    lista->cantidad += 1;
    return 0;
}

/*
 * Inserta un elemento en la posicion indicada, reservando la memoria necesaria para este nodo.
 * En caso de no existir la posicion indicada, lo inserta al final.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int insertar_en_posicion(lista_se_t* lista, void* elemento, int indice){
    nodo_t* nuevo_nodo;
    nodo_t* nodo_posicion;
    if(!lista || !elemento)
        return -1;
    if(indice > lista->cantidad)
        return insertar(lista,elemento);
    nuevo_nodo = malloc(sizeof(nodo_t));
    if(!(nuevo_nodo))
        return -1;
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;
    nodo_posicion = lista->inicio;
    if(indice == 0){
        nuevo_nodo->siguiente = lista->inicio;
        lista->inicio = nuevo_nodo;
    } else {
        for (int i = 0; i < indice - 1; ++i) {
            nodo_posicion = nodo_posicion->siguiente;
        }
        nuevo_nodo->siguiente = nodo_posicion->siguiente;
        nodo_posicion->siguiente = nuevo_nodo;
    }
    lista->cantidad +=1;
    return 0;
}

/*
 * Borra el elemento que se encuentra en la ultima posición liberando la memoria reservada para el.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int borrar(lista_se_t* lista){
    nodo_t* nodo_a_eliminar;
    if(!lista || vacia_lista(lista))
        return -1;
    nodo_a_eliminar = lista->inicio;
    for (int i = 0; i < lista->cantidad - 1; ++i) {
        nodo_a_eliminar = nodo_a_eliminar->siguiente;
    }
    free(nodo_a_eliminar);
    lista->cantidad -= 1;
    if(lista->cantidad == 0)
        lista->inicio = NULL;
    return 0;
}

/*
 * Borra el elemento que se encuentra en la posición indicada, liberando la memoria reservada para el.
 * En caso de no existir esa posición se intentará borrar el último elemento.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int borrar_de_posicion(lista_se_t* lista, int indice){
    nodo_t* nodo_a_eliminar;
    nodo_t* nodo_aux;
    if(!lista || vacia_lista(lista))
        return -1;
    if(indice > lista->cantidad)
        return -1;
    if(indice != 0){
        nodo_aux = lista->inicio;
        for (int i = 0; i < indice-1; ++i) {
            nodo_aux = nodo_aux->siguiente;
        }
        nodo_a_eliminar = nodo_aux->siguiente;
        nodo_aux->siguiente = nodo_a_eliminar->siguiente;
    } else {
        nodo_a_eliminar = lista->inicio;
        lista->inicio = nodo_a_eliminar->siguiente;
    }
    free(nodo_a_eliminar);
    lista->cantidad -= 1;
    if(lista->cantidad == 0)
        lista->inicio = NULL;
    return 0;
}

/*
 * Devuelve el elemento en la posicion indice.
 * Si no existe dicha posicion devuelve NULL.
 */
void* elemento_en_posicion(lista_se_t* lista, int indice){
    nodo_t* nodo_posicion;
    if(!lista || vacia_lista(lista) || indice >= lista->cantidad)
        return NULL;
    nodo_posicion = lista->inicio;
    for (int i = 0; i < indice; ++i) {
        nodo_posicion = nodo_posicion->siguiente;
    }
    return nodo_posicion->elemento;
}

/*
 * Devuelve el último elemento de la lista.
 * Si no existe dicha posicion devuelve NULL.
 */
void* ultimo(lista_se_t* lista){
    nodo_t* ultimo_nodo;
    if(!lista || vacia_lista(lista))
        return NULL;
    ultimo_nodo = lista->inicio;
    for (int i = 0; i < lista->cantidad-1; ++i) {
        ultimo_nodo = ultimo_nodo->siguiente;
    }
    return ultimo_nodo->elemento;
}

/*
 * Devuelve true si la lista está vacía o false si no lo está.
 */
bool vacia_lista(lista_se_t* lista){
    return (!lista || lista->cantidad == 0 || lista->inicio == 0);
}

/*
 * Devuelve la cantidad de elementos en una lista.
 */
size_t elementos(lista_se_t* lista){
    if(!lista)
        return 0;
    return lista->cantidad;
}

/*
 * Libera la memoria reservada por los nodos presentes en la lista y luego la memoria reservada por la estructura.
 * Devuelve 0 si pudo destruirla o -1 si no pudo.
 */
int destruir_lista(lista_se_t* lista){
    if(!lista)
        return -1;
    while (borrar(lista) == 0);
    free(lista);
    return 0;
}