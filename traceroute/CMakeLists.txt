cmake_minimum_required(VERSION 3.24)
project(traceroute)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(traceroute
        src/main.cpp 
	src/Tracerouter/Tracerouter.h 
	src/Tracerouter/Tracerouter.cpp)

target_link_libraries(traceroute PRIVATE Ws2_32)

option(BUILD_TESTS "Build the tests" ON)

if(BUILD_TESTS)
	add_executable(traceTests
        	tests/tests.cpp 
		src/Tracerouter/Tracerouter.h 
		src/Tracerouter/Tracerouter.cpp
	)
	target_link_libraries(traceTests  Ws2_32 GTest::gtest_main)
endif()