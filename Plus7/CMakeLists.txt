set(SOURCE_FILES App.cpp Module/Gfx/SdlGl/SdlGlRenderer.cpp)
add_library(Plus7 STATIC ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
target_link_libraries(Plus7 ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Werror")

target_include_directories(Plus7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})