cmake_minimum_required(VERSION 3.15)
project(NovacoinCore VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
set(SRC_FILES
    src/main.cpp
    src/novacoin.cpp
    src/config.cpp
    src/logging.cpp
)

add_executable(novacoin-node ${SRC_FILES})
target_include_directories(novacoin-node PRIVATE src)

if (MSVC)
  target_compile_definitions(novacoin-node PRIVATE -D_WIN32_WINNT=0x0A00)
endif()

# Link filesystem on platforms that require it (CMake will handle on modern toolchains)
find_package(Threads REQUIRED)
target_link_libraries(novacoin-node PRIVATE Threads::Threads)