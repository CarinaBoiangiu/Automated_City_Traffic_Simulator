#ifndef CITY_TRAFFIC_FACADE_H
#define CITY_TRAFFIC_FACADE_H

#include "TrafficLight.h"
#include "IVehicle.h"
#include "VehicleFactory.h"
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

class CityTrafficFacade {
public:
    CityTrafficFacade();
    ~CityTrafficFacade();

    // 1. Initialization Method
    void initializeGrid(int numVehicles);

    // 2. Execution Method
    void startSimulation();

    // 3. Shutdown Method
    void shutdown();

private:
    std::shared_ptr<TrafficLight> mainIntersection;
    std::vector<std::shared_ptr<IVehicle>> vehicles;

    // Holding the raw threads
    std::vector<std::thread> simulationThreads;

    CityVehicleFactory factory;
    std::atomic<bool> isSimulationRunning;
};

#endif // CITY_TRAFFIC_FACADE_H
