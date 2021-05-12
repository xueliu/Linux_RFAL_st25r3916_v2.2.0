How to build on Windows for Raspberry Pi:
-----------------------------------------

Install cmake for Windows (https://cmake.org)

Install cross-compilation toolchain (http://gnutoolchains.com/raspberry)

Use the cross compilation file toolchain_arm-linux-gnueabihf.cmake, you may need to update the CMAKE_BUILD_RPATH with the path to the right RFAL library.

Ensure the correct RFAL variant and definition is set in CMakeLists.txt, e.g.:
    set(RFAL_VARIANT "st25r3916" CACHE STRING "Select the RFAL library")
    add_definitions(-DST25R3916)

Then

Using the Git Bash shell, configure cmake, specifying the toolchain file on the command line:

$ mkdir -p linux_demo/build
$ cd linux_demo/build
$ cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./toolchain_arm-linux-gnueabihf.cmake ..
-- The C compiler identification is GNU 6.3.0
-- The CXX compiler identification is GNU 6.3.0
-- Check for working C compiler: C:/SysGCC/raspberry/bin/arm-linux-gnueabihf-gcc.exe
-- Check for working C compiler: C:/SysGCC/raspberry/bin/arm-linux-gnueabihf-gcc.exe -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: C:/SysGCC/raspberry/bin/arm-linux-gnueabihf-c++.exe
-- Check for working CXX compiler: C:/SysGCC/raspberry/bin/arm-linux-gnueabihf-c++.exe -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: linux_demo/build

Then build:

$ make
Scanning dependencies of target rfal_st25r3916
[  3%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_analogConfig.c.o
[  6%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_crc.c.o
[  9%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_dpo.c.o
[ 12%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_iso15693_2.c.o
[ 15%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_isoDep.c.o
[ 18%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_nfc.c.o
[ 21%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_nfcDep.c.o
[ 25%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_nfca.c.o
[ 28%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_nfcb.c.o
[ 31%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_nfcf.c.o
[ 34%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_nfcv.c.o
[ 37%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_st25tb.c.o
[ 40%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_st25xv.c.o
[ 43%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_t1t.c.o
[ 46%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_t2t.c.o
[ 50%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/rfal_t4t.c.o
[ 53%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/st25r3916/rfal_rfst25r3916.c.o
[ 56%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/st25r3916/st25r3916.c.o
[ 59%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/st25r3916/st25r3916_aat.c.o
[ 62%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/st25r3916/st25r3916_com.c.o
[ 65%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/st25r3916/st25r3916_irq.c.o
[ 68%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/C_/repositories/ST25R3911_linux/rfal/source/st25r3916/st25r3916_led.c.o
[ 71%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/__/__/platform/Src/pltf_gpio.c.o
[ 75%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/__/__/platform/Src/pltf_spi.c.o
[ 78%] Building C object rfal/st25r3916/CMakeFiles/rfal_st25r3916.dir/__/__/platform/Src/pltf_timer.c.o
[ 81%] Linking C shared library librfal_st25r3916.so
[ 81%] Built target rfal_st25r3916
[ 84%] Building C object demo/CMakeFiles/nfc_demo_st25r3916.dir/Src/logger.c.o
[ 87%] Building C object demo/CMakeFiles/nfc_demo_st25r3916.dir/Src/main.c.o
[ 90%] Building C object demo/CMakeFiles/nfc_demo_st25r3916.dir/Src/rfal_analog_config_custom.c.o
[ 93%] Building C object demo/CMakeFiles/nfc_demo_st25r3916.dir/C_/repositories/ST25R3911_linux/st25r_demos/Src/demo_ce.c.o
[ 96%] Building C object demo/CMakeFiles/nfc_demo_st25r3916.dir/C_/repositories/ST25R3911_linux/st25r_demos/Src/demo_polling.c.o
[100%] Linking C executable nfc_demo_st25r3916
[100%] Built target nfc_demo_st25r3916


WinSCP to the device

$ chmod +x nfc_demo_st25r3916
$ sudo ./demo/nfc_demo_st25r3916

The root CMakeLists.txt specifies the RFAL_VARIANT to build for a different target.

Pay attention to the RPATH value set with CMAKE_BUILD_RPATH, check it with /c/SysGCC/raspberry/bin/arm-linux-gnueabihf-readelf.exe -d demo/nfc_demo_st25r3916


Debugging:
----------
$ cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./toolchain_arm-linux-gnueabihf.cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
sudo gdbtui ./demo/nfc_demo_<variant>
b to set a breakpoint
r to run
s to step into the code (go down into the function)
up to go one level up
n to go to next line
bt to show backtrace
CTRL-C to break
c to continue
info b to list the breakpoints
del n to delete breakpoint
