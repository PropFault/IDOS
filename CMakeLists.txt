cmake_minimum_required(VERSION 3.0.0)
project(IDOS VERSION 0.1.0)
include(CTest)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_subdirectory(./test)
add_subdirectory(./src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)