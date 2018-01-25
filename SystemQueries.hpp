/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  SystemQueries.hpp
 *  MemoryManager
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef SystemQueries_hpp
#define SystemQueries_hpp

#ifdef __APPLE__
    #include <unistd.h>
    inline unsigned long long totalMemoryUnix () {
        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        return pages * page_size;
    }
#elif defined _WIN32 || defined _WIN64
    #include <windows.h>
    inline unsigned long long totalMemoryWin () {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;
    }
#else
    #error "unknown platform"
#endif

unsigned long long totalSystemMemory () {
#ifdef __APPLE__
    return totalMemoryUnix ();
#elif defined _WIN32 || defined _WIN64
    return totalMemoryWin  ();
#else
    #error "unknown platform"
#endif
}

#endif /* SystemQueries_h */
