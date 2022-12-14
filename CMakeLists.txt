cmake_minimum_required(VERSION 3.14)
project(myedgeset)

set(CMAKE_CXX_STANDARD 14)

add_library(myedgeset SHARED myedgeset.cc)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(myedgeset_test myedgeset_test.cc)
target_link_libraries(myedgeset_test GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(myedgeset_test)
