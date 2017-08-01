#ifndef PICMDK_TYPES_H
#define PICMDK_TYPES_H


#include "../Utility/Vector.h"


namespace picmdk {

// Everything in the namespace interface defines interfaces required by picmdk
// Implementations of adapters generally should not inherit these class,
// but should implement compatible interface to be used via templates.
namespace interface {

// Real number (normally float or double)
typedef double Real;

// Vector of 3 values of real numbers
typedef Vector3<Real> Real3;

// Vector of 3 values of real numbers
typedef Real3 Position;

} // namespace picmdk::interface
} // namespace picmdk


#endif

