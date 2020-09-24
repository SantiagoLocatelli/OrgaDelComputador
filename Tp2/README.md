#### Introduccion:
  Decidimos hacer el trabajo con una lista implicita con la siguiente estructura de bloques:
    
  ``` c
    struct bloque{
      header
      payload
      padding
      footer
    }
  ```

  El size minimo de un bloque es de 16 esto es porque tenemos como minimo 4 bytes por header 4 bytes por footer y 8 bytes de payload/padding para que este alineado a 16. 

  Tenemos el prologo como el inicio, y el epilogo como el final de la lista. En la primera llamada a mem_sbrk hacemos un alineamiento de 16 bytes.

  Tambien, incluimos la implementacion de 4 formas de busqueda de bloques libres, First fit, Next fit, Best fit, y Worst fit.

  #### Primitivas pedidas:

  ``` C
  static unsigned get(void* bp);

  static unsigned get_size(void* bp);

  static bool is_free(void* bp)

  static void* get_next_block(void* bp);

  static void* get_previous_block(void* bp)
  ```

  #### Inconsistencias que se pueden encontrar en el diseño:
  
  - Que el tamaño que guardan el Header, y el Footer sean diferente. Esto se puede verificar, comparando ambos valores, si se da que son diferentes, se encuentra la inconsistencia.
  - Que el estado del bloque, que almacenan el Header, y el Footer sean diferentes. Este se verifica con la misma logica que se usa para verificar el Header y Footer.
  - Que el bloque no este alineado. Si el resto de la division por 16 del tamaño, no da 0, entonces, no esta alineado.
  