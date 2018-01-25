/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  MemoryManager.hpp
 *  MemoryManager
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef MemoryManager_hpp
#define MemoryManager_hpp

#include "BytePointer.hpp"
#include "Node.hpp"

#include <iostream>
#include <vector>
#include <stack>
#include <deque>

class MemoryManager {
    public:
        enum Mode { Stack, Queue, Pool };
    
        MemoryManager (Mode _mode, size_t _size);
       ~MemoryManager ();

        void*  allocate (size_t _size);
        bool deallocate (void*  _data);
        void release ();
    
        inline size_t occupiedMemory () { return used; }
        inline size_t totalMemory    () { return size; }
        inline size_t freeMemory     () { return size - used; }

        void reportStatus ();
    
    private:
    
        /** return nullptr on fail */
        BytePointer StackMalloc (size_t _size);
        BytePointer QueueMalloc (size_t _size);
        BytePointer PoolMalloc  (size_t _size);
    
        /** return false on fail */
        bool StackFree (void* _data);
        bool QueueFree (void* _data);
        bool PoolFree  (void* _data);
    
        void StackRelease ();
        void QueueRelease ();
        void PoolRelease  ();
    
        const Mode  mode;    // the strategy employed by this instance of a manager
        BytePointer data;    // the handle to the preallocated memory

        std::vector<Node> pool;
        std::stack <Node> stack;
        std::deque <Node> queue;
        size_t   size;      // the total size of the preallocated memory
        size_t   used;      // the total size of used memory
};

#endif /* MemoryManager_hpp */
