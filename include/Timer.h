#pragma once 
#include <chrono>

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    int elapsed() const { 
        return std::chrono::duration_cast<mili_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<int, std::milli > mili_;
    std::chrono::time_point<clock_> beg_;
};
