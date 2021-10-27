# -- Minimum required version
cmake_minimum_required (VERSION 3.4.0)

# -- Project name
project (rfal-st25r3916)

# -- Various includes
include (GenerateExportHeader)
include (CMakePackageConfigHelpers)
include (GNUInstallDirs)

find_package(Threads)

# -- Set cxx 14 standard default
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -- Set c 99 standard default
set(CMAKE_C_STANDARD 99)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
if (NOT (${CMAKE_INSTALL_PREFIX} STREQUAL "/usr" ) )
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

set(LIB_NAME ${CMAKE_PROJECT_NAME})

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_library(${LIB_NAME} "")
generate_export_header(${LIB_NAME})

set(
        ${LIB_NAME}_PUBLIC_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_analogConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_chip.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_dpo.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_isoDep.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_nfca.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_nfcb.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_nfcDep.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_nfcf.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_nfcv.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_rf.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_st25tb.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_st25xv.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_t1t.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_t2t.h
        ${CMAKE_CURRENT_LIST_DIR}/rfal/include/rfal_t4t.h
)

target_sources(
        ${LIB_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_analogConfig.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_crc.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_dpo.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_iso15693_2.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_isoDep.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_nfc.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_nfca.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_nfcb.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_nfcDep.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_nfcf.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_nfcv.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_st25tb.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_st25xv.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_t1t.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_t2t.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/rfal_t4t.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916/rfal_rfst25r3916.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916/st25r3916.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916/st25r3916.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916/st25r3916_aat.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916/st25r3916_com.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916/st25r3916_irq.c
        ${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916/st25r3916_led.c
)

target_include_directories(
        ${LIB_NAME}
        PUBLIC #PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/common/firmware/STM/utils/Inc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/linux_demo/demo/Inc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/linux_demo/platform/Inc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/linux_demo/platform/Inc/st25r3916>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/rfal/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/rfal/source>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/rfal/source/st25r3916>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(
        ${LIB_NAME}
        PRIVATE
        ST25R3916
)

target_link_libraries(
        ${LIB_NAME}
        PUBLIC
        rt
        m
)

add_executable(demo "")

target_sources(
        demo
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/linux_demo/demo/Src/logger.c
        ${CMAKE_CURRENT_LIST_DIR}/linux_demo/demo/Src/main.c
        ${CMAKE_CURRENT_LIST_DIR}/linux_demo/demo/Src/rfal_analog_config_custom.c
        ${CMAKE_CURRENT_LIST_DIR}/linux_demo/platform/Src/pltf_gpio.c
        ${CMAKE_CURRENT_LIST_DIR}/linux_demo/platform/Src/pltf_spi.c
        ${CMAKE_CURRENT_LIST_DIR}/linux_demo/platform/Src/pltf_timer.c
        ${CMAKE_CURRENT_LIST_DIR}/st25r_demos/Src/demo_ce.c
        ${CMAKE_CURRENT_LIST_DIR}/st25r_demos/Src/demo_polling.c
)

target_link_libraries(
        demo
        PRIVATE
        rfal-st25r3916
        Threads::Threads
)

target_include_directories(
        demo
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/st25r_demos/Inc>
        ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}
)

target_compile_definitions(
        demo
        PRIVATE
        # Define to use RFAL Custom Analog Config
        RFAL_ANALOG_CONFIG_CUSTOM

)



