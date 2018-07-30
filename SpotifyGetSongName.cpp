/*
     Note:
      We can detect Spotify only when music is paused
      It runs in a backgroud (window is not visible)
      It quits automatically if the second program is not running
      It saves the name to data.dll file

      For more informations please check readme file


      Author: Paweł Wrzesień
*/


#include <windows.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <string>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <windows.h>
#include <tlhelp32.h>


const LPCTSTR windowClassName = "";
HWND procWindow;
bool SpotifyFound = false;
bool MotherExeFound = false;
std::string motherExeName;

void init() {
  std::ofstream out("data.dll");
  out << "";
  out.close();
  return;
}

// Search for spotify and motherExe
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  if (IsWindowVisible(hwnd)) { // check whether window is visible
    char wnd_title[256];
    GetWindowText(hwnd, wnd_title, sizeof(wnd_title));

    if (SpotifyFound == 0) {
      if (strcmp(wnd_title, "Spotify") == 0) { // it has title "Spotify" only when music is paused
        procWindow = hwnd;
        std::cout << "Spotify has been found!" << std::endl;
        SpotifyFound = true;
      }
    }

    if (MotherExeFound == 0) {
      if (strcmp(wnd_title, "Computer to Arduino") == 0) {
        // std::cout << "Mother exe found!" << std::endl;
        MotherExeFound = true;
      }
    }

    if (SpotifyFound && MotherExeFound) // we return false to get out of CALLBACK
      return false;

  }
  return true; // function must return true if you want to continue enumeration
}

// Update string when new song is playing
void main_activity() {
  TCHAR previous_song[255] = "Paused";
  TCHAR current_song[255] = " ";

  for (;;) {
    GetWindowText(procWindow, current_song, 255);
    if (strcmp(current_song, "Spotify") == 0)
      strcpy(current_song, "Paused");

    if (strcmp(previous_song, current_song) != 0) { // if song is changed
      strcpy(previous_song, current_song);

      std::cout << current_song << std::endl;
      std::ofstream out("data.dll");
      out << current_song;
      out.close();
    }

    // we check if mother exe is still running
    MotherExeFound = 0;
    EnumWindows(EnumWindowsProc, 0);
    if (!MotherExeFound) {
      return; // exit this function if motherExe is not running
    }

    Sleep(1000);

  }
  return;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow) {

  ShowWindow( GetConsoleWindow(), SW_HIDE );  // Hide terminal

  void init();

  EnumWindows(EnumWindowsProc, 0); // search for spotify and motherExe procWindow

  if (!MotherExeFound) { // to avoid zombie process to works only if mother exe is running
    std::cout << "Mother exe is not running" << std::endl;
    std::cout << "Exiting" << std::endl;
    return 0;
  }

  while (!SpotifyFound) { // we are detecting Spotify window untill we find it
    MotherExeFound = 0;
    EnumWindows(EnumWindowsProc, 0); // Search for Spotifys procWindow and check if mother exe is still running
    if (!MotherExeFound) { // if mother exe is closed then we close this program
      std::cout << "Mother exe closed" << std::endl;
      std::cout << "Exiting" << std::endl;
      return 0;
    }
  }

  main_activity(); // Update string when new song is playing

  return 0;
}
