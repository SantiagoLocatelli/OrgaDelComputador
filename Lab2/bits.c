/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
//1
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  // Basicamente aplique la Ley de Morgan.
  return ~(~x | ~y);
}
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y){
  // ~(x & y) Da cero cuando x e y tienen 1 en el mismo lugar.
  // (x | y) va a dar cero cuando x e y tiene 0 en el mismo lugar. Y si le aplicas Morgan queda ~(~x & ~y).
  return (~(~x & ~y)) & ~(x & y);
}
//2
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  // No creo que haya mucho que explicar, es muy automatico, negas y sumas 1.
  return ~x + 1;
}
/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {

  int bits_m = m << 3; // Cantidad de bits a mover para poscionarse en el byte m.
  int bits_n = n << 3; // Cantidad de bits a mover para poscionarse en el byte n.
  int mask_m;
  int mask_n;

  mask_m = (x & (0xFF << bits_m)) >> bits_m; // Copio el byte m y lo dejo en el byte 0.
  mask_m = (0xFF & mask_m) << bits_n; // Dejo el byte copiado en el byte n.

  mask_n = (x & (0xFF << bits_n)) >> bits_n; // Copio el byte n y lo dejo en el byte 0.
  mask_n = (0xFF & mask_n) << bits_m; // Dejo el byte copiado en el byte m.

  x = x & ~(0xFF << bits_m); // Pongo a cero el byte m.
  x = x & ~(0xFF << bits_n); // Pongo a cero el byte n.

  return x | mask_m | mask_n; // Union de todos los resultados.
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */

 // 1010 1010 1010 1010 1010 1010 1010 1010
int allOddBits(int x) {
  int mask = 0xAA;
  mask = mask | mask << 8;
  mask = mask | mask << 16;
  // Cree la mascara con 1s en los bits impares.

  x = x & mask; // Si x tiene 1s en los bits impares entonces x quedara igual a mask.
  x = x ^ mask; // Todo numero que XOR consigo mismo dara cero, entonces si x es igual a mask dara cero.

  return !x;
}
//3
/*
 * bitMask - Generate a mask consisting of all 1's
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {

  int lower_mask = 0x1 << 31; // 0x80000000.
  int upper_mask = 0x1 << 31; // 0x80000000.

  int n_bits = 31 + (~lowbit + 1); // como no se puede restar, le sumo sumo complemento a 2. Consigo la distancia del lowbit al bit 31.
  lower_mask = lower_mask >> n_bits; // Lleno de 1s desde el lowbit al bit 31.

  n_bits = 31 + (~highbit + 1); // Consigo la distancia del highbit al bit 31.
  upper_mask = ~((upper_mask >> n_bits) << 1); // Lleno de 1s desde el bit cero al highbt.

  return upper_mask & lower_mask;
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {

  int value = !!x; // Pongo en 1 o en 0 el valor de x. Si x es falso queda en cero, sino en 1.
  value = ~value + 1; // Si es cero, queda en cero, si es 1 pasa a ser 0xFFFFFFFF.

  return (value & y) | (~value & z);
}
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {

  int sign_bit = 1 << 31; // 0x80000000
  int lower_rank = ~0x30;  // 0xFFFFFFCF = 0xFFFFFFFF - 0x30
  int upper_rank = ~(sign_bit | 0x39); // 0x7FFFFFC6 = 0x7FFFFFFF - 0x39

  upper_rank = !!(sign_bit & (upper_rank + x));
  // si x es mayor a 0x39 upper_rank queda en 1, ya que se excede de los positivos que puede representar y el bit de signo de x queda en 1
  // en caso de que x sea menor a 0x39 upper_rank queda en cero.

  lower_rank = !!(sign_bit & (lower_rank+1+x));
  // misma logica que la linea de arriba, pero verificando el caso contrario, si x es menor a 0x30 lower_rank queda en 1,
  // en el caso contrario sucede un OVERFLOW y lower_rank queda en cero

  return !( lower_rank | upper_rank ); // si alguno de los dos casos dio 1, significa que no esta entre 0x39 y 0x30
}
//4
/*
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int isNonZero(int x) {

  // Cualquier numero, menos 0x80000000 y 0 (en ambos casos la operacion da el mismo numero), al hacer OR con su complemento a 2 da 0xFFFFFFFF
  x = x | (~x + 1);
  return (x >> 31) & 1; // Y para el caso de que sea 0x80000000 se el corrimiento >> 31.
}
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
  // Misma logica que isNonZero solo que se niega al hacer OR con su complemento a 2.
  x = ~(x | (~x + 1));
  return (x >> 31) & 1;
}
//float
/*
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {

  int frac_mask = ~(0x1FF << 23); // 0x7FFFFF.
  int e_mask = 0xFF << 23; // 0x7F800000.

  // Se verifica el caso NaN, es decir si el exp = 0x7F800000 y si la frac = 0x7FFFFF.
  if( ((e_mask & uf) == e_mask) && (uf & frac_mask) )
    return uf;

  return (0x1 << 31) ^ uf; // Cambio el bit del signo.
}
/*
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {

  int e_mask = 0xFF << 23; // 0x7F800000.
  int sign_mask = ~(0x1 << 31); // 0x7FFFFFFF.

  // Verifica si el argumento pasado es Nan.
  if( (uf & sign_mask) > e_mask )
    return uf;

  return uf & sign_mask; // Pone el bit del signo a cero.
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {

  int e_mask;
  // Verifico que el numero no sea demasiado grande.
  if(x > 127)
    return 0xFF << 23;

  // Verifico que el numero no sea demasiado chico.
  if( x < -126 )
    return 0;

  // Hago el corrimiento necesario para lograr el 2.0^x.
  e_mask = (127 + x) << 23;

  return e_mask;
}
