/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc_zba_zbs -mabi=lp64 -O2" } */

long
foo (long i)
{
  return (long)(unsigned int)i;
}
/* XXX: This pattern require combine improvement.   */
/* { dg-final { scan-assembler-not "slli.uw" } } */
