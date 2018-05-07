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
#include "setup.h"

using namespace std;

const int number_max = 100;
const int usernamebox_width = 100;
const int usernamebox_height = 225;

const string config_path = "config.txt"; /* C:\\Users\\wille\\Desktop\\password\\ */
const string pws_path = "pws.txt"; // C:\\Users\\wille\\Desktop\\password\\ */
const string steam_process = "steam.exe";

bool x22 = false;
string path_steam = "C:\\Program Files (x86)\\Steam\\Steam.exe";
string title_window = "Logga in – Steam";
vector<userpw> store;


vector<userpw> &ret()
{
return store;
}

handle_data getProcess(char procName[])
{

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    handle_data data;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    HANDLE hProcess = NULL;

        if (Process32First(snapshot, &entry)){
            while (Process32Next(snapshot,&entry)){
                if (strcmp(entry.szExeFile, procName) == 0){
                    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                    break;
                }
            }
        }
            CloseHandle(snapshot);
    data.window_handle;
    data.process = hProcess;
    data.process_id = entry.th32ProcessID;

    return data;
}


void killProc(HANDLE proc)
{
        if (TerminateProcess(proc,1)){
            WaitForSingleObject(proc,1);
            CloseHandle(proc);
            }
}

bool checkIfProcRunning(char procName[])
{
    while (!getProcess(procName).window_handle)
    {
        checkIfProcRunning(procName);
        return false;
    }
    return true;
}


void getDesktopRes(double& horizontal, double& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}


void sendMessageDelay(HWND handle, string str, uint16_t type_key, uint16_t key, uint16_t delay)
{
    Sleep(delay);
        if (!key)
            for (int i = 0; i!=str.length(); ++i){
                SendMessage(handle,type_key,str[i],0);
            }
            else{
                SendMessage(handle,type_key,key,0);
            }
}


int waiting(int i)
{
    if (i > 0){
        cout << "Please wait for the Steam window to open." << endl;
        Sleep(400);
        --i;
        waiting(i);
    }
    return 0;
}

BOOL is_main_window(HWND handle)
{
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}


BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
    handle_data& data = *(handle_data*)lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId(handle, &process_id);
    if (data.process_id != process_id || !is_main_window(handle))
        return TRUE;
        data.window_handle = handle;
        return FALSE;

}

string getPw(string username)
{
    string pw;
    vector<userpw>::iterator itr = store.begin();
    for (;itr!=store.end();++itr){
        if (username == itr->name){
            return itr->pw;
        }
    }
    return "";
}



HWND find_main_window(unsigned long process_id)
{
    handle_data data;
    data.process_id = process_id;
    data.window_handle = 0;
    EnumWindows(enum_windows_callback, (LPARAM)&data);
}

void startSteam(TCHAR _msg[])
{


        string pw = getPw((string)_msg);
        //cout << "C:\\Program Files (x86)\\Steam\\Steam.exe" << endl;
        cout << path_steam << endl;
        killProc(getProcess("Steam.exe").process);

        SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = NULL;
        ShExecInfo.lpFile = path_steam.c_str();
        ShExecInfo.lpParameters = "";
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.nShow = SW_SHOW;
        ShExecInfo.hInstApp = NULL;
        ShellExecuteEx(&ShExecInfo);

        HWND steam = NULL;
        while(!(steam = FindWindow(NULL,title_window.c_str()))){}

        RECT rect;
        SetForegroundWindow(steam);
        SetFocus(steam);
        SetActiveWindow(steam);
        ShowWindow(steam,SW_SHOW);
        GetWindowRect(steam,&rect);


        double fScreenWidth,fScreenHeight;
        getDesktopRes(fScreenWidth,fScreenHeight);
        double fx = (rect.right-usernamebox_width)*(65535.0f/fScreenWidth);
        double fy = (rect.bottom-usernamebox_height)*(65535.0f/fScreenHeight);

        INPUT event;
        event.type = INPUT_MOUSE;
        event.mi.dx = fx;
        event.mi.dy = fy;
        event.mi.mouseData=0;
        event.mi.time=0;
        event.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE|MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP);
        SendInput(1,&event,sizeof(INPUT));

        for (int i = 1; i!= 100; ++i){
        sendMessageDelay(steam,"",WM_KEYDOWN,VK_BACK,2);
        }
        string details[2] = {(string)_msg, pw};

        sendMessageDelay(steam,details[0],WM_CHAR,0,100);
        sendMessageDelay(steam,"",WM_KEYDOWN,VK_TAB,100);
        sendMessageDelay(steam,details[1],WM_CHAR,0,100);
        sendMessageDelay(steam,details[1],WM_KEYDOWN,VK_RETURN,100);

}


void setup()
{
    string tmp;
    fstream obj(pws_path, fstream::in | fstream::out);
    fstream obj2(config_path, fstream::in);

    while(getline(obj,tmp)){
    size_t found = tmp.find(' ');
    store.push_back(userpw(tmp.substr(0,found),tmp.substr(found+1,tmp.length())));

    }
    uint16_t i = 1;
    while(getline(obj2,tmp)){
        string::size_type found = tmp.find('=');
        string answer;
        if (found != string::npos){
            cout << "hello\n" << found << endl;
            answer = (tmp.substr(found+2,tmp.length()-1));
            cout << "ANS:" << answer << endl;
        }
        switch(i){
    case 1:
        if (answer == "1")
        x22 = true;
        break;
    case 2:
        path_steam = answer;
        break;
    case 3:
        title_window = answer;
     default:
        break;
        }
        ++i;
    }
}
