cmake_minimum_required(VERSION 2.8.11)
project(utilities)

list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake
  $ENV{DEV_LIBS}/clany/cmake
)
include(Utilities)

find_compiler(
  GNU   4.7  "-std=c++11 -Wall -Wextra -Wno-unused-variable\
              -Wno-unused-but-set-variable"
  Clang 3.2  "-std=c++11 -Wall -Wextra -Wno-unused-variable"
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
  include/clany/clany_defs.h
  include/clany/utilities.h
  include/clany/algorithm.hpp
  include/clany/timer.hpp
  include/clany/cmdparser.hpp
  include/clany/file_operation.hpp
  include/clany/factory.hpp
  include/clany/traits.hpp
  include/clany/eigen.hpp
  include/clany/point_types.hpp
  include/clany/byte_array.hpp
  include/clany/dyn_bitset.hpp
)

add_executable(utilities ${SRC_LIST} ${HEADER_LIST})
# target_link_libraries(utilities)
