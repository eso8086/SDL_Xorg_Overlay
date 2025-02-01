cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(OpenGL)
add_executable(OpenGL main.cpp)

set(SDL_STATIC True)
set(SDL_EXAMPLES True)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
target_link_libraries(OpenGL PRIVATE SDL3::SDL3-static X11 Xext Xfixes)