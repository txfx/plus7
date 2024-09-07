include(FetchContent)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        10.2.1
    GIT_SHALLOW    TRUE
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(fmtlib)