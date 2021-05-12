# Cross compilation file for the Raspberry Pi

# Specify the target OS name
set(CMAKE_SYSTEM_NAME Linux)

# Specify the archiving tool for static libraries
set(CMAKE_AR          C:/SysGCC/raspberry/bin/arm-linux-gnueabihf-ar.exe)

# Specify the Cross Compiler
set(CMAKE_C_COMPILER  C:/SysGCC/raspberry/bin/arm-linux-gnueabihf-gcc.exe)

# Specify sysroot path
set(CMAKE_SYSROOT     C:/SysGCC/raspberry/arm-linux-gnueabihf/sysroot)

# Specify RPATH to locate .so path on the target
set(CMAKE_BUILD_RPATH rfal/st25r3916)
