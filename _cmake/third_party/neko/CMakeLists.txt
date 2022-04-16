cmake_minimum_required(VERSION 3.23)
find_package(Threads REQUIRED)

include(ExternalProject)

set(NEKO_DL "neko-download")
configure_file(CMakeLists.txt.in "${CMAKE_BINARY_DIR}/${NEKO_DL}/CMakeLists.txt")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
               WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${NEKO_DL}")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${NEKO_DL}")

add_subdirectory("${OPT_NEKO_DIR}"
                 "${CMAKE_BINARY_DIR}/neko-build")