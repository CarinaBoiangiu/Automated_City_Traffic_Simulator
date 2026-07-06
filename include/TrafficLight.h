#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "IIntersectionObserver.h"
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>


class TrafficLight {
    public:
        TrafficLight();
        ~TrafficLight();

        void addObserver(std::weak_ptr<IIntersectionObserver> observer);
        void runRoutine();
        void stop();
    private:
        void notify(LightColor color);
        std::vector<std::weak_ptr<IIntersectionObserver>> observers;
        std::mutex observerMutex;
        std::atomic<bool> isRunning;
        LightColor currentColor;
};

#endif // TRAFFICLIGHT_H
