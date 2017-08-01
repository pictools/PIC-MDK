#ifndef PICMDK_INPUT_H
#define PICMDK_INPUT_H


#include "Handler.h"
#include "Utility.h"
#include "Vector.h"


namespace picmdk {

// Everything in the namespace interface defines interfaces required by picmdk
// Implementations of adapters generally should not inherit these class,
// but should implement compatible interface to be used via templates.
namespace interface {

class Input {
public:

    // Get a free variable
    template<typename ResultType>
    ResultType get(const std::string& name);

    // Get a module/handler variable
    template<typename ResultType>
    ResultType get(const Handler& handler, const std::string& name);

    // Get or try to get a free variable depending on value of isRequired
    template<typename ResultType>
    void get(const std::string& name, ResultType& value, bool isRequired);

    // Get or try to get a module/handler variable depending on value of isRequired
    template<typename ResultType>
    void get(const Handler& handler, const std::string& name, ResultType& value);

};

} // namespace picmdk::interface
} // namespace picmdk


#endif

