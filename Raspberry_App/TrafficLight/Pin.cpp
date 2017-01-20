#include "Pin.h"
#include <wiringPi.h>
#include <iostream>
#include <chrono>

#include "Settings.h"

bool Pin::alreadyInitialized = false;
bool Pin::buttonPressed = false;
std::mutex Pin::buttonMutex;
std::function<void()> Pin::callback;

Pin::Pin(int number, int mode) : number(number), mode(mode) {
    if(!alreadyInitialized) {
        wiringPiSetupGpio();
        alreadyInitialized = true;
    }

    if(mode == OUTPUT) {
        digitalWrite(number, HIGH);
        pinMode(number, OUTPUT);
    } else if(mode == INPUT) {
        pinMode(number, INPUT);
        pullUpDnControl(number, PUD_UP);
        wiringPiISR(number, INT_EDGE_FALLING, &Pin::buttonInterrupt);
    } else {
        std::clog << "invalid pin mode" << std::endl;
        std::abort();
    }
}

int Pin::read() {
    return digitalRead(number);
}

void Pin::write(int level) {
    if(mode == OUTPUT) {
        digitalWrite(number, level);
    } else {
        std::clog << "can not set level of input pin: " << number << std::endl;
        std::abort();
    }
}

void Pin::resetButton() {
    std::lock_guard<std::mutex> lock(buttonMutex);
    buttonPressed = false;
}

bool Pin::getButtonPressed() {
    std::lock_guard<std::mutex> lock(buttonMutex);
    return buttonPressed;
}

void Pin::setCallback(std::function<void ()> callback) {
    Pin::callback = callback;
}

void Pin::buttonInterrupt() {
    //debounce button with delay
    static auto timestamp = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(BUTTON_PRESS_DELAY);

    if(buttonMutex.try_lock()) {
        int diffInMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timestamp).count();

        if(diffInMs >= BUTTON_PRESS_DELAY) {
            timestamp = std::chrono::high_resolution_clock::now();
            buttonPressed = true;
            std::clog << "button pressed" << std::endl;
            callback();
        }

        buttonMutex.unlock();
    }
}
