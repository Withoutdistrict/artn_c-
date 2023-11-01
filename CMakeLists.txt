cmake_minimum_required(VERSION 3.18)

# Define main project
project(ARTN_ABINITIO LANGUAGES CXX C)

#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(VERBOSE ON)

# cmake Project authors
set(AUTHOR "Eugene Sanscartier")
set(AUTHOR_DETAILS "")
set(DESCRIPTION "This is a cmake project that aim to ease compilation. ARTN is a software for transition state finding using activation and relaxation technique. ")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CXX_FLAGS)
    message(STATUS "Setting the build type to \"RelWithDebInfo\" as none was specified.")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Set the build type in this build tree." FORCE)
    # Possible values of build type in MLIP for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CXX_FLAGS)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        "${CMAKE_SOURCE_DIR}/cmake"
        "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(PreventInSourceBuilds)

string(TOUPPER ${CMAKE_BUILD_TYPE} type)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${type}}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${type}}")

# find_package(LAPACK)
# find_package(BLAS)

# include(FetchContent)

# FetchContent_Declare(
#         XTENSOR_GIT
#         GIT_PROGRESS            ON
#         GIT_REPOSITORY          https://github.com/xtensor-stack/xtensor.git
# )
# FetchContent_Declare(
#         XSIMD_GIT
#         GIT_PROGRESS            ON
#         GIT_REPOSITORY          https://github.com/xtensor-stack/xsimd.git
# )
# FetchContent_Declare(
#         XTL_GIT
#         GIT_PROGRESS            ON
#         GIT_REPOSITORY          https://github.com/xtensor-stack/xtl.git
# )
# FetchContent_Declare(
#         XTENSOR_BLAS_GIT
#         GIT_PROGRESS            ON
#         GIT_REPOSITORY          https://github.com/xtensor-stack/xtensor-blas.git
# )
# FetchContent_Declare(
#         XTENSOR_IO_GIT
#         GIT_PROGRESS            ON
#         GIT_REPOSITORY          https://github.com/xtensor-stack/xtensor-io.git
# )

# FetchContent_MakeAvailable(XTL_GIT XTENSOR_GIT XSIMD_GIT XTENSOR_BLAS_GIT)

include_directories(include)

add_executable(artn)

set_target_properties(artn
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(artn blas cblas lapack)

add_subdirectory(src)

option(WITH_LAMMPS "" ON)
set(LAMMPS_ROOT "/home/eugene/Master/lammps_new")
set(LAMMPS_LIB "src/liblammps.so")
if(WITH_LAMMPS)
    set(LAMMPS_LIB ${LAMMPS_ROOT}/${LAMMPS_LIB})
    set(LAMMPS_SRC ${LAMMPS_ROOT}/src)
    include_directories(${LAMMPS_SRC})
    add_compile_definitions(WITH_LAMMPS)
    target_link_libraries(artn ${LAMMPS_LIB})
endif()
message(STATUS  "WITH_LAMMPS=${WITH_LAMMPS}")
message(STATUS  "LAMMPS_ROOT=${LAMMPS_ROOT}")
message(STATUS  "LAMMPS_LIB=${LAMMPS_LIB}\n")


# add_subdirectory(include/xflens/cxxblas)
# add_subdirectory(include/xflens/cxxlapack)