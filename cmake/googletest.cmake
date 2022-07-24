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

set (GoogleTestsList
	dummy
	test_polynomial
	test_chunker
)

include(GoogleTest)

foreach(test ${GoogleTestsList})
	add_executable(${test} ../gtest/${test}.cpp)
	target_link_libraries(${test} GTest::gtest_main rolling-hash)
	gtest_discover_tests(${test})
endforeach()

