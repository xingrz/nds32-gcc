/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-march=rv64gc_zba_zbs -mabi=lp64 -O2" } */

int foo(int n, unsigned char *arr, unsigned y){
  int s = 0;
  unsigned x = 0;
  for (;x<n;x++)
    s += arr[x+y];
  return s;
}

/* { dg-final { scan-assembler "add.uw" } } */
