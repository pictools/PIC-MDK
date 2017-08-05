#ifndef PICMDK_MODULEINSTANTIATOR_H
#define PICMDK_MODULEINSTANTIATOR_H


#include "Module.h"

#include <string>
#include <utility>
#include <vector>


namespace picmdk {


template<class Controller>
class ModuleInstantiator {
public:
    ModuleInstantiator(Controller& controller);
    ~ModuleInstantiator();
    // Add modules to the controller according to requests is form of pairs of module and instance names
    void instantiate(const std::vector<std::pair<std::string, std::string> >& requests);

private:
    Controller& controller;
    std::vector<Module<Controller>*> modules;

    void checkModuleDuplicates();

    // Copy and assignment are forbidden
    ModuleInstantiator(const ModuleInstantiator&);
    ModuleInstantiator& operator=(const ModuleInstantiator&);
};


// Add all modules to be instantiated here
template<class Controller>
ModuleInstantiator<Controller>::ModuleInstantiator(Controller& _controller):
    controller(_controller)
{
}

template<class Controller>
ModuleInstantiator<Controller>::~ModuleInstantiator()
{
    for (int i = 0; i < modules.size(); i++)
        delete modules[i];
}

template<class Controller>
void ModuleInstantiator<Controller>::instantiate(const std::vector<std::pair<std::string, std::string> >& requests)
{
    checkModuleDuplicates();
    std::vector<std::pair<std::string, std::string> > activeRequests = requests;
    for (int i = 0; i < activeRequests.size(); i++) {
        bool duplicatesFound = false;
        for (int j = i + 1; j < activeRequests.size(); j++)
            if (activeRequests[i] == activeRequests[j])
            {
                if (!duplicatesFound)
                    controller.getComputationLog().writeWarning("Duplicate module '" + requests[i].first +
                        "' instance '" + requests[i].second + "' requested for instantiation at ModuleInstantiator, duplicates will be ignored");
                duplicatesFound = true;
                activeRequests.erase(activeRequests.begin() + j);
            }
        std::string moduleName = activeRequests[i].first;
        bool foundModule = false;
        for (int j = 0; j < modules.size(); j++)
            if (moduleName == modules[j]->getName()) {
                modules[j]->setInstanceName(activeRequests[i].second);
                controller.addModule(*modules[j]);
                foundModule = true;
                break;
            }
        if (!foundModule)
            controller.getComputationLog().writeWarning("Module '" + moduleName + "' is not registered at ModuleInstantiator, instantiation request ignored");
    }
}

template<class Controller>
void ModuleInstantiator<Controller>::checkModuleDuplicates()
{
    for (int i = 0; i < modules.size(); i++)
        for (int j = 0; j < modules.size(); j++)
            if (modules[i]->getName() == modules[j]->getName())
                controller.getComputationLog().writeWarning("Several modules with name '" +
                    modules[i]->getName() + "' registered at ModuleInstantiator, duplicates will be ignored during module instantiation");
}


} // namespace picmdk

#endif