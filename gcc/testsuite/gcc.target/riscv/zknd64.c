/* { dg-do compile } */
/* { dg-require-effective-target riscv64 } */
/* { dg-options "-O2 -march=rv64gc_zknd -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
int64_t foo1(int64_t rs1, int64_t rs2)
{
    return _rv64_aes64ds(rs1,rs2);
}

int64_t foo2(int64_t rs1, int64_t rs2)
{
    return _rv64_aes64dsm(rs1,rs2);
}

int64_t foo3(int64_t rs1, int rnum)
{
    rnum = 8;
    return _rv64_aes64ks1i(rs1,rnum);
}

int64_t foo4(int64_t rs1, int64_t rs2)
{
    return _rv64_aes64ks2(rs1,rs2);
}

int64_t foo5(int64_t rs1)
{
    return _rv64_aes64im(rs1);
}

/* { dg-final { scan-assembler-times "aes64ds" 2 } } */
/* { dg-final { scan-assembler-times "aes64dsm" 1 } } */
/* { dg-final { scan-assembler-times "aes64ks1i" 1 } } */
/* { dg-final { scan-assembler-times "aes64ks2" 1 } } */
/* { dg-final { scan-assembler-times "aes64im" 1 } } */