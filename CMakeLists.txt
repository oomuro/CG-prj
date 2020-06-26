cmake_minimum_required(VERSION 3.17)
project(CG-prj)

set(CMAKE_CXX_STANDARD 11)

set(FREEGLUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/freeglut")
set(GLEW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew")
set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glm")

include_directories(
        ${FREEGLUT_DIR}/include
        ${GLEW_DIR}/include
        ${GLM_DIR}
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")

link_directories(
        ${FREEGLUT_DIR}/lib
        ${GLEW_DIR}/lib
)

link_libraries(
        opengl32.lib
        glu32.lib
        freeglut_static.lib
        glew32s.lib
)

add_compile_definitions(FREEGLUT_STATIC)
add_compile_definitions(GLEW_STATIC)

add_executable(first WIN32 src/sample/first_sample.cpp)
add_executable(second src/sample/second_sample.cpp)
add_executable(hw3 src/hw3/hw3.cpp)
add_executable(hw5 src/hw5/hw5.cpp)
add_executable(hw6_1 src/hw6/6_1/hw6_1.cpp)
add_executable(hw6_2 src/hw6/6_2/hw6_2.cpp)
add_executable(hw7 src/hw7/hw7.cpp)
add_executable(hw8 src/hw8/hw8.cpp)
add_executable(hw9 src/hw9/hw9.cpp)
add_executable(hw10 src/hw10/hw10.cpp)
add_executable(hw11_1 src/hw11/11_1/hw11_1.cpp)
add_executable(hw11_2 src/hw11/11_2/hw11_2.cpp)