/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc_zbb -mabi=lp64 -O2" } */

long
li_rori (void)
{
  return 0xffff77ffffffffffL;
}

long
li_rori_2 (void)
{
  return 0x77ffffffffffffffL;
}

long
li_rori_3 (void)
{
  return 0xfffffffeefffffffL;
}

long
li_rori_4 (void)
{
  return 0x5ffffffffffffff5L;
}

long
li_rori_5 (void)
{
  return 0xaffffffffffffffaL;
}


/* { dg-final { scan-assembler-times "rori\t" 5 } } */
