cmake_minimum_required(VERSION 3.20)

set(VERSION 0.1.0)

project(rolling-hash VERSION ${VERSION} LANGUAGES CXX)

include(cmake/sources.cmake)
include(cmake/compile_options.cmake)
include(cmake/tests.cmake)
include(cmake/googletest.cmake)

