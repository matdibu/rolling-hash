find_package(GTest CONFIG REQUIRED)

add_executable(dummy ../gtest/unittest.cpp)
target_link_libraries(dummy gtest_main GTest::gtest)
add_test(NAME dummy_test COMMAND dummy)
