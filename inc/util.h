#ifndef UTIL_H
#define UTIL_H

#define SET1(v,i) v|=1L << i
#define SET0(v,i) v&=~(1L << i)
#define read_sreg(sreg, x) asm volatile ("csrr %0, " #sreg : "=r" (x))
#define write_sreg(sreg, x) asm volatile("csrw " #sreg  ", %0" : : "r" (x))
#define read_reg(reg, x) asm volatile ("mv %0, " #reg : "=r" (x))
#define write_reg(reg, x) asm volatile("mv " #reg  ", %0" : : "r" (x))

#endif