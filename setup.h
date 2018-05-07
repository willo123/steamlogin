#ifndef SETUP_H
#define SETUP_H

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <shellapi.h>
#include <Lmcons.h>
#include <cstring>
#include <tlhelp32.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
using namespace std;


struct handle_data {
    unsigned long process_id;
    HWND window_handle;
    HANDLE process;
};

struct userpw{
    userpw(string _name, string string_pw):
        name(_name),
        pw(string_pw) {}

    string name;
    string pw;
};

    vector<userpw> &ret();
    string getPw();
    void setup();
    handle_data getProcess(char procName[]);
    void startSteam(TCHAR _msg[]);
    void killProc(HANDLE proc);
    bool checkIfProcRunning(char procName[]);
    void getDesktopRes(double& horizontal, double& vertical);
    void sendMessageDelay(HWND handle, string str, uint16_t type_key, uint16_t key, uint16_t delay);




#define SETUP_H
#endif
