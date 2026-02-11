/* File: bitOps.c
 * Author: Denis Myer
 * Last Edited: Gary Hubley
 * Date: 2026/02/04
 * Description: DEMONSTRATION OF BOOLEAN AND SHIFT, BIT OPERATIONS
 */

#include <stdio.h>

int main() {
  unsigned char A, B, C, D, E, F;
  A = 0x34;  // 0011 0100 //
  B = 0x27;  // 0010 0111 //
  C = A & B; // 0010 0100 //
  D = A | B; // 0011 0111 //
  E = A ^ B; // 0001 0011 //
  F = ~A;    // 1100 1011 //
  printf("BOOLEAN OPERATIONS\n");
  printf("A = (0011 0100) = %x\n", A);
  printf("B = (0010 0111) = %x\n", B);
  printf("A AND B (0010 0100) = %x\n", C);
  printf("A OR B (0011 0111) = %x\n", D);
  printf("A XOR B (0001 0011) = %x\n", E);
  printf("NOT A (1100 1011) = %x\n", F);
  printf("SHIFT OPERATIONS\n");
  A = 0x34;   // 0011 0100 //
  B = A << 1; // 0110 1000Z ZERO FILL //
  C = A >> 2; // 0000 1101 //
  printf("A = (0011 0100) = %x\n", A);
  printf("SHIFT LEFT ONE (0110 1000) = %x\n", B);
  printf("SHIFT RIGHT TWO (0000 1101) = %x\n", C);

  return (0);
}
