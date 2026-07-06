#include "../include/VehicleFactory.h"
#include "../include/TrafficLight.h"
#include "../include/CentralDispatchLogger.h"
#include <memory>
#include <mutex>
#include <stdexcept>
#include <condition_variable>
#include <thread>
#include <utility>

class CivilianCar : public IVehicle, public IIntersectionObserver{
    private:
        std::string id;
        std::mutex driveMutex;
        std::condition_variable driveCv;
        bool isGreenLight = true;
        bool isEngineOn = true;

    public:
        explicit CivilianCar(std::string identifier) : id(std::move(identifier)) {}

        void update(LightColor color) override {
            std::lock_guard<std::mutex> lock(driveMutex);
            if (color == LightColor::GREEN) {
                isGreenLight = true;
                driveCv.notify_one(); // Wake up the driving thread!
            } else if (color == LightColor::RED) {
                isGreenLight = false;
            }

        }
        void drivingRoutine() override{
            while (isEngineOn) {
                // Use unique_lock because condition_variable requires it (it unlocks during wait)
                std::unique_lock<std::mutex> lock(driveMutex);

                // Wait handles Spurious Wakeups via the lambda
                driveCv.wait(lock, [this]{ return isGreenLight || !isEngineOn; });

                if (!isEngineOn) break;

                lock.unlock();
                CentralDispatchLogger::getInstance().log("CivilianCar [" + id + "] is driving through intersection.");
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            CentralDispatchLogger::getInstance().log("CivilianCar [" + id + "] thread ended.");
        }


        void startEngine() override {
            CentralDispatchLogger::getInstance().log("CivilianCar [" + id + "] started engine.");
        }

        void stopForRedLight() override {
            CentralDispatchLogger::getInstance().log("CivilianCar [" + id + "] stopped for red light.");
        }

        void resumeDriving() override {
            CentralDispatchLogger::getInstance().log("CivilianCar [" + id + "] resumed driving.");
        }

        void stopEngine() override{
            {
                std::lock_guard<std::mutex> lock(driveMutex);
                isEngineOn = false;
            }
            driveCv.notify_all();
        }
        std::string getId() const override {
            return id;
        }
};

class Ambulance : public IVehicle, public IIntersectionObserver {
    private:
        std::string id;
        std::mutex driveMutex;
        std::condition_variable driveCv;
        bool isGreenLight = true;
        bool isEngineOn = true;
    public:
        explicit Ambulance(std::string identifier) : id(std::move(identifier)) {}

        void startEngine() override {
            CentralDispatchLogger::getInstance().log("Ambulance [" + id + "] started engine.");
        }

        void stopForRedLight() override {
            CentralDispatchLogger::getInstance().log("Ambulance [" + id + "] stopped for red light.");
        }

        void resumeDriving() override {
            CentralDispatchLogger::getInstance().log("Ambulance [" + id + "] resumed driving.");
        }

        std::string getId() const override {
            return id;
        }
        void update(LightColor color) override {
            std::lock_guard<std::mutex> lock(driveMutex);
            // Note: Ambulances might treat red lights differently in a real simulation,
            // but for thread safety here, we must handle the synchronization.
            if (color == LightColor::GREEN || color == LightColor::YELLOW) {
                isGreenLight = true;
                driveCv.notify_one();
            } else if (color == LightColor::RED) {
                isGreenLight = false;
            }
        }

        // --- IVehicle threading methods ---
        void drivingRoutine() override {
            while (isEngineOn) {
                std::unique_lock<std::mutex> lock(driveMutex);

                driveCv.wait(lock, [this]{ return isGreenLight || !isEngineOn; });

                if (!isEngineOn) break;

                lock.unlock();

                CentralDispatchLogger::getInstance().log("Ambulance [" + id + "] is rushing through intersection.");
                std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Ambulances drive faster (400ms instead of 500ms)
            }
            CentralDispatchLogger::getInstance().log("Ambulance [" + id + "] thread ended.");
        }

        void stopEngine() override {
            {
                std::lock_guard<std::mutex> lock(driveMutex);
                isEngineOn = false;
            }
            driveCv.notify_all(); // Wake up to terminate safely
        }
};

std::unique_ptr<IVehicle> CityVehicleFactory::createVehicle(const  std::string& type) {

    static int vehicleCount = 0;
    vehicleCount++;

    std::string vId = "V-" + std::to_string(vehicleCount);
    if(type == "CivilianCar"){
        return std::make_unique<CivilianCar>(vId);
    } else if(type == "Ambulance"){
        return std::make_unique<Ambulance>(vId);
    }
    throw std::invalid_argument("Unknown vehicle type requested: " + type);
}
