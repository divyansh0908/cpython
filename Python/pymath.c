#include "Python.h"


#ifdef HAVE_GCC_ASM_FOR_X87
// Inline assembly for getting and setting the 387 FPU control word on
// GCC/x86.
#ifdef _Py_MEMORY_SANITIZER
__attribute__((no_sanitize_memory))
#endif
unsigned short _Py_get_387controlword(void) {
    unsigned short cw;
    __asm__ __volatile__ ("fnstcw %0" : "=m" (cw));
    return cw;
}

void _Py_set_387controlword(unsigned short cw) {
    __asm__ __volatile__ ("fldcw %0" : : "m" (cw));
}
#endif  // HAVE_GCC_ASM_FOR_X87


#ifndef HAVE_HYPOT
double hypot(double x, double y)
{
    double yx;

    x = fabs(x);
    y = fabs(y);
    if (x < y) {
        double temp = x;
        x = y;
        y = temp;
    }
    if (x == 0.)
        return 0.;
    else {
        yx = y/x;
        return x*sqrt(1.+yx*yx);
    }
}
#endif /* HAVE_HYPOT */

#ifndef HAVE_COPYSIGN
double
copysign(double x, double y)
{
    /* use atan2 to distinguish -0. from 0. */
    if (y > 0. || (y == 0. && atan2(y, -1.) > 0.)) {
        return fabs(x);
    } else {
        return -fabs(x);
    }
}
#endif /* HAVE_COPYSIGN */

#ifndef HAVE_ROUND
double
round(double x)
{
    double absx, y;
    absx = fabs(x);
    y = floor(absx);
    if (absx - y >= 0.5)
        y += 1.0;
    return copysign(y, x);
}
#endif /* HAVE_ROUND */
