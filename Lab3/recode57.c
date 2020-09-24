#include "recode57.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 * Devuelve el encoding correspondiente al argumento.
 */
static enum encoding str_to_encoding(const char *enc) {
    if (strcmp(enc, "UTF-8") == 0)
        return UTF8;
    else if (strcmp(enc, "UTF-16BE") == 0)
        return UTF16BE;
    else if (strcmp(enc, "UTF-16LE") == 0)
        return UTF16LE;
    else if (strcmp(enc, "UTF-32BE") == 0)
        return UTF32BE;
    else if (strcmp(enc, "UTF-32LE") == 0)
        return UTF32LE;
    else
        return UNKNOWN;
}


/*
 * Transforma una codificación a UCS-4.
 *
 * Argumentos:
 *
 *   - enc: el encoding original.
 *   - buf: los bytes originales.
 *   - nbytes: número de bytes disponibles en buf.
 *   - destbuf: resultado de la conversión.
 *
 * Devuelve: el número de CODEPOINTS obtenidos (número
 *           de elementos escritos en destbuf).
 *
 * Actualiza: nbytes contiene el número de bytes que no se
 *            pudieron consumir (secuencia o surrgate incompleto).
 *
 * Se debe garantiza que "destbuf" puede albergar al menos nbytes
 * elementos (caso enc=UTF-8, buf=ASCII).
 */
static size_t orig_to_ucs4(enum encoding enc, uint8_t *buf, size_t *nbytes, codepoint *destbuf) {
    size_t b = 0, n = 0;

    while ((b = read_codepoint(enc, buf, *nbytes, &destbuf[n])) > 0) {
        n++;
        buf += b;
        *nbytes -= b;
    }

    return n;
}

/*
 * Transforma UCS-4 a la codificación deseada.
 *
 * Argumentos:
 *
 *   - enc: el encoding destino.
 *   - input: los codepoints a codificar.
 *   - npoints: el número de codepoints en input.
 *   - output: resultado de la conversión.
 *
 * Devuelve: el número de BYTES obtenidos (número
 *           de elementos escritos en destbuf).
 *
 * Se debe garantiza que "destbuf" puede albergar al menos npoints*4
 * elementos, o bytes (caso enc=UTF-32).
 */
static size_t ucs4_to_dest(enum encoding enc, codepoint *input, size_t npoints, uint8_t *outbuf) {
    size_t b = 0;

    for (size_t i=0; i < npoints; i++) {
        b += write_codepoint(enc, input[i], &outbuf[b]);
    }

    return b;
}


int main(int argc, char *argv[]) {
    enum encoding orig_enc, dest_enc;

    if (argc != 2) {
        fprintf(stderr, "Uso: ./recode57 <encoding>\n");
        return 1;
    }

    if ((dest_enc = str_to_encoding(argv[1])) >= UNKNOWN) {
        fprintf(stderr, "Encoding no válido: %s\n", argv[1]);
        return 1;
    }

    // Detectar codificación origen con byte order mark.
    uint8_t bom[4];

    read(STDIN_FILENO, bom, 4);
    orig_enc = bom_to_encoding(bom);

    // En cada iteración, leer hasta 1024 bytes, convertirlos a UCS-4
    // (equivalente a UTF32-LE con enteros nativos) y convertirlo por
    // salida estándar.
    uint8_t inbuf[1024];
    uint8_t outbuf[1024*4+4]; // Worst case for 1024 codepoints + BOM.
    codepoint ucs4[1024];
    size_t prevbytes = 0;
    ssize_t inbytes;

    // Si orig_enc no fue UTF-32, quedaron 2 o 4 bytes en "bom" que
    // deben ser prefijados en inbuf.
    // TODO: bom_to_encoding() debería devolver el tamaño del BOM.
    if (orig_enc == UTF8) {
        memcpy(inbuf, bom, 4);
        prevbytes = 4;
    } else if (orig_enc == UTF16BE || orig_enc == UTF16LE) {
        memcpy(inbuf, bom + 2, 2);
        prevbytes = 2;
    }

    // Lo primero que se debe escribir, de ser necesario, es el BOM.
    size_t bom_bytes = write_bom(dest_enc, outbuf);

    while ((inbytes = read(STDIN_FILENO, inbuf + prevbytes, sizeof(inbuf) - prevbytes)) > 0) {
        inbytes += prevbytes;
        prevbytes = inbytes;
        // fprintf(stderr, "Processing: %zu bytes, ", prevbytes);

        size_t npoints = orig_to_ucs4(orig_enc, inbuf, &prevbytes, ucs4);
        size_t outbytes = ucs4_to_dest(dest_enc, ucs4, npoints, outbuf + bom_bytes);
        // fprintf(stderr, "codepoints: %d, output: %d bytes, remaining: %zu bytes\n",
        //         npoints, outbytes, prevbytes);

        write(STDOUT_FILENO, outbuf, outbytes + bom_bytes);
        bom_bytes = 0;  // Solo se usa en la primera iteración.

        if (prevbytes > 0) {
            memcpy(inbuf, inbuf + inbytes - prevbytes, prevbytes);
        }
    }
}
