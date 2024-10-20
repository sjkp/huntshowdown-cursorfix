#include <windows.h>
#include <iostream>

#define HOTKEY_ID 1

BOOL g_cursorVisible = TRUE;

// Function to enumerate processes and attach input
void AttachToProcess(const char* windowName) {
    // Find the window handle of the target process
    HWND hwnd = FindWindow(NULL, windowName);
    if (hwnd == NULL) {        
        //MessageBox(orgHwnd, "Hotkey pressed!", "Failed to find window with name: ", MB_OK);
        return;
    }

    // Get the thread process ID
    DWORD processId;
    DWORD threadId = GetWindowThreadProcessId(hwnd, &processId);

    if (threadId == 0) {
        std::cerr << "Failed to get thread process ID." << std::endl;
    } else {
        std::cout << "Thread ID: " << threadId << ", Process ID: " << processId << std::endl;
    }

    DWORD currentThreadId = GetCurrentThreadId();
    if (AttachThreadInput(currentThreadId, threadId, TRUE)) {
        std::cout << "Successfully attached input to thread ID: " << threadId << std::endl;

        // Find the main window of the target process
       

            // Hide the cursor in the target process
            SendMessage(hwnd, WM_SETCURSOR, (WPARAM)hwnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
            ShowCursor(g_cursorVisible);
            std::cout << "Cursor hidden in the target process." << std::endl;
        
    } else {
        std::cerr << "Failed to attach input to thread ID: " << threadId << std::endl;
    }

}


// Window Procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }
        return 0;
        case WM_HOTKEY:
           if (wParam == HOTKEY_ID) {
               //MessageBox(hwnd, "Hotkey pressed!", "Hotkey", MB_OK);
               g_cursorVisible = !g_cursorVisible;
               ShowCursor(g_cursorVisible);
                const char* targetProcessName = "Hunt: Showdown"; // Change this to the target process name
                AttachToProcess(targetProcessName);
           }
           return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// WinMain function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Hunt - Cursor Fix",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

   

    ShowWindow(hwnd, nCmdShow);

     // Register the global hotkey (Ctrl + Shift + H)
    if (!RegisterHotKey(hwnd, HOTKEY_ID, MOD_CONTROL | MOD_SHIFT, 'H')) {
        MessageBox(hwnd, "Failed to register hotkey 123", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

     // Unregister the hotkey
    UnregisterHotKey(hwnd, HOTKEY_ID);

    return 0;
}