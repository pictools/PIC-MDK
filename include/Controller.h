#ifndef PICMDK_CONTROLLER_H
#define PICMDK_CONTROLLER_H


#include "Communicator.h"
#include "ComputationLog.h"
#include "Handler.h"
#include "OpenMPWrapper.h"
#include "Vector.h"

#include <vector>


namespace picmdk {


class HandlerInitializer {
public:

    template<class Handler, class Input>
    static Handler* createHandler(Input* input, ComputationLog* computationLog,
        InterData* interData, Communicator* communicator, typename Handler::Data* data)
    {
        Handler* handler = new Handler;
        ///handler->input = input;
        ///handler->computationLog = computationLog;
        ///handler->interData = interData;
        ///handler->communicator = communicator;
        ///handler->data = data;
        return handler;
    }  
    
};

template<class Adapter>
class Controller {
public:

    // Types to use in modules
    typedef typename Adapter::Cell Cell;
    typedef typename Adapter::Grid::CellIterator CellIterator;
    typedef typename Adapter::Ensemble Ensemble;
    typedef typename Adapter::Grid Grid;
    typedef typename Adapter::Input Input;
    typedef Vector3<int> Int3;
    typedef InterData InterData;
    typedef typename Adapter::Particle Particle;
    typedef typename Adapter::Ensemble::ParticleIterator ParticleIterator;
    typedef typename Adapter::Position Position;
    typedef typename Adapter::Real Real;
    typedef typename Adapter::Real3 Real3;

    typedef typename HandlerImplementation<Controller>::Data Data;

    // Constructor with the given communicator
    Controller(Data _data, Communicator *_communicator, Input _input):
        data(_data),
        communicator(_communicator),
        input(_input),
        computationLog(ComputationLog::getInstance())
    {}

    // Adding handlers, these are called automatically for modules declared with PICMDK_MODULE
    template<class ParticleHandler>
    void addParticleHandler()
    {
       PICMDK_THROW(NotImplementedException, ("Particle handlers are not currently implemented"));
    }
    template<class CellHandler>
    void addCellHandler()
    {
        PICMDK_THROW(NotImplementedException, ("Cell handlers are not currently implemented"));
    }

    template<class DomainHandler>
    void addDomainHandler()
    {
        DomainHandler* handler = HandlerInitializer::createHandler<DomainHandler, Input>(
            &input, &computationLog, &interData, communicator, &data);
        handler->init();
        handler->registerFunctions(*this);
        domainHandlers.push_back(handler);
    }

    template<class OutputHandler>
    void addOutputHandler()
    {
        OutputHandler* handler = new OutputHandler;
        handler->init();
        outputHandlers.push_back(handler);
    }


    typedef void(*IterationStartEventHandlerFunction)(IterationStartEvent<Controller>&, Handler&);

    void registerIterationStartEventFunction(IterationStartEventHandlerFunction function)
    {

    }


    void startIteration(Real timeStep)
    {
        data.iteration++;
        data.timeStep = timeStep;
        data.iterationStartTime = data.iterationEndTime;
        data.iterationEndTime += data.timeStep;
    }

    void resetCurrentData(Data newData)
    {
        data = newData;
    }

    // Run handlers
    
    void runParticleHandlers(Particle& particle, const Real3& E, const Real3& B)
    {
        int threadIdx = omp_get_thread_num();
        for (size_t i = 0; i < particleHandlers.size(); i++)
            particleHandlers[i][threadIdx]->handle(particle, E, B);
    }
    
    
    void runCellHandlers(Cell& cell)
    {
        int threadIdx = omp_get_thread_num();
        for (size_t i = 0; i < cellHandlers.size(); i++)
            cellHandlers[i][threadIdx]->handle(cell);
    }

    void runDomainHandlers(Ensemble& ensemble, Grid& grid)
    {
        for (size_t i = 0; i < domainHandlers.size(); i++)
            domainHandlers[i]->handle(ensemble, grid);
    }

    void runOutputHandlers()
    {
        for (size_t i = 0; i < outputHandlers.size(); i++)
            outputHandlers[i]->handle();
    }


private:

    Communicator* communicator;
    Input input;
    InterData interData;
    ComputationLog& computationLog;
    Data data;

    std::vector<std::vector<ParticleHandler<Controller>*> > particleHandlers;
    std::vector<std::vector<CellHandler<Controller>*> > cellHandlers;
    std::vector<DomainHandler<Controller>*> domainHandlers;
    std::vector<OutputHandler<Controller>*> outputHandlers;

};


} // namespace picmdk


#endif
