INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_ATSC2 atsc2)

FIND_PATH(
    ATSC2_INCLUDE_DIRS
    NAMES atsc2/api.h
    HINTS $ENV{ATSC2_DIR}/include
        ${PC_ATSC2_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    ATSC2_LIBRARIES
    NAMES gnuradio-atsc2
    HINTS $ENV{ATSC2_DIR}/lib
        ${PC_ATSC2_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ATSC2 DEFAULT_MSG ATSC2_LIBRARIES ATSC2_INCLUDE_DIRS)
MARK_AS_ADVANCED(ATSC2_LIBRARIES ATSC2_INCLUDE_DIRS)

