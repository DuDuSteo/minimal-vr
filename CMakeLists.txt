cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME minimal-vr)
set(CMAKE_CXX_STANDARD 14)

## Paths
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw)
set(OPENVR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/openvr)
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)

## OpenVR
set(OPENVR_LIB_DIR ${OPENVR_DIR}/lib ${OPENVR_DIR}/bin)
set(OPENVR_INCLUDE_DIR ${OPENVR_DIR}/headers)

find_library(OPENVR_LIBRARIES 
	NAMES 
	  openvr_api 
	PATHS	
	  ${OPENVR_DIR}/bin
	  ${OPENVR_DIR}/lib
	PATH_SUFFIXES
	  osx32
	  linux64
	  ${WINDOWS_PATH_SUFFIXES}
	NO_DEFAULT_PATH
	NO_CMAKE_FIND_ROOT_PATH
)

## GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL " FORCE")
set(GLFW_BUILD_TESTS OFF CACHE BOOL " FORCE")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " FORCE")

add_subdirectory(${GLFW_DIR})

## glm

add_subdirectory(${GLM_DIR})


## Finish setup

set(SOURCE ${SOURCE_DIR}/main.cpp)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENVR_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glm glfw ${OPENVR_LIBRARIES})