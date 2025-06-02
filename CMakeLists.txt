cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(GammaTest)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  set(Geant4_DIR "/Users/kartikeyasharma/Desktop/Software/Geant4/geant4-v11.3.0-install/lib/cmake/Geant4")
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(SYSTEM BEFORE ${PROJECT_SOURCE_DIR}/include)

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

set(ROOTSYS "/opt/homebrew/opt/root")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIRS})

add_executable(gamma_test gamma_test.cc ${sources} ${headers})
target_link_libraries(gamma_test ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

add_custom_target(GammaTest DEPENDS gamma_test)
