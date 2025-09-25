#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

// control flags
bool IsRunning = false;
bool IsMouseClicking = false;
bool IsKeyPressing = false;

// configuration
int MouseButton = 0; // 0 - left, 1 - right, 2 - middle
int KeyCode = 0x41; // key code (now it is 'A')
int ClickInterval = 100; // interval in milliseconds

// function for perform mouse clicks
void PerformMouseClick(int Button)
{
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    switch (Button)
    {
    case 0: // left mouse button
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        break;

    case 1: // right mouse button
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        break;

    case 2: // middle mouse button
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        SendInput(1, &input, sizeof(INPUT));
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        break;

    }

    SendInput(1, &input, sizeof(INPUT));
}

// function for perform key press
void PerformKeyPress(int vkCode)
{
    INPUT input = { 0 };

    // press key
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;
    SendInput(1, &input, sizeof(INPUT));

    // release key
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

// function for more info about program
void ShowHelp()
{
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
void ShowStatus()
{
    system("cls"); // clear console
    ShowHelp();

    std::cout << "Current status: \n";
    std::cout << "Mouse autoclicker: " << (IsMouseClicking ? "ON" : "OFF") << "\n";

    std::string ButtonName;
    switch (MouseButton)
    {
    case 0:
        ButtonName = "Left button";
        break;
    case 1:
        ButtonName = "Right button";
        break;
    case 2:
        ButtonName = "Middle button";
        break;
    }

    std::cout << "Mouse button: " << ButtonName << "\n";
    std::cout << "Key: " << (char)KeyCode << " (0x" << std::hex << KeyCode << ")\n";
    std::cout << "Interval: " << std::dec << ClickInterval << "milliseconds\n";
    
}

// function for selecting keys
void SelectKey()
{
    std::cout << "Enter key char for emulate: ";
    char ch;
    std::cin >> ch;
    KeyCode = VkKeyScanA(ch) & 0xFF; // we get the virtual key code
}

int main()
{
    ShowStatus();

    while (true)
    {
        // check hot keys
        if (GetAsyncKeyState(VK_F6) & 0x8000) // F6 for mouse
        {
            IsMouseClicking = !IsMouseClicking;
            ShowStatus();
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_F7) & 0x8000) // F7 for keyboard
        {
            IsKeyPressing = !IsKeyPressing;
            ShowStatus();
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_F8) & 0x8000) // F8 for mouse button changing
        {
            MouseButton = (MouseButton + 1) % 3; // Circle 0->1->2->0
            ShowStatus();
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_F9) & 0x8000) // F9 for selecting keys
        {
            SelectKey();
            ShowStatus();
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_ADD) & 0x8000) // + for '+' interval
        {
            ClickInterval += 10;
            ShowStatus();
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) // - for '-' interval
        {
            ClickInterval = (((10) > (ClickInterval - 10)) ? (10) : (ClickInterval - 10));
            ShowStatus();
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_F10) & 0x8000) // F10 for exit
        {
            break;
        }

        // execute
        if (IsMouseClicking)
        {
            PerformMouseClick(MouseButton);
        }

        if (IsKeyPressing)
        {
            PerformKeyPress(KeyCode);
        }

        // Sleep
        Sleep(ClickInterval);
    }

    return 0;
}
