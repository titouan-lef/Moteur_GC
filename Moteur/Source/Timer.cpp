#include "Timer.h"
using namespace std::chrono;

// Constructor for the Timer class
Timer::Timer()
{
    // Initialize the 'last' time point with the current time
    last = steady_clock::now();
}

// Mark the current time and return the time elapsed since the last mark in seconds
float Timer::Mark()
{
    // Store the previous 'last' time point in 'old'
    const auto old = last;

    // Update 'last' with the current time
    last = steady_clock::now();

    // Calculate the time elapsed between 'last' and 'old' in seconds and return it
    const duration<float> frameTime = last - old;
    return frameTime.count();
}

// Peek at the time elapsed since the last mark without resetting the timer
float Timer::Peek()
{
    // Calculate the time elapsed between the current time and 'last' in seconds and return it
    return duration<float>(steady_clock::now() - last).count();
}
