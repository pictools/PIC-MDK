#ifndef PICMDK_GRID_H
#define PICMDK_GRID_H


#include "Cell.h"


namespace picmdk {

// Everything in the namespace interface defines interfaces required by picmdk
// Implementations of adapters generally should not inherit these class,
// but should implement compatible interface to be used via templates.
namespace interface {


class Grid {
public:

    class CellIterator {
    public:
        // Construction and assignment
        CellIterator();
        CellIterator(const CellIterator& other);
        CellIterator& operator=(const CellIterator& other);

        // Access to Particle by iterator
        Cell& operator*();
        Cell* operator->();

        // Comparison
        bool operator==(const CellIterator& other) const;
        bool operator!=(const CellIterator& other) const;

        // Increment
        CellIterator& operator++();
        CellIterator operator++(int);
    };

    class CellConstIterator {
    public:
        // Construction and assignment
        CellConstIterator();
        CellConstIterator(const CellConstIterator& other);
        CellConstIterator& operator=(const CellConstIterator& other);

        // Access to Particle by iterator
        const Cell& operator*();
        const Cell* operator->();

        // Comparison
        bool operator==(const CellConstIterator& other) const;
        bool operator!=(const CellConstIterator& other) const;

        // Increment
        CellConstIterator& operator++();
        CellConstIterator operator++(int);
    };

    // Range for iterator traversal
    CellIterator begin();
    CellIterator end();
    CellConstIterator cbegin() const;
    CellConstIterator cend() const;

    // Get number of cells
    int size() const;

    /*
    Example:
    double fieldEnergy = 0.0;
    for (CellIterator cell = grid.begin(); cell != grid.end(); cell++) {
        fieldEnergy += cell->Ex() * cell->Ex() + cell->Ey() * cell->Ey() + cell->Ez() * cell->Ez() +
            cell->Bx() * cell->Bx() + cell->By() * cell->By() + cell->Bz() * cell->Bz();
    }

    */

    // Get result of interpolation of grid values into the given position.
    // Interpolation does not have to be done with the same scheme as for particle push,
    // there routines are supposed to be used for output and diagnostics
    Real getBx(Position position) const;
    Real getBy(Position position) const;
    Real getBz(Position position) const;
    Real3 getB(Position position) const;
    Real getEx(Position position) const;
    Real getEy(Position position) const;
    Real getEz(Position position) const;
    Real3 getE(Position position) const;
    Real getJx(Position position) const;
    Real getJy(Position position) const;
    Real getJz(Position position) const;
    Real3 getJ(Position position) const;
    Real getRho(Position position) const;
    void getField(Position position, Real3& B, Real3& E) const;
};


} // namespace picmdk::interface
} // namespace picmdk


#endif

