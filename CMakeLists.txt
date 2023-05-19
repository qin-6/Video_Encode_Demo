cmake_minimum_required(VERSION 3.18)

set(CMAKE_C_COMPILER clang-10)
set(CMAKE_CXX_COMPILER clang++-10)
#set(CMAKE_CXX_FLAGS -stdlib=libc++)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -fPIC")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fPIC")

project(server 
        VERSION 2.4.0
        LANGUAGES C CXX ASM)

include_directories(./inc/glad
                    ./inc/glfw-3.3.2/include
                    ./inc/streaming
                    ./inc/image
                    ./inc/learnopengl
                    ./inc/glm
                    ./inc/avcenc)

set(GLFW_BUILD_DOCS        OFF CACHE BOOL "" FORCE) 
set(GLFW_BUILD_TESTS       OFF CACHE BOOL "" FORCE) 
set(GLFW_BUILD_EXAMPLES    OFF CACHE BOOL "" FORCE) 
set(GLFW_INSTALL           OFF CACHE BOOL "" FORCE) 
add_definitions(-DGLFW_USE_EGL) 
add_subdirectory(glfw-3.3.2) 
link_libraries(glfw)
                    

set(PARSE_FILES ./main.cpp ./glad.c ./stb_image.cpp)
add_executable(server ${PARSE_FILES})
target_link_directories(server PRIVATE ./lib)
target_link_libraries(server
dl
z
)
