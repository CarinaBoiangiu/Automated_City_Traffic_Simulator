#include "../include/CentralDispatchLogger.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <locale>


CentralDispatchLogger::CentralDispatchLogger() {
    logFile.open("traffic_simulation.log", std::ios::out | std::ios::app);
    if(!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
    }
}

CentralDispatchLogger::~CentralDispatchLogger() {
    if(logFile.is_open()){
        logFile.close();
    }
}

CentralDispatchLogger& CentralDispatchLogger::getInstance() {
    static CentralDispatchLogger instance;
    return instance;
}

void CentralDispatchLogger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::cout<< "[DISPATCH] " << message<<std::endl;
    if(logFile.is_open()){
        logFile<<std::put_time(std::localtime(&now_time),"%H:%M:%S")<<" | "<<message<<std::endl;

        logFile.flush();
    }
}
