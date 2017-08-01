#ifndef PICMDK_HANDLERLIST_H
#define PICMDK_HANDLERLIST_H


#include "Controller.h"
#include "Handler.h"


namespace picmdk {


// A collection of handlers, given as template parameters
// Constructing a list adds the handlers to the given controller
// Parameters equal to DummyHandler have no effect
template<class HandlerClass1,
    class HandlerClass2 = DummyHandler,
    class HandlerClass3 = DummyHandler,
    class HandlerClass4 = DummyHandler,
    class HandlerClass5 = DummyHandler,
    class HandlerClass6 = DummyHandler,
    class HandlerClass7 = DummyHandler,
    class HandlerClass8 = DummyHandler,
    class HandlerClass9 = DummyHandler,
    class HandlerClass10 = DummyHandler>
class HandlerList {
public:

    HandlerList(typename HandlerClass1::ControllerType& controller)
    {
        addHandler<HandlerClass1>(controller);
        addHandler<HandlerClass2>(controller);
        addHandler<HandlerClass3>(controller);
        addHandler<HandlerClass4>(controller);
        addHandler<HandlerClass5>(controller);
        addHandler<HandlerClass6>(controller);
        addHandler<HandlerClass7>(controller);
        addHandler<HandlerClass8>(controller);
        addHandler<HandlerClass9>(controller);
        addHandler<HandlerClass10>(controller);
    }

private:

    template<class HandlerClass>
    void addHandler(Controller* controller)
    {
        HandlerClass* handler = 0; // just for overloading to detect type
        addHandler<HandlerClass>(controller, handler);
    }

    template<class HandlerClass>
    void addHandler(Controller* controller, OutputHandler* handler)
    {
        controller->addOutputHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller* controller, DomainHandler* handler)
    {
        controller->addDomainHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller* controller, CellHandler* handler)
    {
        controller->addCellHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller* controller, ParticleHandler* handler)
    {
        controller->addParticleHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller* controller, DummyHandler* handler)
    {
    }

};


} // namespace picmdk


#endif
