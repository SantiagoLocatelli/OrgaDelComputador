#ifndef RECODE57_H
#define RECODE57_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t codepoint;

enum encoding {
    UTF8, UTF16BE, UTF16LE, UTF32BE, UTF32LE, UNKNOWN
};

/*
 * Determina la codificación correspondiente a un byte order mark.
 *
 * Argumentos:
 *   - bom: un arreglo con los primeros 4 bytes del archivo.
 *
 * Devuelve:
 *   - el encoding detectado según el byte order mark (BOM) recibido.
 */
enum encoding bom_to_encoding(uint8_t *bom);

/*
 * Escribe el byte order mark correspondiente a una codificación.
 *
 * Argumentos:
 *   - enc: el encoding cuyo byte order mark debe escribirse.
 *   - buf: el arreglo donde escribir el byte order mark; contiene
 *          espacio para al menos 4 bytes.
 *
 * Devuelve:
 *   - el número de bytes escritos, o 0 si no se escribió BOM (por
 *     ejemplo en el caso de UTF-8).
 */
size_t write_bom(enum encoding enc, uint8_t *buf);

/*
 * Decodifica UN codepoint a partir de una secuencia de bytes.
 *
 * Argumentos:
 *   - enc: el encoding en que están codificados los bytes de entrada.
 *   - buf: arreglo con los bytes de entrada a decodificar.
 *   - n: el tamaño del arreglo buf.
 *   - dest: puntero donde escribir el codepoint decodificado.
 *
 * Devuelve:
 *   - el número de bytes consumidos para decodificar el codepoint (por
 *     ejemplo, siempre 4 en UTF32; 2 o 4 en UTF16; de 1 a 4 para UTF8).
 *
 *     Se devuelve 0 si no se pudo decodificar ningún codepoint, ya que no
 *     se consumió ningún byte. (Esto ocurre para UTF32 si n < 4; para UTF16
 *     si n < 2, o si n < 4 y el primer codepoint es un surrogate; etc.)
 */
size_t read_codepoint(enum encoding enc, uint8_t *buf, size_t n, codepoint *dest);

/*
 * Codifica un codepoint a una secuencia de bytes.
 *
 * Argumentos:
 *   - enc: el encoding en que codificar el codepoint.
 *   - cp: el codepoint a codificar en bytes.
 *   - outbuf: el arreglo de salida, de tamaño 4, donde escribir los
 *             bytes.
 *
 * Devuelve:
 *   - el número de bytes escritos en "outbuf" (siempre entre 1 y 4).
 */
size_t write_codepoint(enum encoding enc, codepoint cp, uint8_t *outbuf);

#endif
