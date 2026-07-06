cmake_minimum_required(VERSION 3.15)
project(CityTrafficSimulator VERSION 1.0)

# Enforce C++20 to safely utilize std::jthread and modern concurrency features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Specify where the compiler should look for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Explicitly list source files
set(SOURCES
    src/main.cpp
    src/CentralDispatchLogger.cpp
    src/VehicleFactory.cpp
    src/TrafficLight.cpp
    src/CityTrafficFacade.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find and link the threading library safely
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
