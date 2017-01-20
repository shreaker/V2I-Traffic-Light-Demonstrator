#ifndef PIN_H
#define PIN_H

#include <mutex>
#include <functional>

class Pin {
public:
    Pin(int number, int mode);
    int read();
    void write(int level);
    enum Pins {
        PIN_CAR_RED = 27,
        PIN_CAR_YELLOW = 4,
        PIN_CAR_GREEN = 17,
        PIN_PEDESTRIAN_RED = 3,
        PIN_PEDESTRIAN_GREEN = 2,
        PIN_BUTTON_LED = 14,
        PIN_BUTTON = 15
    };
    static void resetButton();
    static bool getButtonPressed();
    static void setCallback(std::function<void()> callback);

private:
    int number;
    int mode;
    /*
     * static stuff is ugly, but there is not other option to
     * use a member function as a function pointer for button-ISR
     */
    static bool buttonPressed;
    static bool alreadyInitialized;
    static std::mutex buttonMutex;
    static void buttonInterrupt();
    static std::function<void()> callback;
};

#endif // PIN_H
