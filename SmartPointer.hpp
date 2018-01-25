/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  SmartPointer.hpp
 *  MemoryManager
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef SmartPointer_hpp
#define SmartPointer_hpp

#include "BytePointer.hpp"

// UNFINISHED, couldn't crack the generics
template <class T>
class SmartPointer {
    public:
        SmartPointer ();
       ~SmartPointer ();
    
        inline T load () { return *data; }
        inline void store (T val) { *data = val; }
    
    private:
        T* data;
    
};

#endif /* SmartPointer_h */
