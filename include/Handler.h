#ifndef PICMDK_HANDLER_H
#define PICMDK_HANDLER_H


#include "Communicator.h"
#include "ComputationLog.h"
#include "Event.h"
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

    enum Type { Cell, Domain, Output, Particle, Dummy };

    Handler() {}
    virtual ~Handler() {}

    // Initialize members of the handler.
    // Data sets operated by InterData must be initialized here.
    virtual void init() {}

    // Return type of the handler
    virtual Type getType() const = 0;

    // Serialization of the internal state of the handler.
    virtual void save(std::ostream& f) {}
    virtual void load(std::istream& f) {}

    // Name getters: handler and module names are unique for each handler/module class,
    // instance names are unique for each object of a class
    // These should generally not be implemented in the child classes, left virtual just in case.
    virtual std::string getHandlerName() const = 0;
    virtual std::string getHandlerInstanceName() const = 0;
    virtual std::string getModuleName() const = 0;
    virtual std::string getModuleInstanceName() const = 0;

private:

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

    virtual void registerFunctions(Controller& controller) = 0;

    virtual std::string getHandlerName() const { return handlerName; }
    virtual std::string getHandlerInstanceName() const {
        if (handlerInstanceName != "")
            return handlerInstanceName;
        else
            return getModuleInstanceName() + "_" + getHandlerName();
    }
    virtual std::string getModuleName() const { return moduleName; }
    virtual std::string getModuleInstanceName() const { return moduleInstanceName; }

protected:

    // Data available for user-written handlers
    const Data* data;
    Input* input;
    ComputationLog* computationLog;
    InterData* interData;
    Communicator* communicator;

    std::string handlerName, handlerInstanceName;
    std::string moduleName, moduleInstanceName;

    friend class HandlerInitializer;

};


template<class Controller>
class DomainHandler : public HandlerImplementation<Controller> {
public:
    virtual Handler::Type getType() const { return Handler::Domain; }
    virtual void registerFunctions(Controller& controller)
    {
        controller.registerHandlerFunction(
            &::picmdk::internal::domainHandlerFunction<Controller>, Event::IterationStart, this);
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

    // Process particle data, change status if necessary
    virtual void handle(Particle& particle, const Real3& E, const Real3& B) = 0;
    virtual bool isActiveIteration() { return true; }
};


template<class Controller>
class OutputHandler : public HandlerImplementation<Controller> {
public:
    OutputHandler(): HandlerImplementation<Controller>(HandlerType_Output) {}
    virtual void registerFunctions(Controller& controller)
    {
        controller.registerHandlerFunction(
            &::picmdk::internal::outputHandlerFunction<Controller>, Event::Output, this);
    }
    virtual void handle() = 0;
};


namespace internal {

template<class Controller>
void domainHandlerFunction(Event& event, Handler& handler)
{
    IterationStartEvent<Controller>& e = (IterationStartEvent<Controller>&)event;
    ((DomainHandler<Controller>&)handler).handle(e.getEnsemble(), e.getGrid());
}

template<class Controller>
void particleHandlerFunction(Event& event, Handler& handler)
{
    ParticlePostPushEvent<Controller>& e = (ParticlePostPushEvent<Controller>&)event;
    ((ParticleHandler<Controller>&)handler).handle(e.getParticle(), e.getEnsemble(), e.getGrid());
}

template<class Controller>
void cellHandlerFunction(Event& event, Handler& handler)
{
    CellEvent<Controller>& e = (CellEvent<Controller>&)event;
    ((CellHandler<Controller>&)handler).handle(e.getCell(), e.getGrid());
}

template<class Controller>
void outputHandlerFunction(Event& event, Handler& handler)
{
    ((OutputHandler<Controller>&)handler).handle();
}

} // namespace picmdk::internal


// A dummy handler class which does nothing and is used as a placeholder for templates.
// It should not be used directly.
template<class Controller>
class DummyHandler : public HandlerImplementation<Controller> {
    DummyHandler(): HandlerImplementation<Controller>(HandlerType_Dummy) {}
};

} // namespace picmdk

#endif
