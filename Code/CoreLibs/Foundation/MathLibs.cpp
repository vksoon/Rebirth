#include "MathLibs.h"

RB_NAMESPACE_BEGIN

float RMath::BIGGEST_FLOAT_SMALLER_THAN_ONE = 0.99999994f;
float RMath::POS_INFINITY = std::numeric_limits<float>::infinity();
float RMath::NEG_INFINITY = -std::numeric_limits<float>::infinity();
float RMath::PI = 3.14159265358979323846f;
float RMath::TWO_PI = (float)(2.0f * PI);
float RMath::HALF_PI = (float)(0.5f * PI);
float RMath::QUARTER_PI = (float)(0.25f * PI);
float RMath::INV_PI = (float)(1 / PI);
float RMath::INV_HALF_PI = (float)(INV_PI / 2);
float RMath::INV_TWO_PI = (float)(2.0f * INV_PI);
float RMath::DEG2RAD = PI / 180.0f;
float RMath::RAD2DEG = 180.0f / PI;
float RMath::SQRT2 = 1.4142135623730951f;
float RMath::INV_SQRT2 = (float)(1.0f / SQRT2);

RB_NAMESPACE_END