cmake_minimum_required(VERSION 3.16) 

project(CURVE)

set(PARENT_DIR ${PROJECT_SOURCE_DIR}/..)
set(GCC_COMPILE_FLAGS "-Wall -pedantic -Wextra -g")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
		
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if(WIN32)

  #set(GLFW_INCLUDE_PATH ${PARENT_DIR}/external/glfw-3.3.8.bin.WIN32/include) 
  #set(GLEW_INCLUDE_PATH ${PARENT_DIR}/external/glew-2.1.0/include)

  #set(GLEW_LIBS ${PARENT_DIR}/external/glew-2.1.0/lib/Release/Win32)

  #file(GLOB GLEW_DLLS ${PARENT_DIR}/external/glew-2.1.0/bin/Release/Win32/*.dll)

  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    #set(GLFW_LIBS  ${PARENT_DIR}/external/glfw-3.3.8.bin.WIN32/lib-mingw-w64)
    #file(GLOB GLFW_DLLS ${PARENT_DIR}/external/glfw-3.3.8.bin.WIN32/lib-mingw-w64/*.dll) 
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(GLFW_LIBS  ${PARENT_DIR}/external/glfw-3.3.8.bin.WIN32/lib-vc2022)
    file(GLOB GLFW_DLLS ${PARENT_DIR}/external/glfw-3.3.8.bin.WIN32/lib-vc2022/*.dll) 
  endif()

  #find_library(GLFW glfw3dll PATHS ${GLFW_LIBS})
  #find_library(GLEW glew32 glew32dll PATHS ${GLEW_LIBS})
  find_library(GLFW NAMES glfw3dll glfw3)
  find_library(GLEW NAMES glew32 glew32dll)
  find_library(PTHREAD NAMES pthreads pthread thread)
  find_library(GDI NAMES gdi32)
  find_library(GLU NAMES glu32) 
  find_library(GLUT NAMES freeglut)

  include_directories(${GLEW_INCLUDE_PATH} ${GLFW_INCLUDE_PATH})

  file(COPY ${GLFW_DLLS} ${GLEW_DLLS} DESTINATION ${PROJECT_SOURCE_DIR}/build)
    
endif (WIN32)

if (UNIX)

  find_library(GLFW NAMES glfw3 glfw glfw3dll)
  find_library(GLEW NAMES glew glew2 GLEW)
  find_library(GLUT NAMES glut freeglut freeglut3)
  find_library(PTHREAD NAMES thread pthread threads pthreads)

endif (UNIX)

find_library(OPENGL NAMES opengl32 opengl libopengl OpenGL OpenGL32)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GLUT} ${GDI} ${GLU} ${GLEW} ${OPENGL} ${PTHREAD} )
