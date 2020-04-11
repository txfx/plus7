set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options("$<$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang,GNU>:-Wall;-Wpedantic;-Wextra;-Werror;-Wnon-virtual-dtor;-Wshadow;-Wunused;-Woverloaded-virtual;-Wmisleading-indentation;-Wduplicated-cond;-Wduplicated-branches;-Wlogical-op;-Wnull-dereference;-Wuseless-cast;-Wdouble-promotion;-Wno-unknown-warning-option>")
add_compile_options("$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/W3;/WX;/wd4244;/wd4312>")

add_subdirectory(Core)
add_subdirectory(Modules)