#ifndef IVEHICLE_H
#define IVEHICLE_H

#include <string>

class IVehicle {
public:
    virtual ~IVehicle() = default;

    virtual void startEngine() = 0;
    virtual void stopForRedLight() = 0;
    virtual void resumeDriving() = 0;
    virtual std::string getId() const = 0;

    // --- NEW THREADING METHODS ---
    // The loop that runs on the thread
    virtual void drivingRoutine() = 0;

    // The kill-switch called by the Facade during shutdown
    virtual void stopEngine() = 0;
};

#endif // IVEHICLE_H
