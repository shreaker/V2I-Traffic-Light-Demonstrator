set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER ${TOOLCHAIN}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/bin/arm-linux-gnueabihf-g++)

set(CMAKE_SYSROOT ${PIROOT})
set(CMAKE_FIND_ROOT_PATH ${PIROOT})

set(FLAGS "-Wall -pedantic -Wextra -Wl,-rpath-link,${PIROOT}/opt/vc/lib -Wl,-rpath-link,${PIROOT}/lib/arm-linux-gnueabihf -Wl,-rpath-link,${PIROOT}/usr/lib/arm-linux-gnueabihf -Wl,-rpath-link,${PIROOT}/usr/local/lib")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}" CACHE STRING "" FORCE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_INSTALL_PREFIX ${PIROOT}/usr/local)
