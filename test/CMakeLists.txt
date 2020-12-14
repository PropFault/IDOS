cmake_minimum_required(VERSION 3.0.0)
project(IDOS_TEST VERSION 0.1.0)

enable_testing()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake" 
                 TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_add_remote(NAME bincrafters 
                 INDEX 1
                 URL https://api.bintray.com/conan/bincrafters/public-conan
                 VERIFY_SSL True)         

conan_cmake_run(REQUIRES nlohmann_json/3.9.1  boost_test/1.69.0@bincrafters/stable 
                BASIC_SETUP 
                BUILD missing)
            

add_executable(IDOSUnitTest main_unit_test.cpp idotest.cpp IDOManagerTest.cpp)
set_property(TARGET IDOSUnitTest PROPERTY CXX_STANDARD 17)
target_link_libraries(IDOSUnitTest IDOS)
target_link_libraries(IDOSUnitTest ${CONAN_LIBS})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
add_test(NAME IDOS_Unit_Test COMMAND IDOSUnitTest -x true)