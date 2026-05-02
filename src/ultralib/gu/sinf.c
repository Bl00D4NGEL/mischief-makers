#include <gu/guint.h>

static const du sPolynomial[] = {
    {0x3FF00000, 0x00000000},
    {0xBFC55554, 0xBC83656D},
    {0x3F8110ED, 0x3804C2A0},
    {0xBF29F6FF, 0xEEA56814},
    {0x3EC5DBDF, 0x0E314BFE},
};

static const du sReciprocalPi = {0x3FD45F30, 0x6DC9C883};
static const du sPiHigh = {0x400921FB, 0x50000000};
static const du sPiLow = {0x3E6110B4, 0x611A6263};
static const fu sZero = {0x00000000};

float __sinf(float x) {
    double dx, xsq, poly;
    double dn;
    int n;
    double result;
    int ix, xpt;

    ix = *(int*)&x;
    xpt = ix >> 22;
    xpt &= 0x1FF;

    if (xpt < 0xFF) {
        dx = x;

        if (xpt >= 0xE6) {
            xsq = dx * dx;

            poly = (((sPolynomial[4].d * xsq + sPolynomial[3].d) * xsq + sPolynomial[2].d) * xsq) +
                   sPolynomial[1].d;

            result = dx + ((dx * xsq) * poly);

            return (float)result;
        }

        return x;
    }

    if (xpt < 0x136) {
        dx = x;

        dn = dx * sReciprocalPi.d;

        n = ROUND(dn);
        dn = n;

        dx = dx - (dn * sPiHigh.d);
        dx = dx - (dn * sPiLow.d);

        xsq = dx * dx;

        poly = (((sPolynomial[4].d * xsq + sPolynomial[3].d) * xsq + sPolynomial[2].d) * xsq) + sPolynomial[1].d;

        result = dx + ((dx * xsq) * poly);

        if ((n & 1) == 0) {
            return (float)result;
        }

        return -(float)result;
    }

    if (x != x) {
        return __libm_qnan_f;
    }

    return sZero.f;
}
