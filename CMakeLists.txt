cmake_minimum_required(VERSION 3.0)
project(utilities)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake
  $ENV{DEV_LIBS}/cls/cmake
)
include(Utilities)

find_compiler(
  GNU   4.7  "-Wall -Wextra\
              -Wno-unused-variable\
              -Wno-unused-but-set-variable"
  Clang 3.2  "-Wall -Wextra\
              -Wno-unused-variable"
  MSVC  2013 "/W4"
)

option(USE_MY_PATH "Use my own default library path" OFF)
if(USE_MY_PATH)
  add_default_lib_path($ENV{DEV_LIBS} $ENV{DEV_PLAT})
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

# find_package(Boost REQUIRED COMPONENTS filesystem thread)
# find_package(Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})
# target_link_libraries(<target> ${Boost_LIBRARIES})
#
# find_package(OpenCV REQUIRED)
# include_directories(${OpenCV_INCLUDE_DIRS})
# target_link_libraries(<target> ${OpenCV_LIBS})
#
# find_package(Eigen3 REQUIRED)
# include_directories(${EIGEN3_INCLUDE_DIR})
#
# find_package(FFTW REQUIRED)
# include_directories(${FFTW_INCLUDES})
# target_link_libraries(<target> ${FFTW_LIBRARIES})
#
# find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# include_directories(${PROJECT_BINARY_DIR})
# target_link_libraries(<target> Qt5::Core Qt5::Gui Qt5::Widgets)
#
# find_package(TBB REQUIRED)
# include_directories(${TBB_INCLUDE_DIRS})
# link_directories(${TBB_LIBRARY_DIRS}) # MSVC issue: could not find tbb_debug.lib
# target_link_libraries(<target> ${TBB_LIBRARIES})

set(SRC_LIST
  test/test.cpp
)

set(HEADER_LIST
  include/cls/cls_defs.h
  include/cls/utilities.h
  include/cls/error.hpp
  include/cls/algorithm.hpp
  include/cls/string.hpp
  include/cls/timer.hpp
  include/cls/cmdparser.hpp
  include/cls/file_sys.hpp
  include/cls/factory.hpp
  include/cls/traits.hpp
  include/cls/eigen.hpp
  include/cls/point_types.hpp
  include/cls/byte_array.hpp
  include/cls/dyn_bitset.hpp
)

add_executable(utilities ${SRC_LIST} ${HEADER_LIST})
# target_link_libraries(utilities)
