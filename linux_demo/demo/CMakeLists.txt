# Bring headers into the project
include_directories(
    ../../common/firmware/STM/utils/Inc
    ../platform/Inc
    ../platform/Inc/${RFAL_VARIANT}
    ../../rfal/include
    ../../rfal/source/${RFAL_VARIANT}
    ../../st25r_demos/Inc
    ./Inc)

file(GLOB MAIN "Src/*.c")
file(GLOB SOURCE_DEMO1 "../../st25r_demos/Src/demo_ce.c")
file(GLOB SOURCE_DEMO2 "../../st25r_demos/Src/demo_polling.c")
file(GLOB SOURCE_RFAL_CUSTOM_ANALOG_CONFIG "Src/rfal_analog_config_custom.c")
set(SOURCES ${MAIN} ${SOURCE_DEMO1} ${SOURCE_DEMO2} ${SOURCE_RFAL_CUSTOM_ANALOG_CONFIG})

# Define to use RFAL Custom Analog Config
add_definitions(-DRFAL_ANALOG_CONFIG_CUSTOM)

# Directory in which the linker will look for the library (must be set before the add_executable() command!)
link_directories(../build/rfal/${RFAL_VARIANT})

# link with pthread for semaphores
find_library(LIBPTHREAD_PATH pthread)

# Set the executable name
add_executable(nfc_demo_${RFAL_VARIANT} ${SOURCES})

# Set the library to link with
target_link_libraries(nfc_demo_${RFAL_VARIANT} librfal_${RFAL_VARIANT}.so ${LIBPTHREAD_PATH})
