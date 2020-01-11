set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCE_FILES *.c *.cpp *.hpp)
add_library(Plus7 STATIC ${SOURCE_FILES})
target_compile_features(Plus7 PUBLIC cxx_std_17)
target_compile_options(Plus7 PRIVATE
    $<$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang,GNU>:
        -Wall -Wpedantic -Wextra -Werror -Wnon-virtual-dtor -Wshadow -Wunused -Woverloaded-virtual -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wno-unknown-warning-option
    >
    $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:
        /W3 /WX /wd4244 /wd4312
    >)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(Plus7 PUBLIC SDL2 ${OPENGL_LIBRARIES} Imgui)

target_include_directories(Plus7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} Imgui)