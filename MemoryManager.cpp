/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  MemoryManager.cpp
 *  MemoryManager
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "MemoryManager.hpp"
#include "SystemQueries.hpp"

/**
 *  MemoryManager Constructor
 *
 *  _mode   the type of the allocator object
 *  _size   the amount of memory to preallocate
 *
 *  Constructs a Memory Manager object of the given mode and size.
 *  Reports system memory and kills executing program on errors such as
 *  malloc failure or too much memory requested.
 */
MemoryManager::MemoryManager (Mode _mode, size_t _size)
    : mode (_mode), size (_size), used (0) {
    std::cout << std::endl;
    std::cout << "SYSTEM MEMORY: " << totalSystemMemory() << " Bytes";
    std::cout << std::endl;
    
    if (size > totalSystemMemory()) {
        std::cout << "ERROR: requested more RAM than system contains" << std::endl;
        exit (1);
    }
 
    if (!(data = (char*)malloc(size))) {
        std::cout << "ERROR: malloc failure" << std::endl;
        exit (1);
    }
}

/**
 *  MemoryManager Destructor
 *
 *  Frees the block of memory
 */
MemoryManager::~MemoryManager () {
    free (data);
}

/**
 *  allocate
 *
 *  _size   the size of memory required
 *
 *  Passes the size to the appropriate allocation function with a kind
 *  of enum based manual polymorphism. returns a null pointer on failure.
 */
void* MemoryManager::allocate (size_t _size) {
    if ((used + _size) < size) {
        // allocataion is safe, continue
        switch (mode) {
            case Stack: return StackMalloc(_size);
            case Queue: return QueueMalloc(_size);
            case Pool:  return PoolMalloc (_size);
        }
    } else {
        // allocation will overflow data, fail
        return nullptr;
    }
}

/**
 *  deallocate
 *
 *  _data   a pointer to the data to free
 *
 *  Passes the size to the appropriate deallocation function with a kind
 *  of enum based manual polymorphism. returns true on successful deallocation,
 *  false otherwise.
 */
bool MemoryManager::deallocate (void* _data) {
    switch (mode) {
        case Stack: return StackFree(_data);
        case Queue: return QueueFree(_data);
        case Pool:  return PoolFree (_data);
    }
}

/**
 *  release
 *
 *  cleares all data from the pool, using a switch statement to call the
 *  implementation appropriate method.
 */
void MemoryManager::release () {
    switch (mode) {
        case Stack: return StackRelease();
        case Queue: return QueueRelease();
        case Pool:  return PoolRelease();
    }
}

/**
 *  reportStatus
 *
 *  outputs the status of the memory manager to the console.
 */
void MemoryManager::reportStatus() {
    std::cout << "free memory:     " << freeMemory() << " Bytes" << std::endl;
    std::cout << "occupied memory: " << occupiedMemory() << " Bytes" << std::endl;
    std::cout << "total memory:    " << size << " Bytes" << std::endl;
    std::cout << std::endl;
}

/**
 *  StackMalloc
 *
 *  _size   the size of memory required
 *
 *  Allocates memory using the stack implementation.
 */
BytePointer MemoryManager::StackMalloc (size_t _size) {
    Node n;
    n.size = _size;
    n.data = data + used;
    used += _size;
    stack.push(n);
    return stack.top().data;
}

/**
 *  QueueMalloc
 *
 *  _size   the size of memory required
 *
 *  Allocates memory using the queue implementation.
 */
BytePointer MemoryManager::QueueMalloc (size_t _size) {
    Node n;
    n.size = _size;
    n.data = data + used;
    used += _size;
    queue.push_back (n);
    return queue.back().data;
}

/**
 *  PoolMalloc
 *
 *  _size   the size of memory required
 *
 *  Allocates memory using the pool implemenation
 */
BytePointer MemoryManager::PoolMalloc  (size_t _size) {
    // if the pool is empty we can just throw it in...
    if (pool.empty()) {
        Node n;
        n.size = _size;
        n.data = data + used;
        used += _size;
        pool.push_back (n);
        return n.data;
    }
    
    // likewise if there's space free at the back...
    else if ((data + size) - (pool.back().data + pool.back().size + _size) >= size) {
        Node n;
        n.size = _size;
        n.data = data + used;
        used += _size;
        pool.push_back (n);
        return n.data;
    }
    
    // ... or the front...
    else if ((pool.front().data) - (data) >= _size) {
        Node n;
        n.size = _size;
        n.data = data + used;
        used += _size;
        pool.insert(pool.begin(), n);
        return n.data;
    }
    
    // ...otherwise, try find space elsewhere
    else {
        // find the biggest gap in the block
        std::vector<Node>::iterator it;
        for (it = pool.begin(); it != pool.end(); ++it) {
            size_t gap = (*(it + 1)).data - ((*it).data + (*it).size);
            if (gap >= _size) {
                Node n;
                n.size = _size;
                n.data = data + used;
                used += _size;
                pool.insert (it, n);
                return n.data;
            }
        }
    }
    
    // if we get to here there's no space, give em null
    return nullptr;
}

/**
 *  StackFree
 *
 *  _data   a pointer to the data to free
 *
 *  Deallocates memory using the stack method. _data must be the
 *  address of the top node in the data, otherwise deallocation fails.
 */
bool MemoryManager::StackFree (void* _data) {
    if (!stack.empty()) {
        if (_data == stack.top().data) {
            used -= stack.top().size;
            stack.pop();
            return true;
        } else return false;
    } else return false;
}

/**
 *  QueueFree
 *
 *  _data   a pointer to the data to free
 *
 *  Deallocates memory using the queue method. _data must be the
 *  address of the front of back node in the data, otherwise deallocation
 *  fails.
 */
bool MemoryManager::QueueFree (void* _data) {
    if (!queue.empty()) {
        if (_data == queue.back().data) {
            used -= queue.back().size;
            queue.pop_back();
            return true;
        }
        else if (_data == queue.front().data) {
            used -= queue.front().size;
            queue.pop_front();
            return true;
        } else return false;
    } else return false;
}

/**
 *  PoolFree
 *
 *  _data   a pointer to the data to free
 *
 *  Deallocates memory using the queue method. Deallocation fails
 *  when _data is not the pointer for any Node in the structure.
 */
bool MemoryManager::PoolFree  (void* _data) {
    if (!pool.empty()) {
        // search the pool for a node that starts at _data
        std::vector<Node>::iterator it;
        for (it = pool.begin(); it != pool.end(); ++it) {
            if ((*it).data == _data) {
                used -= (*it).size;
                pool.erase(it);
                return true;
            }
        }
        return false;
    } else return false;
}

/**
 *  StackRelease
 *
 *  pops all Node's from the stack
 */
void MemoryManager::StackRelease () {
    used = 0;
    while (!stack.empty()) stack.pop();
}

/**
 *  QueueRelease
 *
 *  clears the queue
 */
void MemoryManager::QueueRelease () {
    used = 0;
    queue.clear();
}

/**
 *  PoolRelease
 *
 *  clears the pool
 */
void MemoryManager::PoolRelease  () {
    used = 0;
    pool.clear();
}
