include(FetchContent)
FetchContent_Declare(tig
    GIT_REPOSITORY "https://github.com/alexbatalov/tig-dev"
    GIT_TAG "main"
)

set(TIG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TIG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(tig)
