How to build:
-------------

Ensure the correct RFAL variant and definition is set in CMakeLists.txt, e.g.:
    set(RFAL_VARIANT "st25r3916" CACHE STRING "Select the RFAL library")
    add_definitions(-DST25R3916)

Then

$ mkdir -p linux_demo/build
$ cd linux_demo/build
$ cmake ..
$ make

Just delete the build folder content to start a clean build


How to run from the build folder:
---------------------------------

$ sudo ./demo/nfc_demo_<variant>


Debugging:
----------
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
sudo gdbtui ./demo/nfc_demo_<variant>
to set a breakpoint:
b <function>
b <filename>:<linenumber>
r to run
s to step into the code (go down into the function)
up to go one level up
n to go to next line
bt to show backtrace
CTRL-C to break
c to continue
info b to list the breakpoints
del n to delete breakpoint
