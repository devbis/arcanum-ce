include(FetchContent)
FetchContent_Declare(tig
    GIT_REPOSITORY "https://github.com/alexbatalov/tig"
    GIT_TAG "main"
)

set(TIG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TIG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(tig)

set(TIG_LIBRARY "tig" PARENT_SCOPE)
set(TIG_INCLUDE_DIR "${tig_SOURCE_DIR}/include" PARENT_SCOPE)
