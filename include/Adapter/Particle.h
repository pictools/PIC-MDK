#ifndef PICMDK_PARTICLE_H
#define PICMDK_PARTICLE_H


#include "Types.h"


namespace picmdk {

// Everything in the namespace interface defines interfaces required by picmdk
// Implementations of adapters generally should not inherit these class,
// but should implement compatible interface to be used via templates.
namespace interface {


// Class to access particle data.
// Note that this class may, but does not have to actually store the particle data,
// instead it could only store an index or pointer to the particle storage and redirect all the calls there.
// All units are in CGS.
class Particle {
public:
    Particle();
    Particle(const Position& position, const Real3& momentum, int typeIndex, Real factor);
    
    Real mass() const;
    Real charge() const;
    
    int getType() const;
    void setType(int newType);

    const Position getPosition() const;
    void setPosition(const Position& newPosition);

    const Real3 getMomentum() const;
    void setMomentum(const Real3& newMomentum);

    const Real3 getVelocity() const;
    void setVelocity(const Real3& newVelocity);

    Real gamma() const;
    
    Real getFactor() const;
    void setFactor(Real newFactor);
};


} // namespace interface
} // namespace picmdk


#endif

