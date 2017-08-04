#include "Handler.h"

using std::string;

namespace picmdk {


Handler::~Handler()
{
}

void Handler::init()
{
}
    
void Handler::save(std::ostream& f)
{
}

void Handler::load(std::istream& f)
{
}

string Handler::getHandlerName() const
{ 
    return handlerName;
}

string Handler::getHandlerInstanceName() const
{ 
    if (handlerInstanceName != "")
        return handlerInstanceName;
    else
        return getModuleInstanceName() + "_" + getHandlerName();
}

string Handler::getModuleName() const
{
    return moduleName;
}

string Handler::getModuleInstanceName() const
{ 
    return moduleInstanceName;
}

} // namespace picmdk
