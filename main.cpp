#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <boost/signals2.hpp>
#include <signal.h> /*signal,raise,sig_atomic_t*/
#include <string> 
#include <cstring> 
#include <vector> 
#include <tinyxml2.h> 
#include <utils/tc_utils.h> 
#include <test_function.h> 

typedef void(*TEST_FUNCTION_PTR)(void); 

using namespace tinyxml2; 
using namespace std; 

std::vector<TEST_FUNCTION_PTR> testFuncPtrVec = { 
    testFunc001,
    testFunc002,
    testFunc003,
    testFunc004,
    testFunc005,
    testFunc006,
    testFunc007,  //bit shift operator test 
    testFunc008,  //tc_utils functions test 
    testFunc009,
    testFunc010,
    testFunc011,  //access function test  
    testFunc012,
    testFunc013,
    testFunc014,
    testFunc015,
    testFunc016,
    testFunc017,
    testFunc018,
    testFunc019,
}; 

int32_t main(int32_t argc,char** argv){ 
    std::cout << "================================================================" << std::endl; 
    if(argc > 1){ 
        int32_t index = stoi(argv[1]);  
        printf("argv1:%s,index:%d\n",argv[1],index); 
        std::cout << "----------------------------------------------------------------" << std::endl; 
        testFuncPtrVec[index - 1](); 
    } 
    else { 
        testFuncPtrVec[testFuncPtrVec.size() - 1](); 
    } 
    std::cout << "================================================================" << std::endl;    

    //getch(); 
    return 0; 
} 



#if 0

struct HelloWorld{
  void operator()() const {
    std::cout << "Hello,World!" << std::endl;
  }
};  


sig_atomic_t signaled = 0;  
void my_handler (int param) {  
  signaled = 1;  
}  

 

 #if 1
int main () { 
    void (*prev_handler)(int); 
    prev_handler = signal(SIGINT,my_handler); 

    raise(SIGINT); 
  
    printf ("signaled is %d.\n",signaled);  
 
    return 0; 
} 
#else 
int32_t main(int32_t argc,char** argv){
    std::cout << "Hello,world!" << std::endl;

    // Signal with no arguments and a void return value
    boost::signals2::signal<void ()> sig;

    // Connect a HelloWorld slot
    HelloWorld hello;
    sig.connect(hello);

    // Call all of the slots
    sig();

    return 0;
}
#endif

#endif