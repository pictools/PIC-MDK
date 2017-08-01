#include "Exception.h"

#include "Utility.h"


namespace picmdk {


Exception::Exception(const std::string& _message):
    file(NULL),
    line(0),
    message(_message)
{
}

Exception::~Exception() throw()
{
}

Exception& Exception::operator=(const Exception& e)
{
    file = e.file;
    line = e.line;
    message = e.message;
    return *this;
}

const char* Exception::getFile() const
{
    return file;
}

long Exception::getLine() const
{
    return line;
}

void Exception::setLocation(const char* _file, long _line)
{
    file = _file;
    line = _line;
}

const char* Exception::what() const throw()
{
    return message.c_str();
}

std::ostream& operator<<(std::ostream& os, const Exception& e)
{
    os << e.getName() << ": " << e.what() << "\n";
    if (e.getFile())
        os << " at " << e.getFile() << ":" << e.getLine() << "\n";
    else
        os << " at unknown location\n";
    return os;
}


NamedException::NamedException(const std::string& message, const char* _name) :
    Exception(message),
    name(_name)
{
}

NamedException::~NamedException() throw()
{
}

const char* NamedException::getName() const
{
    return name;
}


OutOfMemoryException::OutOfMemoryException(std::size_t bytes):
    NamedException("Unable to allocate " + picmdk::toString((int)bytes) + " byte(s) of memory", "out of memory exception")
{
}

OutOfMemoryException::~OutOfMemoryException() throw()
{
}


NotImplementedException::NotImplementedException(const std::string& message):
    NamedException(message, "not implemented exception")
{
}

NotImplementedException::~NotImplementedException() throw()
{
}


} // namespace picmdk
