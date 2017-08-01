#ifndef PICMDK_EXCEPTION_H
#define PICMDK_EXCEPTION_H


#include <exception>
#include <ostream>
#include <string>


namespace picmdk {


// This should normally be used for throwing derivatives of MDK::Exception (see below) to capture the location
#define PICMDK_THROW(type, args) \
    do { \
        type EXCEPTION args; \
        EXCEPTION.setLocation(__FILE__, __LINE__); \
        throw EXCEPTION; \
    } while (0)


// A base class for MDK exceptions with location information
class Exception : public std::exception {
public:
    Exception(const std::string& message);
    virtual ~Exception() throw(); // for gcc pre C++11 to match std::exception::~exception() throw()
    Exception& operator=(const Exception& e);
    
    virtual const char* getName() const = 0;
    const char* getFile() const;
    long getLine() const;
    
    void setLocation(const char* file, long line);
    const char* what() const throw();

private:
    const char* file;
    long line;
    std::string message;
};

std::ostream& operator<<(std::ostream& os, const Exception& e);


// Base exception class with a name
class NamedException : public Exception {
public:
    NamedException(const std::string& message, const char* name);
    virtual ~NamedException() throw();
    virtual const char* getName() const;
private:
    const char* name;
};


// Preferrably should be thrown in case std::bad_alloc is caught
class OutOfMemoryException : public NamedException {
public:
    OutOfMemoryException(std::size_t bytes);
    virtual ~OutOfMemoryException() throw();
};


// Preferrably should be thrown in case std::bad_alloc is caught
class NotImplementedException : public NamedException {
public:
    NotImplementedException(const std::string& message);
    virtual ~NotImplementedException() throw();
};



} // namespace picmdk


#endif
