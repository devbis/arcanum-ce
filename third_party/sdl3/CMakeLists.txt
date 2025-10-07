set(SDL_SHARED OFF)
set(SDL_STATIC ON)

include(FetchContent)

FetchContent_Declare(SDL3
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
    GIT_TAG "f66e4d0a4391194e7e95de3ab629eb7e23812b29"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL3)
