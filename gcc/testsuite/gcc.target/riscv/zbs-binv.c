/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc_zbs -mabi=lp64 -O2" } */

/* binv */
long
foo0 (long i, long j)
{
  return i ^ (1L << j);
}

/* binvi */
long
foo1 (long i)
{
  return i ^ (1L << 20);
}

/* { dg-final { scan-assembler-times "binv\t" 1 } } */
/* { dg-final { scan-assembler-times "binvi\t" 1 } } */
/* { dg-final { scan-assembler-not "andi" } } */
