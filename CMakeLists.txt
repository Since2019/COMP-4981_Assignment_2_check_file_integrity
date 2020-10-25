cmake_minimum_required(VERSION 3.18.2)
project(error_detection C)

set(CMAKE_C_STANDARD 11)

add_library(lib STATIC input_handler.c bit_handler.c)
target_compile_options(lib PRIVATE -Wpedantic -Wall -Wextra)

add_executable(encode-parity encode_parity.c)
target_link_libraries(encode-parity lib)
target_compile_options(encode-parity PRIVATE -Wpedantic -Wall -Wextra)

add_executable(decode-parity decode_parity.c)
target_link_libraries(decode-parity lib)
target_compile_options(decode-parity PRIVATE -Wpedantic -Wall -Wextra)

add_executable(encode-checksum encode_checksum.c)
target_link_libraries(encode-checksum lib)
target_compile_options(encode-checksum PRIVATE -Wpedantic -Wall -Wextra)

add_executable(decode-checksum decode_checksum.c)
target_link_libraries(decode-checksum lib)
target_compile_options(decode-checksum PRIVATE -Wpedantic -Wall -Wextra)

add_executable(encode-crc32 encode_crc32.c)
target_link_libraries(encode-crc32 lib)
target_compile_options(encode-crc32 PRIVATE -Wpedantic -Wall -Wextra)

add_executable(decode-crc32 decode_crc32.c)
target_link_libraries(decode-crc32 lib)
target_compile_options(decode-crc32 PRIVATE -Wpedantic -Wall -Wextra)