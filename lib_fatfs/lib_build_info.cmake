set(LIB_NAME lib_fatfs)

set(LIB_VERSION 0.0.1)

set(LIB_INCLUDES api)

if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/../third_party/FatFs/source/ff.c")
    message(FATAL_ERROR "FatFs submodule missing: run git submodule update --init --recursive in lib_fatfs")
endif()

#set(LIB_DEPENDENT_MODULES "lib_xud")

set(LIB_COMPILER_FLAGS  -O3
                        -Wall
                        -Wextra
                        -Wsign-compare
                        -Wconversion)

XMOS_REGISTER_MODULE()
