/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  QueueTest.hpp
 *  MemoryManager
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef QueueTest_hpp
#define QueueTest_hpp

#include "MemoryManager.hpp"
#include "UnitTest.hpp"

#define POOL_SIZE 1024

class QueueTest : public UnitTest {
public:
    QueueTest () {}
    ~QueueTest () {}
    
    void setup    () override {}
    void teardown () override {}
    
    std::string name () override { return "Queue Test"; }
    
    void run () override {
        // run tests
        QueueCorrectnessTest ();
        QueueFillTest        ();
        QueueSpeedTest       ();
        
        // show results
        show                 ();
    }
    
    /**
     *  Tests the Queue implementation for correectness
     */
    void QueueCorrectnessTest () {
        MemoryManager manager (MemoryManager::Mode::Queue, POOL_SIZE);
        
        manager.reportStatus();
        double* a = (double*) manager.allocate (sizeof(double));
        assert("Queue Allocation Test 1", true, a != nullptr);
        
        manager.reportStatus();
        int*    b = (int*)    manager.allocate (sizeof(int));
        assert("Queue Allocation Test 2", true, b != nullptr);
        
        manager.reportStatus();
        bool*   c = (bool*)   manager.allocate (sizeof(bool));
        assert("Queue Allocation Test 3", true, c != nullptr);
        
        manager.reportStatus();
        assert("Queue Allocation Test 4", manager.occupiedMemory(), sizeof(double) + sizeof(int) + sizeof(bool));
        
        *a = 3.14159;
        *b = 256;
        *c = false;
        
        std::cout << std::endl << "a: " << *a << std::endl;
        std::cout << std::endl << "b: " << *b << std::endl;
        
        assert("Queue Allocation test 5", 3.14159, *a);
        assert("Queue Allocation test 6", 256, *b);
        assert("Queue Allocation test 7", false, *c);
        
        assert("Queue Deallocation test 1", manager.deallocate(c), true);
        manager.reportStatus();
        assert("Queue Deallocation test 2", manager.deallocate(b), true);
        manager.reportStatus();
        assert("Queue Deallocation test 3", manager.occupiedMemory(), sizeof(double));
        
        int* d = (int*) manager.allocate(sizeof(int));
        assert("Queue Allocation Test 8", true, d != nullptr);
        
        short* e = (short*) manager.allocate(sizeof(short));
        assert("Queue Allocation Test 9", true, e != nullptr);
        
        *d = 86;
        *e = 15;
        
        assert("Queue Allocation test 10", *d, 86);
        assert("Queue Allocation test 11", *e, 15);
        assert("Queue Allocation test 12", *a, 3.14159);
        
        manager.release();
        
        assert("Queue Release Test 1", POOL_SIZE, manager.freeMemory());
        assert("Queue Release Test 2", 0, manager.occupiedMemory());
        
        int* f = (int*)manager.allocate(sizeof(int));
        char* g = (char*)manager.allocate(sizeof(char));
        
        *f = 64;
        *g = 'A';
        
        assert("Queue Allocation test 13", *f, 64);
        assert("Queue Allocation test 14", *g, 'A');
    }
    
    /**
     *  Checks the queue can't be overfilled
     */
    void QueueFillTest () {
        MemoryManager manager (MemoryManager::Mode::Stack, POOL_SIZE);
        
        int* n = (int*)manager.allocate(sizeof(int));
        while (n != nullptr) {
            n = (int*)manager.allocate(sizeof(int));
            
            if (manager.occupiedMemory() > manager.totalMemory()) {
                assert ("Queue Fill Test", true, false);
                return;
            }
        }
        
        assert ("Queue Fill Test", true, true);
    }
    
    /**
     *  Tests the Queue implementation for speed vs new
     */
    void QueueSpeedTest () {
        MemoryManager manager (MemoryManager::Mode::Queue, POOL_SIZE);
     
        // allocate a load of data with new
        clock_t newStart = clock();
        for (int i = 0; i < TEST_DEPTH; ++i) new int();
        double newTime = (double)(clock() - newStart) / CLOCKS_PER_SEC;
        
        // allocate a load of data with manager
        clock_t managedStart = clock();
        for (int i = 0; i < TEST_DEPTH; ++i) manager.allocate(sizeof(int));
        double managedTime = (double)(clock() - managedStart) / CLOCKS_PER_SEC;
        
        // who was faster
        assert("Queue Speed Test",  true, (managedTime < newTime));
    }
};

#endif /* QueueTest_hpp */
