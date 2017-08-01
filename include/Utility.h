#ifndef PICMDK_UTILITY_H
#define PICMDK_UTILITY_H


#include "Vector.h"

#include <sstream>
#include <string>
#include <vector>


namespace picmdk {


template<typename T>
std::vector<T> makeVector(T elem)
{
    std::vector<T> v;
    v.push_back(elem);
    return v;
}


// Index is a type used for indexing, normally int, Int2 or Int3
// This is probably not needed with ParticleIterator and CellIterator
template<typename Index>
class Range {
public:
    class Iterator {
    public:
        Iterator& operator++ ();
        Iterator operator++ (int)
        {
            Iterator result(*this);
            ++(*this);
            return result;
        }
        Index& operator*()
        {
            return index;
        }
        const Index& operator*() const
        {
            return index;
        }
        Index* operator->()
        {
            return &index;
        }
        const Index* operator->() const
        {
            return &index;
        }

    private:
        Index begin, end, index;
    };
};

// Convert to string value of type T which can be written to ostream 
template<typename T>
inline std::string toString(const T& value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}


// Version for vectors of 2 components with .x, .y, .z access
template<typename T>
inline std::string toString(const Vector2<T>& v, const std::string& delimeter,
    const std::string& prefix = "(", const std::string& postfix = ")")
{
    std::ostringstream stream;
    stream << prefix << v.x << delimeter << v.y << postfix;
    return stream.str();
}


// Version for vectors of 3 components with .x, .y, .z access
template<typename T>
inline std::string toString(const Vector3<T>& v, const std::string& delimeter,
    const std::string& prefix = "(", const std::string& postfix = ")")
{
    std::ostringstream stream;
    stream << prefix << v.x << delimeter << v.y << delimeter << v.z << postfix;
    return stream.str();
}


} // namespace picmdk


#endif
