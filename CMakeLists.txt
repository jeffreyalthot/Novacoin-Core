cmake_minimum_required(VERSION 3.15)
project(NovacoinCore VERSION 0.4.0 LANGUAGES CXX)

option(USE_OPENSSL "Use OpenSSL for crypto" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources - ensure all modules are compiled
set(SRC_FILES
    src/main.cpp
    src/novacoin.cpp
    src/config.cpp
    src/logging.cpp

    src/crypto/sha256.cpp
    src/crypto/eckey.cpp
    src/crypto/uint256.cpp

    src/consensus/serialize.cpp
    src/consensus/tx.cpp
    src/consensus/block.cpp
    src/consensus/merkle.cpp
    src/consensus/compact.cpp
    src/consensus/validation.cpp
    src/consensus/utxo.cpp
    src/consensus/blockindex.cpp
    src/consensus/blockchain.cpp

    src/db/blockstore.cpp

    src/mempool/mempool.cpp

    src/miner/miner.cpp

    src/wallet/keystore.cpp
)

add_executable(novacoin-node ${SRC_FILES})
target_include_directories(novacoin-node PRIVATE src)

if (USE_OPENSSL)
  find_package(OpenSSL REQUIRED)
  if (OpenSSL_FOUND)
    target_compile_definitions(novacoin-node PRIVATE USE_OPENSSL)
    target_include_directories(novacoin-node PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(novacoin-node PRIVATE ${OPENSSL_LIBRARIES})
  endif()
endif()

if (MSVC)
  target_compile_definitions(novacoin-node PRIVATE -D_WIN32_WINNT=0x0A00)
endif()

find_package(Threads REQUIRED)
target_link_libraries(novacoin-node PRIVATE Threads::Threads)
