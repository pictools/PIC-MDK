#ifndef PICMDK_HANDLER_H
#define PICMDK_HANDLER_H

#include "Communicator.h"
#include "ComputationLog.h"
#include "InterData.h"
#include "Utility.h"

#include <iostream>
#include <string>


namespace picmdk {
    
// Base class for all handlers.
// Module developers should not inherit it directly, but instead 
// publicly inherit DomainHandler, ParticleHandler, CellHandler or OutputHandler.
class Handler {
public:   

    enum Type { HandlerType_Cell, HandlerType_Domain, HandlerType_Output,
        HandlerType_Particle, HandlerType_Dummy };

    Handler(Type type);
    virtual ~Handler();

    // Initialize members of the handler.
    // Data sets operated by InterData must be initialized here.
    virtual void init();

    // Return type of the handler
    Type getType() const;

    // Serialization of the internal state of the handler.
    virtual void save(std::ostream& f);
    virtual void load(std::istream& f);

    // Name getters: handler and module names are unique for each handler/module class,
    // instance names are unique for each object of a class
    // These should generally not be implemented in the child classes, left virtual just in case.
    virtual std::string getHandlerName() const;
    virtual std::string getHandlerInstanceName() const;
    virtual std::string getModuleName() const;
    virtual std::string getModuleInstanceName() const;

protected:

    // These are protected for user-written handlers to set the values when convenient.
    // Another way would be to implement the virtual get...Name() methods.
    std::string handlerName, handlerInstanceName;
    std::string moduleName, moduleInstanceName;

private:

    Type type;
    // Copy and assignment for handlers are forbidden
    Handler(const Handler&);
    Handler& operator=(const Handler&);

};

// This is a helper base class for DomainHandler, ParticleHandler, CellHandler and OutputHandler.
// It should not be inherited directly.
template<class Controller>
class HandlerImplementation : public Handler {
public:

    typedef Controller ControllerType;

    // Types to be use in handlers, alphabetically.
    // The interface of each type is given at picmdk::interface::[TypeName]
    typedef typename Controller::Cell Cell;
    typedef typename Controller::CellIterator CellIterator;
    typedef typename Controller::Ensemble Ensemble;
    typedef typename Controller::Grid Grid;
    typedef typename Controller::Input Input;
    typedef typename Controller::Int3 Int3;
    typedef typename Controller::InterData InterData;
    typedef typename Controller::Particle Particle;
    typedef typename Controller::ParticleIterator ParticleIterator;
    typedef typename Controller::Position Position;
    typedef typename Controller::Real Real;
    typedef typename Controller::Real3 Real3;
  
    // Current parameters of simulation to use in handlers
    struct Data {
        Real iterationStartTime, iterationEndTime; // time at the start and end of the current time iteration
        Real timeStep; // endTime - startTime; could change between time iterations
        Real simulationStartTime, simulationEndTime; // start and end time for whole simulation
        int iteration; // index of the current iteration on time; do not rely on iteration * timeStep == startTime - simulationStartTime
        int numIterations; // number of iterations on time; do not rely on numIterations * timeStep == simulationEndTime - simulationStartTime
        Position globalMin, globalMax; // global simulation area limits
        Position localMin, localMax; // local simulation area limits (for a current domain)

        Data():
            iterationStartTime((Real)0),
            iterationEndTime((Real)0),
            timeStep((Real)0),
            iteration(0),
            numIterations(0),
            simulationStartTime((Real)0),
            simulationEndTime((Real)0)
        {
        }
    };

    HandlerImplementation(Type type): 
        Handler(type)
    {}

    virtual void registerFunctions(Controller& controller) = 0;

protected:

    // Data available for user-written handlers
    const Data* data;
    Input* input;
    ComputationLog* computationLog;
    InterData* interData;
    Communicator* communicator;

};


// Base class for all events used at handlers
class Event {
public:
    virtual ~Event() {}
    virtual std::string getName() const = 0;
};

template<class Controller>
class IterationStartEvent : public Event {
public:
    typedef typename Controller::Ensemble Ensemble;
    typedef typename Controller::Grid Grid;

    IterationStartEvent(Ensemble& _ensemble, Grid& _grid) :
        ensemble(_ensemble),
        grid(_grid)
    {
    }

    virtual std::string getName() const { return "Iteration start event"; }

    Ensemble& getEnsemble() { return ensemble; }
    Grid& getGrid() { return grid; }

private:

    Ensemble& ensemble;
    Grid& grid;
};



enum PortID { PortID_PreCellLoop, PortID_PostCellLoop, PortID_PreParticleLoop, PortID_PostParticleLoop };

namespace internal {
    template<class Controller>
    void domainHandlerFunction(IterationStartEvent<Controller>& event, Handler& handler)
    {
        ((DomainHandler<Controller>&)handler).handle(event.getEnsemble(), event.getGrid());
    }
} // namespace picmdk::internal

template<class Controller>
class DomainHandler : public HandlerImplementation<Controller> {
public:
    DomainHandler(): HandlerImplementation<Controller>(HandlerType_Domain) {}
    virtual void registerFunctions(Controller& controller)
    {
        controller.registerIterationStartEventFunction(&::picmdk::internal::domainHandlerFunction<Controller>);
    }
    virtual void handle(Ensemble& ensemble, Grid& grid) = 0;
};



template<class Controller>
class CellHandler : public HandlerImplementation<Controller> {
public:
    CellHandler(): HandlerImplementation<Controller>(HandlerType_Cell) {}
    virtual void handle(Cell& cell) = 0;
    virtual bool isActiveIteration() {return true; }
};

template<class Controller>
class ParticleHandler : public HandlerImplementation<Controller> {
public:
    ParticleHandler(): HandlerImplementation<Controller>(HandlerType_Particle) {}
    enum Status { Status_Default, Status_DisablePush, Status_Delete };// Status of particle after handling: continue processing as usual (default), disable particle push or delete particle

    // Process particle data, change status if necessary
    virtual void handle(Particle& particle, const Real3& E, const Real3& B, Status& status) = 0;
    virtual bool isActiveIteration() { return true; }
};


template<class Controller>
class OutputHandler : public HandlerImplementation<Controller> {
public:
    OutputHandler(): HandlerImplementation<Controller>(HandlerType_Output) {}
    virtual void handle() = 0;
};

// A dummy handler class which does nothing and is used as a placeholder for templates.
// It should not be used directly.
template<class Controller>
class DummyHandler : public HandlerImplementation<Controller> {
    DummyHandler(): HandlerImplementation<Controller>(HandlerType_Dummy) {}
};

} // namespace picmdk

#endif
