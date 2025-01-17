/* Test promotion of __fp16 to double as arguments to unprototyped
   function in another compilation unit.  */

/* { dg-do run { target riscv_zfh_hw } } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mzfh" } */
/* { dg-additional-sources "fp16-unprototyped-2.c" } */

#include <stdlib.h>

extern int f ();

static __fp16 x = 42.0;
static __fp16 y = -42.0;

int
main (void)
{
  if (!f (x, y))
    abort ();
  return 0;
}
