#include <stdint.h>
#include <math.h>
//Number of bits to use in the fraction part
#ifndef FIXBFRAC
#define FIXBFRAC 16
#endif
typedef int32_t fixed; //16 bit int, 16 bit frac
typedef int64_t ltfixed; //the size exactly double that of the fixed size.
//#define MULFIXEDMEDPREC(a,b) (((a>>(FIXBFRAC/4)) * (b>>(FIXBFRAC/4)))>>(FIXBFRAC/2))
//#define MULFIXEDLOWPREC(a,b) ((a>>(FIXBFRAC/2)) * (b>>(FIXBFRAC/2)))
#define MULFIXEDHIGHPREC(a,b) ((fixed)(((ltfixed)a * (ltfixed)b)>>FIXBFRAC))
#define MULFIXED MULFIXEDHIGHPREC
#define DIVFIXEDHIGHPREC(a,b) (((ltfixed)a<<FIXBFRAC)/(b))
//#define DIVFIXEDLOWPREC(a,b) ((a<<(FIXBFRAC/2))/(b<<(FIXBFRAC/2)))
//#define DIVFIXEDMEDPREC(a,b) (((a<<(FIXBFRAC/4))/(b<<(FIXBFRAC/4)))<<(FIXBFRAC/2))
#define DIVFIXED DIVFIXEDHIGHPREC
#define FIXEDTODOUBLE(a) ((double)a / (double)(1<<FIXBFRAC))
#define DOUBLETOFIXED(a) ((fixed)(a * (double)(1<<FIXBFRAC)))


#define FIXEDTOFLOAT(a) ((float)a / (float)(1<<FIXBFRAC))
#define FLOATTOFIXED(a) ((fixed)(a * (float)(1<<FIXBFRAC)))
