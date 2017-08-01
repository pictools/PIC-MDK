#ifndef PICMDK_CELL_H
#define PICMDK_CELL_H


#include "Types.h"


namespace picmdk {
namespace interface {


// Interface for Cell class to use in modules.
// This file should not be included to anywhere, it is left as documentation
// Adapter must implement a Cell class compatible with this interface
class Cell {
public:
    Real& Ex();
    Real& Ey();
    Real& Ez();
    Real& Bx();
    Real& By();
    Real& Bz();
    Real& Jx();
    Real& Jy();
    Real& Jz();
    Real& Rho();

    const Position ExPosition() const;
    const Position EyPosition() const;
    const Position EzPosition() const;
    const Position BxPosition() const;
    const Position ByPosition() const;
    const Position BzPosition() const;
    const Position JxPosition() const;
    const Position JyPosition() const;
    const Position JzPosition() const;
    const Position RhoPosition() const;

    const Position size() const;
    const Real volume() const;

};


} // namespace picmdk::interface
} // namespace picmdk


#endif

