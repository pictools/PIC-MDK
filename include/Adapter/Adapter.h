#ifndef PICMDK_ADAPTER_H
#define PICMDK_ADAPTER_H


#include "Cell.h"
#include "Grid.h"
#include "Ensemble.h"
#include "Input.h"
#include "Particle.h"
#include "Types.h"



// This is a stub for an adapter from PIC core to picmdk.
// It should define all types to be used in modules.
// The types should follow interfaces defined in picmdk::interface namespace.
class Adapter {
public:
    typedef picmdk::interface::Real Real;
    typedef picmdk::interface::Real3 Real3;
    typedef picmdk::interface::Real3 Position;
    typedef picmdk::interface::Particle Particle;
    typedef picmdk::interface::Ensemble Ensemble;
    typedef picmdk::interface::Grid Grid;
    typedef picmdk::interface::Input Input;
};


#endif

