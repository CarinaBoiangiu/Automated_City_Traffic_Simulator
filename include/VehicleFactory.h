#ifndef VEHICLEFACTORY_H_
#define VEHICLEFACTORY_H_

#include "IVehicle.h"
#include <memory>
#include <string>

class IVehicleFactory {
public:
    virtual ~IVehicleFactory() = default;
    virtual std::unique_ptr<IVehicle> createVehicle(const std::string& type) = 0;
};

class CityVehicleFactory : public IVehicleFactory{
    public:
        std::unique_ptr<IVehicle> createVehicle(const std::string& type) override;
};


#endif
