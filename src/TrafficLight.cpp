#include "../include/TrafficLight.h"
#include "../include/CentralDispatchLogger.h"
#include <mutex>
#include <thread>
#include <chrono>

TrafficLight::TrafficLight() : isRunning(false), currentColor(LightColor::RED) {}

TrafficLight::~TrafficLight() {
    stop();
}

void TrafficLight::addObserver(std::weak_ptr<IIntersectionObserver> observer) {
    std::lock_guard<std::mutex> lock(observerMutex);
    observers.push_back(observer);
}

void  TrafficLight::stop(){
    isRunning = false;
}

void TrafficLight::notify(LightColor color){
    std::lock_guard<std::mutex> lock(observerMutex);
    for (auto it = observers.rbegin(); it != observers.rend(); ++it) {
        if(auto sharedObs = it->lock()) {
            sharedObs->update(color);
        }else{

        }
    }
}

void TrafficLight::runRoutine() {
    CentralDispatchLogger::getInstance().log("TrafficLight routine started.");

    while (isRunning) {
        // GREEN phase
        currentColor = LightColor::GREEN;
        CentralDispatchLogger::getInstance().log("TrafficLight changed to GREEN.");
        notify(currentColor);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if (!isRunning) break;

        // YELLOW phase
        currentColor = LightColor::YELLOW;
        CentralDispatchLogger::getInstance().log("TrafficLight changed to YELLOW.");
        notify(currentColor);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!isRunning) break;

        // RED phase
        currentColor = LightColor::RED;
        CentralDispatchLogger::getInstance().log("TrafficLight changed to RED.");
        notify(currentColor);
        std::this_thread::sleep_for(std::chrono::seconds(4));
    }

    CentralDispatchLogger::getInstance().log("TrafficLight routine stopped.");
}
