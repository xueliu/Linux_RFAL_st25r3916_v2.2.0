# Bring headers into the project
include_directories(
    ../../../rfal/include
    ../../../common/firmware/STM/utils/Inc
    ../../../rfal/source/st25r3916
    ../../../rfal/source
    ../../platform/Inc
    ../../platform/Inc/st25r3916
    ../../demo/Inc)

# Source files to build the project
file(GLOB SOURCE1 "../../../rfal/source/*.c")
file(GLOB SOURCE2 "../../../rfal/source/st25r3916/*.c")
file(GLOB SOURCE3 "../../platform/Src/*.c")
set(SOURCE ${SOURCE1} ${SOURCE2} ${SOURCE3})

# Link with rt for POSIX timers
find_library(LIBRT_PATH rt)
find_library(LIBMATH_PATH m)

# Create the shared library
add_library(rfal_st25r3916 SHARED ${SOURCE})

target_link_libraries(rfal_st25r3916 "${LIBRT_PATH}" "${LIBMATH_PATH}")
