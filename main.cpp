/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  main.cpp
 *  MemoryManager
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "Testing/StackTest.hpp"
#include "Testing/QueueTest.hpp"
#include "Testing/PoolTest.hpp"
#include "Node.hpp"

int main(int argc, const char * argv[]) {
    StackTest stack;
    stack.run();
    
    QueueTest queue;
    queue.run();
    
    PoolTest pool;
    pool.run();
     
    return 0;
}
