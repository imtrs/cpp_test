
project (cpp_test)

INCLUDE_DIRECTORIES ( 
    include
    /usr/include/android
    #/mnt/aosp/system/libziparchive/include    
    #/mnt/aosp/system/libbase/include
)

SET(CPP_TEST_UTILS_SRC_FILES
    src/utils/tc_utils.cpp 
)    

SET (CPP_TEST_SRC_FILES 
    main.cpp
    src/tc_function_test.cpp
    src/test_function.cpp
)

#공통 컴파일 옵션, 링크 옵션
ADD_COMPILE_OPTIONS(
    -g 
    -Wall 
    -std=c++17
) 
    
LINK_LIBRARIES(
    tinyxml2
    pthread
    ziparchive
    jsoncpp
    #/usr/lib/x86_64-linux-gnu/android/libziparchive.so
)

LINK_DIRECTORIES (
    /usr/lib
    /usr/lib/x86_64-linux-gnu/android/
) 

ADD_EXECUTABLE(cpp_test 
    ${CPP_TEST_SRC_FILES}
    ${CPP_TEST_UTILS_SRC_FILES}
) 