#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <Windowsx.h>
#include <iostream>
#include "setup.h"

bool is_checked = false;
const int BTN_LOG_IN = 100;
const int BTN_CHECK = 101;
const int BTN_SELECT = 102;
const int ADD_ACCOUNT = 103;
const int REMOVE_ACCOUNT = 104;

HWND hwndButton;
HWND hWndComboBox;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Willes Steam Manager"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           500,                 /* The programs width */
           300,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
     CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
     50, 50, 30, 30, hwndParent, NULL, HINST_THISCOMPONENT,
     NULL); child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );


    hwndButton = CreateWindow(
    "BUTTON",  // Predefined class; Unicode assumed
    "Logga in",      // Button text
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
    200,         // x position
    100,         // y position
    75,        // Button width
    75,        // Button height
    hwnd,     // Parent window
    (HMENU)BTN_LOG_IN,       // No menu.
    (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
    NULL);      // Pointer n

    hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
     CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE ,
     100, 50, 300, 300, hwnd, (HMENU)BTN_SELECT, NULL,
     NULL);


    /* Make the window visible on the screen */

    HWND checkbox = CreateWindow(WC_BUTTON, TEXT("Open with X22"),
       CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
     170, 23, 110, 20, hwnd, (HMENU)BTN_CHECK, NULL,
     NULL);

    setup();
    vector<userpw> hej = ret();
    vector<userpw>::iterator itr = hej.begin();

    cout << hej.size() << endl;
    for (;itr!=hej.end();++itr){
        string tmp = itr->name;
        SendMessage(hWndComboBox,CB_ADDSTRING,0,(LPARAM)tmp.c_str());
    }

    SendMessage(hWndComboBox, CB_SETCURSEL, 0, 0);
    ShowWindow (hwnd, nCmdShow);


    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_COMMAND:
           switch(LOWORD(wParam)){
            case BTN_LOG_IN:
                TCHAR _msg[256];
                GetWindowText(hWndComboBox,_msg,sizeof(_msg));
                startSteam(_msg);
                break;
            case BTN_CHECK:
                if (!Button_GetCheck((HWND)lParam)){
                    Button_SetCheck((HWND)lParam,BST_CHECKED);
                    is_checked = true;
                }
                else{
                    Button_SetCheck((HWND)lParam,BST_UNCHECKED);
                    is_checked = false;
                }
                break;
            case BTN_SELECT:

                break;
           }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
