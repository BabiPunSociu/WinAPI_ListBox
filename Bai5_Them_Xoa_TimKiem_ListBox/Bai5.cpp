
#include "framework.h"
#include "Bai5.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND stHoten, edtHoten, stDS, lstDS, btnAdd, btnRemove, btnSearch, btnExit;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BAI5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BAI5));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BAI5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int width, height;
    static TCHAR string[100];
    static int index;
    switch (message)
    {
    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        break;
    case WM_CREATE:
        stHoten = CreateWindow(L"static", L"Họ và tên", WS_CHILD | WS_VISIBLE |SS_CENTER , 10, 10, 150, 30, hWnd, NULL, NULL, NULL);
        edtHoten = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 165, 10, 428, 30, hWnd, (HMENU)1,NULL,NULL);                       // Id edtHoten =  1
        stDS = CreateWindow(L"static", L"Danh sách sinh viên:", WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 45 , 583, 30, hWnd, NULL, NULL, NULL);
        lstDS = CreateWindow(L"listbox", L"", WS_CHILD | WS_VISIBLE |LBS_SORT|WS_VSCROLL | WS_BORDER, 10, 80, 583, 420, hWnd, (HMENU)2, NULL, NULL);                      // Id lstDS =  2
        btnAdd = CreateWindow(L"button", L"Thêm mới", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 505, 142, 30, hWnd, (HMENU)3, NULL, NULL);         // Id add: 3
        btnRemove = CreateWindow(L"button", L"Xóa", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 157, 505, 142, 30, hWnd, (HMENU)4, NULL, NULL);          // Id remove: 4
        btnSearch = CreateWindow(L"button", L"Tìm kiếm", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 304, 505, 142, 30, hWnd, (HMENU) 5, NULL, NULL);    // Id search: 5
        btnExit = CreateWindow(L"button", L"Thoát", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 451, 505, 142, 30, hWnd, (HMENU) 6, NULL, NULL);         // Id exit: 6
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case 3: // add
                if (GetWindowText(edtHoten, string, 100) == 0)
                {
                    MessageBox(hWnd, L"Bạn chưa nhập họ và tên sinh viên", L"Nhắc nhở", MB_OK);
                    break;
                }
                SendMessage(lstDS, LB_ADDSTRING, 0, (LPARAM)string);    // Dành cho LBS_SORT
            //    SendMessage(lstDS, LB_INSERTSTRING, 0, (LPARAM)string); // Dành cho listbox thường, vẫn dùng được cho LBS_SORT nhưng sẽ ko được SORT
                wsprintf(string, L"");
                SetWindowText(edtHoten, string);
                SetFocus(edtHoten);
                break;
            case 4: // remove
                index = (int)SendMessage(lstDS, LB_GETCURSEL, 0, 0);
                if(MessageBox(hWnd, L"Bạn chắc chắn muốn xóa sinh viên ?",L"Cảnh báo", MB_YESNO) == IDYES)
                    SendMessage(lstDS, LB_DELETESTRING, index, 0);
                break;
            case 5: // Search
                if (GetWindowText(edtHoten, string, 100) == 0)
                {
                    MessageBox(hWnd, L"Bạn chưa nhập họ và tên sinh viên", L"Nhắc nhở", MB_OK);
                    break;
                }
                index = (int)SendMessage(lstDS, LB_FINDSTRING, 0, (LPARAM)string);
                SendMessage(lstDS, LB_SETCURSEL, index, (LPARAM)string);
                break;
            case 6: // Exit
                if (MessageBox(hWnd, L"Bạn muốn thoát không?", L"Cảnh báo", MB_YESNO) == IDYES)
                    PostQuitMessage(1);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CLOSE:
        SendMessage(btnExit, BM_CLICK, 1, 0);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
