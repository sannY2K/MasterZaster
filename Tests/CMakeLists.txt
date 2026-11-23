include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.4
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(Catch2)

add_executable(MZCoreTests
    DecibelsTests.cpp
    MeteringTests.cpp
)
target_link_libraries(MZCoreTests PRIVATE MZCore Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(MZCoreTests)
