/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc_zba -mabi=lp64 -O2" } */

long test_1(long a, long b)
{
  return a + (b << 1);
}
long test_2(long a, long b)
{
  return a + (b << 2);
}
long test_3(long a, long b)
{
  return a + (b << 3);
}

/* { dg-final { scan-assembler-times "sh1add" 1 } } */
/* { dg-final { scan-assembler-times "sh2add" 1 } } */
/* { dg-final { scan-assembler-times "sh3add" 1 } } */
