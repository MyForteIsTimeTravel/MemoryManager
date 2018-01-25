/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  UnitTest.hpp
 *  NQueens
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef UnitTest_hpp
#define UnitTest_hpp

#include <iostream>
#include <vector>

class UnitTest
    {
    public:
        UnitTest () {}
       ~UnitTest () {}
    
        virtual void setup    () = 0;
        virtual void run      () = 0;
        virtual void teardown () = 0;
    
        virtual std::string name () { return "Unit Test"; };
    
    protected:
        typedef std::pair<std::string, bool> Test;
    
        template <class T>
        void inline assert (std::string message, T expected, T actual)
            { results.push_back(Test(message, expected == actual)); }
    
        template <class T, class V>
        void inline assert (std::string message, T expected, V actual)
            { results.push_back(Test(message, expected == actual)); }
    
        bool passed () {
        return std::all_of(results.begin(), results.end(), [] (Test t) {
            return t.second;
        });
    }

    void show ()  {
        std::cout << "results" << std::endl;
        std::cout << "-------------------------" << std::endl;
        std::for_each(results.begin(), results.end(), [] (Test t) -> void {
            std::cout << t.first << ": " << ((t.second) ? "PASS" : std::string("FAIL")) << std::endl;
        });
        std::cout << std::endl;
        std::cout << name() << ": " << ((passed()) ? "SUCCESS" : "FAIL") << std::endl;
        std::cout << std::endl;
    }

private:
    std::vector<Test> results;
};

#endif /* UnitTest_hpp */
