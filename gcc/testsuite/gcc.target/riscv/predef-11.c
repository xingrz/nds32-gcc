/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc -mabi=lp64 -mcmodel=medlow" } */

int main () {

#ifndef __riscv_arch_test
#error "__riscv_arch_test"
#endif

#if __riscv_xlen != 64
#error "__riscv_xlen"
#endif

#if !defined(__riscv_i) || (__riscv_i != (2 * 1000 * 1000))
#error "__riscv_i"
#endif

#if !defined(__riscv_c) || (__riscv_c != (2 * 1000 * 1000))
#error "__riscv_c"
#endif

#if defined(__riscv_e)
#error "__riscv_e"
#endif

#if !defined(__riscv_a) || (__riscv_a != (2 * 1000 * 1000))
#error "__riscv_a"
#endif

#if !defined(__riscv_m) || (__riscv_m != (2 * 1000 * 1000))
#error "__riscv_m"
#endif

#if !defined(__riscv_f) || (__riscv_f != (2 * 1000 * 1000))
#error "__riscv_f"
#endif

#if !defined(__riscv_d) || (__riscv_d != (2 * 1000 * 1000))
#error "__riscv_d"
#endif

  return 0;
}
