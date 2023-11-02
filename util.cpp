#include <iostream>
#include <chrono>
#include <ctime>

void GetTime() {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 获取当前时间的时、分、秒
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);

    // 获取当前时间的毫秒和微秒
    auto sinceEpoch = now.time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count() % 1000;
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(sinceEpoch).count() % 1000;

    // 打印当前时间
    std::cout << "时: " << localTime->tm_hour << ", "
              << "分: " << localTime->tm_min << ", "
              << "秒: " << localTime->tm_sec << ", "
              << "毫秒: " << ms << ", "
              << "微秒: " << us << std::endl;

    
}
