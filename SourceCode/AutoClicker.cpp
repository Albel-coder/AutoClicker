#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

// control flags
bool isRunning = false;
bool isMouseClicking = false;
bool isKeyPressing = false;

// configuration
int mouseButton = 0; // 0 - left, 1 - right, 2 - middle
int keyCode = 0x41; // key code (now it is 'A')
int clickInterval = 100; // interval in milliseconds

// function for perform mouse clicks
void performMouseClick(int button) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    switch (button) {
    case 0: // left mouse button
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        sendInput(1, &input, sizeof(INPUT));
        zeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        break;

    case 1: // right mouse button
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        sendInput(1, &input, sizeof(INPUT));
        zeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        break;

    case 2: // middle mouse button
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        sendInput(1, &input, sizeof(INPUT));
        zeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        break;

    }

    sendInput(1, &input, sizeof(INPUT));
}

// function for perform key press
void performKeyPress(int vkCode) {
    INPUT input = { 0 };

    // press key
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;
    sendInput(1, &input, sizeof(INPUT));

    // release key
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    sendInput(1, &input, sizeof(INPUT));
}

// function for more info about program
void showHelp() {
    std::cout << "=== Universal autoclicker and key emulator ===\n";
    std::cout << "F6 - Run/stop mouse autoclicker\n";
    std::cout << "F7 - Run/stop key emulator\n";
    std::cout << "F8 - Change mouse button (left/right/middle)\n";
    std::cout << "F9 - Selecting a key to emulate\n";
    std::cout << "+/- - Increase/decrease interval\n";
    std::cout << "F10 - Exit\n";
    std::cout << "=============================================\n";
}

// function to display the current status
void showStatus() {
    system("cls"); // clear console
    showHelp();

    std::cout << "Current status: \n";
    std::cout << "Mouse autoclicker: " << (IsMouseClicking ? "ON" : "OFF") << "\n";

    std::string buttonName;
    switch (mouseButton) {
    case 0:
        buttonName = "Left button";
        break;
    case 1:
        buttonName = "Right button";
        break;
    case 2:
        buttonName = "Middle button";
        break;
    }

    std::cout << "Mouse button: " << buttonName << "\n";
    std::cout << "Key: " << (char)keyCode << " (0x" << std::hex << keyCode << ")\n";
    std::cout << "Interval: " << std::dec << clickInterval << "milliseconds\n";
    
}

// function for selecting keys
void selectKey()
{
    std::cout << "Enter key char for emulate: ";
    char character;
    std::cin >> character;
    KeyCode = VkKeyScanA(character) & 0xFF; // we get the virtual key code
}

int main() {
    showStatus();

    while (true) {
        // check hot keys
        if (GetAsyncKeyState(VK_F6) & 0x8000) {
            isMouseClicking = !IsMouseClicking;
            showStatus();
            sleep(300);
        }
        if (GetAsyncKeyState(VK_F7) & 0x8000) {
            isKeyPressing = !IsKeyPressing;
            showStatus();
            sleep(300);
        }

        if (GetAsyncKeyState(VK_F8) & 0x8000) {
            mouseButton = (mouseButton + 1) % 3; // Circle 0->1->2->0
            showStatus();
            sleep(300);
        }

        if (GetAsyncKeyState(VK_F9) & 0x8000) {
            selectKey();
            showStatus();
            sleep(300);
        }

        if (GetAsyncKeyState(VK_ADD) & 0x8000) {
            clickInterval += 10;
            showStatus();
            sleep(300);
        }

        if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) {
            clickInterval = (((10) > (clickInterval - 10)) ? (10) : (clickInterval - 10));
            showStatus();
            sleep(300);
        }

        if (GetAsyncKeyState(VK_F10) & 0x8000) {
            break;
        }

        // execute
        if (isMouseClicking) {
            performMouseClick(mouseButton);
        }

        if (isKeyPressing) {
            performKeyPress(keyCode);
        }

        // Sleep
        sleep(clickInterval);
    }

    return 0;
}
