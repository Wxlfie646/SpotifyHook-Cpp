#pragma region ProcessorDefs
#define _WIN32_WINNT 0x0500
#define WIN32_LEAN_AND_MEAN
#pragma endregion

#pragma region Includes
#include <windows.h>
#include <tlhelp32.h>
#include <conio.h>
#include <atlbase.h>
#pragma endregion

#pragma region References
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#pragma endregion

#pragma region TypeDefs
HWND window_handle;
wchar_t window_title[256];
std::string spotify_type = ""; // Type of Spotify you are using (such as 'Spotify Premium' and 'Spotify')
const wchar_t* window_class = L"Chrome_WidgetWin_0";
std::string window_name = spotify_type;
LPSTR target_window_title;
std::string target_window_name = ""; // Process you want to rename (optional) window name
const wchar_t* target_window_class = L""; // Process you want to rename window class, obtainable through Winlister.
std::string console_title_raw = "<@wxlfie> Spotify hook { Debug Console } | Currently playing -> ";
std::string new_window_title_raw_1 = "<@wxlfie> Spotify hook { ";
std::string new_window_title_raw_2 = target_window_name;
std::string new_window_title_raw_3 = " } | Currently playing -> ";
std::string new_window_title_raw = new_window_title_raw_1 + new_window_title_raw_2 + new_window_title_raw_3;
LPCSTR console_title;
LPCSTR new_window_title;
#pragma endregion

#pragma region ModifiableTypeDefs
std::string current_song = "No song playing/found";
bool isplaying = false;
bool setcontinue = true;
bool isvalidtrack = false;
#pragma endregion

#pragma region Helpers
void console_clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}
void rename_window(LPCSTR input) {
    if (input = "") return;
    if (!FindWindowW(target_window_class, NULL)) {
        std::cout << "Window not found!" << std::endl;
        return;
    }
    HWND window = FindWindowW(target_window_class, NULL);
    SetWindowTextA(window, input);
    GetWindowTextA(window, target_window_title, 256);
    std::cout << "Renaming " << target_window_name << "'s window." << std::endl;

}
#pragma endregion 

#pragma region Routines
void init() {
    console_clear();
}

void fetch_data() {
    std::cout << "Fetching 'Spotify' data." << std::endl;
    window_handle = FindWindowW(window_class, NULL);
    if (IsWindowVisible(window_handle)) {
        GetWindowTextW(window_handle, window_title, sizeof(window_title));
    }
    else std::cout << "Spotify may be minimised." << std::endl;
}

void check_data() {
    std::wstring window_title_raw(window_title);
    std::string current_song_raw(window_title_raw.begin(), window_title_raw.end());

    if (current_song_raw == spotify_type || current_song_raw == "") current_song = "No song playing/found";
    else current_song = current_song_raw; return;
}

void update() {
    #pragma region Console
    std::string console_title_basic = console_title_raw + current_song;
    console_title = console_title_basic.c_str();
    SetConsoleTitleA(console_title);
    #pragma endregion
    #pragma region Window
    std::string window_title_basic = new_window_title_raw + current_song;
    new_window_title = window_title_basic.c_str();
    rename_window(new_window_title);
    #pragma endregion
}

void return_data() {
    std::cout << "Returned data ----------------" << std::endl;
    std::cout << "Window handle ==> " << window_handle << std::endl;
    std::wcout << "Window class ==> " << window_class << std::endl;
    std::cout << "Window title ==> " << window_name << std::endl;
    std::cout << "Song title ==> " << current_song << std::endl;
    std::cout << "------------------------------" << std::endl;
}
#pragma endregion

int main()
{
    while (setcontinue) {
        init();
        fetch_data();
        check_data();
        return_data();
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1700));
    }
}


