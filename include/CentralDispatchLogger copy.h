#ifndef CENTRAL_DISPATCH_LOGGER_H_
#define CENTRAL_DISPATCH_LOGGER_H_

#include <mutex>
#include <string>
#include <fstream>

class CentralDispatchLogger {
public:
    //Disable copy and assignment constructors
    CentralDispatchLogger(const CentralDispatchLogger&) = delete;
    CentralDispatchLogger& operator=(const CentralDispatchLogger&) = delete;

    //Singleton getter
    static CentralDispatchLogger& getInstance();

    void log(const std::string& message);

private:
    CentralDispatchLogger();
    ~CentralDispatchLogger();
    std::mutex logMutex;
    std::ofstream logFile;
};

#endif // CENTRAL_DISPATCH_LOGGER_H
