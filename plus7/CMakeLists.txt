find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Plus7 App.cpp)
target_include_directories (Plus7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})