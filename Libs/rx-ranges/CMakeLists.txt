include(FetchContent)

FetchContent_Declare(
    rx-ranges
    GIT_REPOSITORY  https://github.com/simonask/rx-ranges
    GIT_SHALLOW     TRUE
    PATCH_COMMAND   git restore . && git apply ${CMAKE_CURRENT_SOURCE_DIR}/includes.patch
)

FetchContent_MakeAvailable(rx-ranges)