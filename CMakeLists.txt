cmake_minimum_required(VERSION 3.0.0)
project(IDOS VERSION 0.1.0)

include(CTest)
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

conan_cmake_run(REQUIRES nlohmann_json/3.9.1 boost/1.74.0 
                BASIC_SETUP 
                BUILD missing)
            

add_executable(IDOS main.cpp ido.cpp IDOManager.cpp ManagedValue.cpp CharArrayIDO.cpp)
set_property(TARGET IDOS PROPERTY CXX_STANDARD 17)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
