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

conan_cmake_run(REQUIRES nlohmann_json/3.9.1
                BASIC_SETUP 
                BUILD missing)


add_library(IDOS ido.cpp IDOManager.cpp ManagedRef.cpp CharArrayIDO.cpp idodao.cpp)
#add_executable(IDOS main.cpp ido.cpp IDOManager.cpp ManagedRef.cpp CharArrayIDO.cpp idodao.cpp)
set_property(TARGET IDOS PROPERTY CXX_STANDARD 17)
target_link_libraries(IDOS ${CONAN_LIBS})