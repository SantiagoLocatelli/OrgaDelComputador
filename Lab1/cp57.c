#include "cp57.h"
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


size_t writeall(int fd, const void *buf, size_t count){

  size_t bytes_wr = 0;
  ssize_t result;

  do{
    result = write( fd, buf + bytes_wr, count - bytes_wr );
    if( result != ERROR )
      bytes_wr += result;
    while( (result == ERROR) && (errno == EINTR) ){
      errno = 0;
      result = write( fd, buf + bytes_wr, count - bytes_wr);
    }
  }while( (bytes_wr < count) && (result > 0) );

  if( bytes_wr == count )
    return 0;

  return bytes_wr;
}


size_t copyfd(int fdin, int fdout, void *buf, size_t size){

  size_t bytes_wr = 0;
  ssize_t result_rd;
  ssize_t result_wr;
  do{
    result_rd = read( fdin, buf, size );
    while( (result_rd == ERROR) && (errno == EINTR) ){
      errno = 0;
      printf( "La accion fue interrumpida. Reintentando...\n" );
      result_rd = read( fdin, buf, size );
    }

    if( result_rd != ERROR ){
      result_wr = writeall( fdout, buf, result_rd );
      if( result_wr == 0 ) // si result_wr es cero quiere decir que writeall no tuvo problemas en escribir lo que se leyo.
        bytes_wr += result_rd;
      else
        bytes_wr += result_wr;
    }
  }while( (result_rd > 0) && (result_wr == 0) );

  if( result_rd == ERROR ){
    if( errno == EISDIR ){
      perror( "El archivo origen es en verdad un directorio.\n" );
      return ERROR;
    }
    perror( "Ocurrio un error mientras se leia el archivo.\n" );
  }
  if( result_rd == 0 )
    return ALL_WRITTEN;

  return bytes_wr;
}


int creat_symlink( char* old_symlink, char* new_symlink, size_t bufsize ){

  char *buf;
  int sym_result;
  ssize_t read_result;
  if( ((buf = malloc(bufsize)) == NULL) ||
      ((read_result = readlink( old_symlink, buf, bufsize )) == ERROR) ||
      ((sym_result = symlink( buf, new_symlink )) == ERROR) )

    fprintf(stderr, "Ocurrio un error en la creacion del symlink: %m " "(buf = %p, readlink = %li, symlink = %i)\n", buf, read_result, sym_result );
  else
    printf("Se pudo crear un nuevo symlink correctamente.\n");

  free(buf);
  return sym_result;
}


int main( int argc, char* argv[] ){

  if( argc != 3 ){
    printf( "La cantidad de argumentos no es la correcta.\n" );
    return ERROR;
  }

  int result;
  struct stat st;
  result = lstat( argv[1], &st );
  if( result == ERROR ){

    perror( "Ocurrio un error en la operacion. (lstat)\n" );
    return result;
  }

  // ARRANCA EL OPCIONAL 2
  if( (st.st_mode & S_IFMT) == S_IFLNK ){

    result = creat_symlink( argv[1], argv[2], st.st_size );
    return result;
  }
  // TERMINA EL OPCIONAL 2

  int origin_file = open( argv[1], O_RDONLY );
  if( origin_file == ERROR ){

    perror( "Ocurrio un error al a brir el archivo origen.\n");
    return ERROR;
  }

  int destiny_file = open( argv[2], O_WRONLY | O_CREAT | O_TRUNC, 00666 );
  // ARRANCA OPCIONAL 1
  if( destiny_file == ERROR ){

    if( errno != EISDIR ){

      perror( "Ocurrio un error al abrir el archivo destino.\n" );
      return ERROR;
    }
    int dir_fd = open( argv[2], O_RDONLY | O_DIRECTORY );
    if( dir_fd == ERROR ){

      perror( "Error al abrir el archivo destino." );
      close(origin_file);
      return ERROR;
    }
    char* destiny = strchr( argv[1], '/' );

    if( destiny != NULL )
      destiny++;
    else
      destiny = argv[1];

    destiny_file = openat( dir_fd, destiny, O_WRONLY | O_CREAT | O_TRUNC, 00666 );
    close(dir_fd);
  }
  // TERMINA OPCIONAL 1

  uint8_t buf[BYTES_TO_RD];
  size_t result_2 = copyfd( origin_file, destiny_file, buf, sizeof(buf) );
  // Cree una variable nueva, porque las pruebas de github me tiraba una warning de que estaba asignando un size_t a int.

  close(origin_file);
  close(destiny_file);

  if( result_2 == 0 )
    printf("La accion se realizo con exito, y se pudo copiar todo el archivo.\n");
  else
    printf("La accion se realizo con exito, solo se llego a copiar %li bytes\n", result_2);

  return result;
}
