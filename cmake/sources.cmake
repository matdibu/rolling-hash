set(LIB_SOURCE_FILES lib/polynomial.cpp lib/chunker.cpp)
set(CLIENT_SOURCE_FILES src/main.cpp)
set(INCLUDE_DIR include)

add_executable(${PROJECT_NAME}-client ${CLIENT_SOURCE_FILES})

add_library(${PROJECT_NAME} SHARED ${LIB_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/rolling-hash.h)

include_directories(${INCLUDE_DIR})

configure_file(include/config.h.in config.h)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
