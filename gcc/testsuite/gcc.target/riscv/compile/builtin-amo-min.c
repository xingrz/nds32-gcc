/* Verify that amomin.w instruction is generated by builtin function.  */

/* { dg-do compile } */
/* { dg-require-effective-target riscv_atomic } */
/* { dg-final { scan-assembler "\tamomin.w\t" } } */

#include <nds_intrinsic.h>

void
func (void)
{
  /* We want to perform an atomic min operation.  */
  signed int data, cmpv, oldv;
  cmpv = 10;
  /* new value: minimum (data, cmpv)  */
  oldv = __nds__amominw (cmpv, &data, UNORDER);
}
