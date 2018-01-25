/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Node.hpp
 *  MemoryManager
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef Node_hpp
#define Node_hpp

#include "BytePointer.hpp"
#include <cstddef>

struct Node {
    size_t      size;  // size of the Node in bytes
    BytePointer data;  // pointer to the node's memory
};

#endif /* Node_hpp */
