include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(
	dummy
	../gtest/unittest.cpp
)

target_link_libraries(
	dummy
	GTest::gtest_main
)

# add_test(NAME dummy_test COMMAND dummy)

include(GoogleTest)
gtest_discover_tests(dummy)
