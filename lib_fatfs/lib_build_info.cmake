set(LIB_NAME lib_fatfs)

set(LIB_VERSION 0.0.1)

set(LIB_INCLUDES api src)

#set(LIB_DEPENDENT_MODULES "lib_xud")

set(LIB_COMPILER_FLAGS  -O3
                        -Wall
                        -Wextra
                        -Wsign-compare
                        -Wconversion)

XMOS_REGISTER_MODULE()
