set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Werror")
set(SOURCE_FILES App.cpp)

add_library(Plus7 STATIC ${SOURCE_FILES})

target_include_directories(Plus7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Plus7 ${SDL2_LIBRARIES})