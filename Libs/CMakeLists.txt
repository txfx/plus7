include(FetchContent)

FetchContent_Declare(
    Imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG        v1.75
    GIT_SHALLOW    TRUE
)
FetchContent_GetProperties(Imgui)
if(NOT Imgui_POPULATED)
  FetchContent_Populate(Imgui)

  file(GLOB IMGUI_SOURCE_FILES ${imgui_SOURCE_DIR}/*.cpp)
  add_library(Imgui STATIC ${IMGUI_SOURCE_FILES})
  target_include_directories(Imgui PUBLIC ${imgui_SOURCE_DIR})
endif()

FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-2.0.10.tar.gz
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        6.2.0
    GIT_SHALLOW    TRUE
)
FetchContent_GetProperties(fmtlib)
if(NOT fmtlib_POPULATED)
  FetchContent_Populate(fmtlib)
  
  add_subdirectory(${fmtlib_SOURCE_DIR} ${fmtlib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()