/* { dg-do compile } */
/* { dg-options "-march=rv32ifv -mabi=ilp32f" } */
int main () {

#ifndef __riscv_arch_test
#error "__riscv_arch_test"
#endif

#if !defined(__riscv_zfh) || (__riscv_zfh != (1 * 1000000))
#error "__riscv_zfh"
#endif

  return 0;
}

