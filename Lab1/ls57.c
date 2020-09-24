#include "ls57.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


ssize_t getsize(int atdirfd, const char *filename){

  struct stat st;
  fstatat( atdirfd, filename, &st, AT_SYMLINK_NOFOLLOW );
  if( (st.st_mode & S_IFMT) ==  S_IFREG )
    return (ssize_t)st.st_size;

  return NOT_REG;
}


int cmpfiles( const void* one_file, const void* another_file ){

  const file_t* one_f = (file_t*) one_file;
  const file_t* another_f = (file_t*) another_file;
  ssize_t result;

  result = one_f->fsize - another_f->fsize;
  if( result == 0 )
    result = strcmp( one_f->name, another_f->name );

  return (int)result;
}


void print_files( file_t files[], int quan_files ){
  for( int i = 0; i < quan_files; i++ ){

    if( files[i].fsize == NOT_REG )
      printf( "%s\n", files[i].name );
    else
      printf( "%s\t%li\n", files[i].name, files[i].fsize );
  }
}


void lsdir(DIR *dir){

  int dir_fd = dirfd( dir );
  struct dirent* directory;
  int quan_files = 0;
  file_t files[MAX_FILES];

  directory = readdir( dir );
  while( directory != NULL ){
    if( directory->d_name[0] != '.' ){
      files[quan_files].fsize = getsize( dir_fd, directory->d_name );
      strncpy( files[quan_files].name, directory->d_name, strlen(directory->d_name) );
      quan_files++;
    }
    directory = readdir( dir );
  }

  qsort( files, quan_files, sizeof(file_t), &cmpfiles );
  print_files( files, quan_files );
}


int main( int argc, char*  argv[] ){

  if( argc != 1 && argc != 2 ){

    printf( "La cantidad de argumentos ingresada no es correcta. Si usted quiere mostrar los contenidos del directorio actual debe ingresar: ls. En el caso de que ustedes quiera listar los contenidos de un directorio en especifico debera agregar la ruta, ejemplo: ls ~/Desktop/.../..." );
    return ERROR;
  }

  DIR* dir;
  if( argc == 1 )
    dir = opendir(".");
  else
    dir = opendir( argv[1] );

  if( dir == NULL ){

    if( errno == ENOTDIR )
      printf("No es un directorio. ");

    perror("Ocurrio un problema al abrir el directorio.");
    return ERROR;
  }

  lsdir( dir );
  closedir( dir );
  return 0;
}
