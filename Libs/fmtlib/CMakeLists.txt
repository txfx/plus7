include(FetchContent)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        8.0.1
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(fmtlib)
if(NOT fmtlib_POPULATED)
  FetchContent_Populate(fmtlib)
  
  add_subdirectory(${fmtlib_SOURCE_DIR} ${fmtlib_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()