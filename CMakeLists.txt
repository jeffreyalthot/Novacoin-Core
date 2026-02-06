cmake_minimum_required(VERSION 3.15)
project(NovacoinCore VERSION 0.4.0 LANGUAGES CXX)

option(USE_OPENSSL "Use OpenSSL for crypto" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(NOVACOIN_SOURCES
    src/main.cpp
    src/novacoin.cpp
    src/config.cpp
    src/logging.cpp

    src/crypto/sha256.cpp
    src/crypto/uint256.cpp

    src/consensus/serialize.cpp
    src/consensus/tx.cpp
    src/consensus/block.cpp
    src/consensus/merkle.cpp
    src/consensus/validation.cpp
    src/consensus/utxo.cpp
    src/consensus/blockindex.cpp
    src/consensus/blockchain.cpp

    src/db/blockstore.cpp

    src/miner/miner.cpp

    src/wallet/keystore.cpp
)

function(add_novacoin_tool target_name)
  add_executable(${target_name} ${NOVACOIN_SOURCES})
  target_include_directories(${target_name} PRIVATE src)

  if (USE_OPENSSL)
    find_package(OpenSSL REQUIRED)
    if (OpenSSL_FOUND)
      target_compile_definitions(${target_name} PRIVATE USE_OPENSSL)
      target_include_directories(${target_name} PRIVATE ${OPENSSL_INCLUDE_DIR})
      target_link_libraries(${target_name} PRIVATE ${OPENSSL_LIBRARIES})
    endif()
  endif()

  if (MSVC)
    target_compile_definitions(${target_name} PRIVATE -D_WIN32_WINNT=0x0A00)
  endif()

  find_package(Threads REQUIRED)
  target_link_libraries(${target_name} PRIVATE Threads::Threads)

  set_target_properties(${target_name} PROPERTIES
    OUTPUT_NAME ${target_name}
    SUFFIX ".exe"
  )
endfunction()

add_novacoin_tool(novacoind)
add_novacoin_tool(novacoin-tx)
add_novacoin_tool(novacoin-wallet)
add_novacoin_tool(novacoin-cli)
add_novacoin_tool(novacoin-regtest)
