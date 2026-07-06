#include "../include/CityTrafficFacede.h"
#include <thread>
#include <chrono>

int main() {
    CityTrafficFacade simulator;

    // 1. Setup
    simulator.initializeGrid(10); // 10 vehicles, 1 light

    // 2. Execute
    simulator.startSimulation();

    // Let the chaotic multithreaded simulation run for 15 seconds
    std::this_thread::sleep_for(std::chrono::seconds(15));

    // 3. Teardown
    simulator.shutdown();

    return 0;
}
