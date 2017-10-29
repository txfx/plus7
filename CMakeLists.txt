project(Plus7)

find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HelloWorld samples/HelloWorld.cpp)