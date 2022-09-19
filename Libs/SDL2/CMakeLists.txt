include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL
    GIT_SHALLOW     TRUE
    GIT_TAG         main
)

FetchContent_MakeAvailable(SDL2)