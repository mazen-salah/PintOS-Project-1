#ifndef __THREAD_FIXED_POINT_H
#define __THREAD_FIXED_POINT_H

typedef int fixed_t;

#define AmountShiftFixedPoint 16

#define ConstFixedPoint(A) ((fixed_t)(A << AmountShiftFixedPoint))

#define AddFixedPoint(A, B) (A + B)

#define MixAddFixedPoint(A, B) (A + (B << AmountShiftFixedPoint))

#define SubFixedPoint(A, B) (A - B)

#define MixSubFixedPoint(A, B) (A - (B << AmountShiftFixedPoint))

#define MultMixFixedPoint(A, B) (A * B)

#define MixDivFixedPoint(A, B) (A / B)

#define MultFixedPoint(A, B) ((fixed_t)(((int64_t)A) * B >> AmountShiftFixedPoint))

#define DivFixedPoint(A, B) ((fixed_t)((((int64_t)A) << AmountShiftFixedPoint) / B))

#define IntPartFixedPoint(A) (A >> AmountShiftFixedPoint)

#define RoundFixedPoint(A) (A >= 0 ? ((A + (1 << (AmountShiftFixedPoint - 1))) >> AmountShiftFixedPoint) \
							: ((A - (1 << (AmountShiftFixedPoint - 1))) >> AmountShiftFixedPoint))

#endif