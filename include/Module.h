#ifndef PICMDK_MODULE_H
#define PICMDK_MODULE_H


#include "Controller.h"
#include "Handler.h"

#include <string>


namespace picmdk {


// Base class for modules.
// A module is basically a set of handlers with a common name
template<class Controller>
class Module {
public:

    virtual ~Module() {}

    // Get a unique module text name
    virtual std::string getName() const = 0;

    // Set a module instance text name
    virtual void setInstanceName(const std::string& name) = 0;

    // Get a module instance text name, unique for this module
    virtual std::string getInstanceName() const = 0;

    // Add all handlers to the controller
    virtual void addHandlers(Controller& controller) = 0;
};


// A collection of handlers, given as template parameters
// Constructing a list adds the handlers to the given controller
// Parameters equal to DummyHandler have no effect
template<class Controller, class HandlerClass1,
    class HandlerClass2 = DummyHandler<Controller>,
    class HandlerClass3 = DummyHandler<Controller>,
    class HandlerClass4 = DummyHandler<Controller>,
    class HandlerClass5 = DummyHandler<Controller>,
    class HandlerClass6 = DummyHandler<Controller>,
    class HandlerClass7 = DummyHandler<Controller>,
    class HandlerClass8 = DummyHandler<Controller>,
    class HandlerClass9 = DummyHandler<Controller>,
    class HandlerClass10 = DummyHandler<Controller> >
class ModuleImplementation: public Module<Controller> {
public:

    virtual void setInstanceName(const std::string& name)
    {
        instanceName = name;
    }

    virtual std::string getInstanceName() const
    {
        return instanceName;
    }

    virtual void addHandlers(Controller& controller)
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
    void addHandler(Controller& controller)
    {
        HandlerClass* handler = 0; // just for overloading to detect type
        addHandler<HandlerClass>(controller, handler);
    }

    template<class HandlerClass>
    void addHandler(Controller& controller, OutputHandler<Controller>* handler)
    {
        controller.addOutputHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller& controller, DomainHandler<Controller>* handler)
    {
        controller.addDomainHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller& controller, CellHandler<Controller>* handler)
    {
        controller.addCellHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller& controller, ParticleHandler<Controller>* handler)
    {
        controller.addParticleHandler<HandlerClass>();
    }

    template<class HandlerClass>
    void addHandler(Controller& controller, DummyHandler<Controller>* handler)
    {
    }

    std::string instanceName;

};

} // namespace picmdk

#endif