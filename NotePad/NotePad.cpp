#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("NotePad");

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance
, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) 
    {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    TCHAR txtArray[1024] = "\0";

    static char testText[256] = "\0";
    static int txtColumn = 2;                  // Text column
    static int txtWidth = 2;                    // Text width
    static int x, y;                                // set TextOut() position

    switch (message)
    {
    // Input keyboard value
    case WM_CHAR:
        txtColumn = strlen(testText);
        //txtWidth = 1;
        testText[txtColumn] = (TCHAR)wParam;
        testText[txtColumn + 1] = 0;

        // Go to next width
        if (wParam == VK_RETURN)
        {
            txtColumn -= 1;
            y += 8;
        }
        // Delete text column
        else if (wParam == VK_BACK && txtColumn)
        {
            txtColumn -= 2;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    // Output Text Data
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        CreateCaret(hWnd, NULL, 3, 10); // Create Cursor

        // Rect
        
        TextOut(hdc, 0, 0, txtArray, lstrlen(txtArray));

        for (int width = 0; width < txtWidth; width++)
        {
            TextOut(hdc, 0, y, testText, strlen(testText));
        }
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}