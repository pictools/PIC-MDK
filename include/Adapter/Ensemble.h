#ifndef PICMDK_ENSEMBLE_H
#define PICMDK_ENSEMBLE_H


#include "Particle.h"


namespace picmdk {

// Everything in the namespace interface defines interfaces required by picmdk
// Implementations of adapters generally should not inherit these class,
// but should implement compatible interface to be used via templates.
namespace interface {


class Ensemble {
public:

    class ParticleIterator {
    public:
        // Construction and assignment
        ParticleIterator();
        ParticleIterator(const ParticleIterator& other);
        ParticleIterator& operator=(const ParticleIterator& other);

        // Access to Particle by iterator
        Particle& operator*();
        Particle* operator->();

        // Comparison
        bool operator==(const ParticleIterator& other) const;
        bool operator!=(const ParticleIterator& other) const;

        // Increment
        ParticleIterator& operator++();
        ParticleIterator operator++(int);
    };

    class ParticleConstIterator {
    public:
        // Construction and assignment
        ParticleConstIterator();
        ParticleConstIterator(const ParticleConstIterator& other);
        ParticleConstIterator& operator=(const ParticleConstIterator& other);

        // Access to Particle by iterator
        const Particle& operator*();
        const Particle* operator->();

        // Comparison
        bool operator==(const ParticleConstIterator& other) const;
        bool operator!=(const ParticleConstIterator& other) const;

        // Increment
        ParticleConstIterator& operator++();
        ParticleConstIterator operator++(int);
    };

    // Range for iterator traversal
    ParticleIterator begin();
    ParticleIterator end();
    ParticleConstIterator cbegin() const;
    ParticleConstIterator cend() const;


    // Get number of particles
    int size() const;

    // Add a particle to ensemble, it will be available from the next time step
    void add(const Particle& particle);

    // Erase the particle given by iterator
    // Return iterator to the next particle
    ParticleIterator erase(ParticleIterator iterator);

    /*
    Example:
    for (ParticleIterator particle = ensemble.begin(); particle != ensemble.end(); ) {
        // Access to methods of Particle:
        FP3 coords = particle->getPosition();
        (*particle).setVelocity(...);
        // Alternatively
        Particle& currentParticle = *i;
        ... // operations with currentParticle will change the data
        
        if (...)
        {
            ...
            particle = ensemble.erase(particle);
        }
        else
            particle++;
    }
    
    */
};


} // namespace picmdk::interface
} // namespace picmdk


#endif

