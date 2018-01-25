/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  PoolTest.hpp
 *  MemoryManager
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef PoolTest_hpp
#define PoolTest_hpp

#include "MemoryManager.hpp"
#include "UnitTest.hpp"

#define POOL_SIZE 1024

class PoolTest : public UnitTest {
public:
    PoolTest () {}
    ~PoolTest () {}
    
    void setup    () override {}
    void teardown () override {}
    
    std::string name () override { return "Pool Test"; }
    
    void run () override {
        // run tests
        PoolCorrectnessTest ();
        PoolSpeedTest       ();
        
        // show results
        show                 ();
    }
    
    /**
     *  Tests the Pool implementation for correectness
     */
    void PoolCorrectnessTest () {
        MemoryManager manager (MemoryManager::Mode::Pool, POOL_SIZE);
        
        manager.reportStatus();
        double* a = (double*) manager.allocate (sizeof(double));
        assert("Pool Allocation Test 1", true, a != nullptr);
        
        manager.reportStatus();
        int*    b = (int*)    manager.allocate (sizeof(int));
        assert("Pool Allocation Test 2", true, b != nullptr);
        
        manager.reportStatus();
        bool*   c = (bool*)   manager.allocate (sizeof(bool));
        assert("Pool Allocation Test 3", true, c != nullptr);
        
        manager.reportStatus();
        assert("Pool Allocation Test 4", manager.occupiedMemory(), sizeof(double) + sizeof(int) + sizeof(bool));
        
        *a = 3.14159;
        *b = 256;
        *c = false;

        assert("Pool Allocation test 5", 3.14159, *a);
        assert("Pool Allocation test 6", 256, *b);
        assert("Pool Allocation test 7", false, *c);
        
        assert("Pool Deallocation test 1", manager.deallocate(c), true);
        manager.reportStatus();
        assert("Pool Deallocation test 2", manager.deallocate(b), true);
        manager.reportStatus();
        assert("Pool Deallocation test 3", manager.occupiedMemory(), sizeof(double));
        
        int* d = (int*) manager.allocate(sizeof(int));
        assert("Pool Allocation Test 8", true, d != nullptr);
        
        short* e = (short*) manager.allocate(sizeof(short));
        assert("Pool Allocation Test 9", true, e != nullptr);
        
        *d = 86;
        *e = 15;
        
        assert("Pool Allocation test 10", *d, 86);
        assert("Pool Allocation test 11", *e, 15);
        assert("Pool Allocation test 12", *a, 3.14159);
        
        manager.release();
        
        assert("Pool Release Test 1", POOL_SIZE, manager.freeMemory());
        assert("Pool Release Test 2", 0, manager.occupiedMemory());
        
        int* f = (int*)manager.allocate(sizeof(int));
        char* g = (char*)manager.allocate(sizeof(char));
        
        *f = 64;
        *g = 'A';
        
        assert("Pool Allocation test 13", *f, 64);
        assert("Pool Allocation test 14", *g, 'A');
        
        // free an arbitrary pieces of memory
        assert("Pool Deallocation test 4", manager.deallocate(f), true);
        
        // try allocate
        int* h = (int*)manager.allocate(sizeof(int));
        assert("Pool Allocation test 15", true, h != nullptr);
        
        *h = 32;
        
        assert("Pool Allocation test 16", *h, 32);
    }
    
    /**
     *  Tests the pool can't be overfilled
     */
    void PoolFillTest () {
        MemoryManager manager (MemoryManager::Mode::Stack, POOL_SIZE);
        
        int* n = (int*)manager.allocate(sizeof(int));
        while (n != nullptr) {
            n = (int*)manager.allocate(sizeof(int));
            
            if (manager.occupiedMemory() > manager.totalMemory()) {
                assert ("Pool Fill Test", true, false);
                return;
            }
        }
        
        assert ("Pool Fill Test", true, true);
    }
    
    /**
     *  Tests the Pool implementation for speed vs new
     */
    void PoolSpeedTest () {
        MemoryManager manager (MemoryManager::Mode::Pool, POOL_SIZE);
        
        // allocate a load of data with new
        clock_t newStart = clock();
        for (int i = 0; i < TEST_DEPTH; ++i) new int();
        double newTime = (double)(clock() - newStart) / CLOCKS_PER_SEC;
        
        // allocate a load of data with manager
        clock_t managedStart = clock();
        for (int i = 0; i < TEST_DEPTH; ++i) manager.allocate(sizeof(int));
        double managedTime = (double)(clock() - managedStart) / CLOCKS_PER_SEC;
        
        // who was faster
        assert("Pool Speed Test",  true, (managedTime < newTime));
    }
};

#endif /* PoolTest_h */
