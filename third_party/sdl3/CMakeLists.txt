set(SDL_SHARED OFF)
set(SDL_STATIC ON)

include(FetchContent)

FetchContent_Declare(SDL3
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
    GIT_TAG "a34d31322ce7423b54db29ca6da5883c189b82ee"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL3)
