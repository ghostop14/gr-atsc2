# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND atsc2_sources
    atsc2_rx_filter_impl.cc
    atsc2_fpll_impl.cc
    tcp_sink_impl.cc
)

set(atsc2_sources "${atsc2_sources}" PARENT_SCOPE)
if(NOT atsc2_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT atsc2_sources)

add_library(gnuradio-atsc2 SHARED ${atsc2_sources})
target_link_libraries(gnuradio-atsc2 ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} gnuradio-filter)
set_target_properties(gnuradio-atsc2 PROPERTIES DEFINE_SYMBOL "gnuradio_atsc2_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-atsc2 PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-atsc2 RUNTIME_COMPONENT "atsc2_runtime" DEVEL_COMPONENT "atsc2_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_atsc2_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_atsc2.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_atsc2.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_atsc2_rx_filter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_sink_impl.cc
)

add_executable(test-atsc2 ${test_atsc2_sources})

target_link_libraries(
  test-atsc2
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-atsc2
  boost_date_time
  pthread
)

GR_ADD_TEST(test_atsc2 test-atsc2)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

