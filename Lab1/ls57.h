#ifndef __LS57_H__
#define __LS57_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define NOT_REG -1
#define MAX_FILES 250
#define MAX_CHAR 256
#define ERROR -1


/*
* Tome como criterio el realizar esta estructura, ya que para el problema
* que plantea el lab se hace mas facil de esta manera.
*/
typedef struct file{

  ssize_t fsize;
  char name[MAX_CHAR];
}file_t;


/*
* print_files() imprimira todos los archivos almacenados en el vector,
* imprime solo el nombre en caso de ser un archivo no regular, en caso
* contrario imprime nombre y tamaño.
*
* Precondicion: quan_files > 0.
*/
void print_files( file_t files[], int quan_files );


/*
 * getsize() devuelve (usando fstatat) el tamaño de un archivo
 * que se ubica en el directorio asociado al file descriptor
 * "atdirfd". Si el archivo no es un archivo regular, se devuelve
 * -1.
 *
 * Precondiciones: atdirfd >= 0, filename != NULL.
 */
ssize_t getsize(int atdirfd, const char *filename);


/*
*  cmpfiles() devuelve un numero positivo si one_file es de mayor tamaño que another_file.
*  En caso, de que tengan el mismo tamaño, desempata con el orden alfabetico, ascendentemente.
*/
int cmpfiles( const void* one_file, const void* another_file );


/*
 * lsdir() muestra los contenidos de un directorio, incluyendo
 * los tamaños de archivos regulares (usando getsize).
 *
 * Precondiciones: dir != NULL.
 */
void lsdir(DIR *dir);

#endif
