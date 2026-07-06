#include "../include/CityTrafficFacede.h"
#include "../include/CentralDispatchLogger.h"
#include <iostream>

CityTrafficFacade::CityTrafficFacade() : isSimulationRunning(false) {
    mainIntersection = std::make_shared<TrafficLight>();
}

CityTrafficFacade::~CityTrafficFacade() {
    // Ensuring safety: if the user forgets to call shutdown, the destructor catches it.
    if (isSimulationRunning) {
        shutdown();
    }
}

void CityTrafficFacade::initializeGrid(int numVehicles) {
    CentralDispatchLogger::getInstance().log("Initializing City Grid with " + std::to_string(numVehicles) + " vehicles.");

    for (int i = 0; i < numVehicles; ++i) {
        // Create vehicle (mixing CivilianCars and Ambulances just as an example)
        std::string type = (i % 5 == 0) ? "Ambulance" : "CivilianCar";

        // Upgrade unique_ptr to shared_ptr so Facade and TrafficLight can both hold references safely
        std::shared_ptr<IVehicle> v = factory.createVehicle(type);
        vehicles.push_back(v);

        // Cast to observer and attach to the traffic light
        auto observer = std::dynamic_pointer_cast<IIntersectionObserver>(v);
        if (observer) {
            mainIntersection->addObserver(observer);
        }
    }
}

void CityTrafficFacade::startSimulation() {
    isSimulationRunning = true;
    CentralDispatchLogger::getInstance().log("--- STARTING SIMULATION ---");

    // 1. Spin up the Traffic Light thread
    // We pass the member function pointer and the object instance it operates on
    simulationThreads.emplace_back(&TrafficLight::runRoutine, mainIntersection.get());

    // 2. Spin up the driving threads for all vehicles
    for (auto& vehicle : vehicles) {
        simulationThreads.emplace_back(&IVehicle::drivingRoutine, vehicle.get());
    }
}

void CityTrafficFacade::shutdown() {
    if (!isSimulationRunning) return;

    CentralDispatchLogger::getInstance().log("--- INITIATING SHUTDOWN ---");

    // 1. Signal all objects to break their while-loops and wake up condition variables
    mainIntersection->stop();
    for (auto& vehicle : vehicles) {
        vehicle->stopEngine(); // This must set isEngineOn = false and call cv.notify_all() inside the vehicle!
    }

    // 2. The most critical step: Join all threads safely
    for (auto& t : simulationThreads) {
        if (t.joinable()) {
            t.join();
        }
    }

    isSimulationRunning = false;
    CentralDispatchLogger::getInstance().log("Simulation shut down gracefully. All threads joined.");
}
