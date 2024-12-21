cmake_minimum_required(VERSION 3.30)
project(Coursework_7sem)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(TESTS_GENERATOR
        Utils/filesystem.cpp
        DataGenerator/FixedWorkers/fixed_workers_generator.cpp
        DataGenerator/Setups/MetaInfo/utils.cpp
        Utils/Random/random_event_generator.cpp
        Schedulers/Offline/offline_scheduler.cpp
        Schedulers/Mock/mock_scheduler.cpp
        DataGenerator/main.cpp)

add_executable(SETUP_GENERATOR
        Utils/filesystem.cpp
        Utils/Random/random_event_generator.cpp
        main.cpp)

add_executable(COMMON_TEST
        Utils/filesystem.cpp
        Schedulers/Offline/offline_scheduler.cpp
        Schedulers/Offline/offline_scheduler_tests.cpp
)

target_link_libraries(COMMON_TEST gtest_main)

add_executable(METRICS
        Metrics/FirstFail/main.cpp
        Utils/filesystem.cpp
        DataGenerator/Setups/MetaInfo/utils.cpp
        Metrics/FirstFail/utils.cpp
        Metrics/FirstFail/first_fail.cpp
        Schedulers/Mock/mock_scheduler.cpp
        Schedulers/Version1/scheduler_v1.cpp
        Schedulers/Version2/scheduler_v2.cpp
        Schedulers/Version3/scheduler_v3.cpp
)

#add_executable(BENCH
#        Benchmarks/bench.cpp
#        Benchmarks/benchmarks.cpp
#)
#
#add_executable(HELPER
#        Utils/array_builder.cpp
#)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
