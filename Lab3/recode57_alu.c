#include "recode57.h"


enum encoding bom_to_encoding(uint8_t *bom){

  if( (bom[0] == 0xFE) && (bom[1] == 0xFF) )
    return UTF16BE;

  else if( (bom[0] == 0) && (0 == bom[1]) && (bom[2] == 0xFE) && (bom[3] == 0xFF) )
    return UTF32BE;

  else if( (bom[0] == 0xFF) && (bom[1] == 0xFE) && (0 == bom[2]) && (bom[3] == 0) )
    return UTF32LE;

  else if( (bom[0] == 0xFF) && (bom[1] == 0xFE) )
    return UTF16LE;

  return UTF8;
}


size_t write_bom(enum encoding enc, uint8_t *buf){

  if( enc == UTF16BE ){
    buf[0] = 0xFE;
    buf[1] = 0xFF;
    return 2;
  }else if( enc == UTF16LE ){
    buf[0] = 0xFF;
    buf[1] = 0xFE;
    return 2;
  }else if( enc == UTF32BE ){
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0xFE;
    buf[3] = 0xFF;
    return 4;
  }else if( enc == UTF32LE ){
    buf[0] = 0xFF;
    buf[1] = 0xFE;
    buf[2] = 0;
    buf[3] = 0;
    return 4;
  }
  return 0;
}


size_t read_codepoint(enum encoding enc, uint8_t *buf, size_t n, codepoint *dest){

  size_t bytes_consumed = 0;
  codepoint cp = 0;
  if( enc == UTF32BE || enc == UTF32LE ){

    if( n < 4 )
      return bytes_consumed;

    bytes_consumed = 4;
    if( enc == UTF32BE ){
      cp |= buf[0] << 24;
      cp |= buf[1] << 16;
      cp |= buf[2] << 8;
      cp |= buf[3];

    }else{
      cp |= buf[0];
      cp |= buf[1] << 8;
      cp |= buf[2] << 16;
      cp |= buf[3] << 24;
    }
  }else if( enc == UTF16BE || enc == UTF16LE ){

    if( n < 2 )
      return bytes_consumed;

    if( enc == UTF16BE ){

      cp |= buf[0] << 8;
      cp |= buf[1];
      if( cp < 0xD800 || cp > 0xDFFF )
        bytes_consumed = 2;

      else{  // SURROGATE
        if( n < 4 )
          return bytes_consumed;

        bytes_consumed = 4;
        cp = 0x10000;
        cp |= buf[3];
        cp |= (buf[2] & 0x3) << 8;
        cp |= (buf[1] & 0xFF) << 10;
        cp |= (buf[0] & 0x3) << 18;
      }
    }else{ // UTF16LE

      cp |= buf[1] << 8;
      cp |= buf[0];
      if( cp < 0xD800 || cp > 0xDFFF )
        bytes_consumed = 2;

      else{  // SURROGATE
        if( n < 4 )
          return bytes_consumed;

        bytes_consumed = 4;
        cp = 0x10000;
        cp |= buf[2];
        cp |= (buf[3] & 0x3) << 8;
        cp |= buf[0] << 10;
        cp |= (buf[1] & 0x3) << 18;
      }
    }
  }else{ // UTF8

    // Un solo byte
    if( (n > 0) && (buf[0] <= 0x7F) ){
      bytes_consumed = 1;
      cp = buf[0];

    // Dos bytes
    }else if( buf[0] <= 0xDF ){
      if( n < 2 )
        return bytes_consumed;

      bytes_consumed = 2;
      cp |= buf[1] & 0x3F;
      cp |= (buf[0] & 0x1F) << 6;

    // Tres bytes
    }else if( buf[0] <= 0xEF ){
      if( n < 3 )
        return bytes_consumed;

      bytes_consumed = 3;
      cp |= buf[2] & 0x3F;
      cp |= (buf[1] & 0x3F) << 6;
      cp |= (buf[0] & 0x0F) << 12;

    // Cuatro bytes
    }else{
      if( n < 4 )
        return bytes_consumed;

      bytes_consumed = 4;
      cp |= buf[3] & 0x3F;
      cp |= (buf[2] & 0x3F) << 6;
      cp |= (buf[1] & 0x3F) << 12;
      cp |= (buf[0] & 0x07) << 18;
    }
  }
  *dest = cp;
  return bytes_consumed;
}


size_t write_codepoint(enum encoding enc, codepoint cp, uint8_t *outbuf){

    size_t bytes_wr = 0;

    if( (enc == UTF32BE) || (enc == UTF32LE) ){

      bytes_wr = 4;
      if( enc == UTF32BE ){
        outbuf[0] = (cp & 0xFF000000) >> 24;
        outbuf[1] = (cp & 0xFF0000) >> 16;
        outbuf[2] = (cp & 0xFF00) >> 8;
        outbuf[3] = cp & 0xFF;

      }else if( enc == UTF32LE ){
        outbuf[0] = cp & 0xFF;
        outbuf[1] = (cp & 0xFF00) >> 8;
        outbuf[2] = (cp & 0xFF0000) >> 16;
        outbuf[3] = (cp & 0xFF000000) >> 24;
      }
    }else if( enc == UTF16BE || enc == UTF16LE ){

      if( enc == UTF16BE ){
        if( cp <=0xFFFF ){
          bytes_wr = 2;
          outbuf[0] = (cp & 0xFF00) >> 8;
          outbuf[1] = cp & 0xFF;

        }else if( cp <= 0x10FFFF ){  // SURROGATE
          bytes_wr = 4;
          cp = cp - 0x10000;

          outbuf[3] = cp & 0xFF;
          outbuf[2] = ((cp >> 8) & 0x3) | 0xDC;
          outbuf[1] = ((cp >> 10) & 0xFF);
          outbuf[0] = ((cp >> 24) & 0x3) | 0xD8;
        }
      }else{ // UTF16LE

        if( cp <= 0xFFFF ){
          bytes_wr = 2;
          outbuf[0] = cp & 0xFF;
          outbuf[1] = (cp & 0xFF00) >> 8;

        }else if( cp <= 0x10FFFF ){ // SURROGATE
          bytes_wr = 4;
          cp = cp - 0x10000;

          outbuf[2] = cp & 0xFF;
          outbuf[3] = ((cp >> 8) & 0x3) | 0xDC;
          outbuf[0] = ((cp >> 10) & 0xFF);
          outbuf[1] = ((cp >> 24) & 0x3) | 0xD8;
        }
      }
    }else{

      if( cp <= 0x7F ){
        bytes_wr = 1;
        outbuf[0] = cp;

      }else if( cp <=  0x7FF){
        bytes_wr = 2;
        outbuf[1] = (cp & 0x3F) | 0x80;
        outbuf[0] = ((cp >> 6) & 0x1F) | 0xC0;

      }else if( cp <= 0xFFFF ){
        bytes_wr = 3;
        outbuf[2] = (cp & 0x3F) | 0x80;
        outbuf[1] = ((cp >> 6) & 0x3F) | 0x80;
        outbuf[0] = ((cp >> 12) & 0x0F) | 0xE0;

      }else if( cp <= 0x10FFFF ){

        bytes_wr = 4;
        outbuf[3] = (cp & 0x3F) | 0x80;
        outbuf[2] = ((cp >> 6) & 0x3F) | 0x80;
        outbuf[1] = ((cp >> 12) & 0x3F) | 0x80;
        outbuf[0] = ((cp >> 18) & 0x07) | 0xF0;
      }
    }
    return bytes_wr;
}
