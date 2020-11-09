#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("NotePad");

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
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

	static char txtLength[10][99];
	static char caretPosX, caretPosY;

	static SIZE txtSize;

	int count = 0;

    switch (message)
    {
    case WM_CREATE:
		caretPosX = caretPosY = 0;
        break;

    // Output Text Data
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        CreateCaret(hWnd, NULL, 1, 20);  // Create cursor
		SetCaretPos(3, 0);						// Set cursor position
        ShowCaret(hWnd);                    // Print cursor

		for (count = 0; count < caretPosY+1; count++)
		{
			GetTextExtentPoint(hdc, txtLength[count], strlen(txtLength[count]), &txtSize);

			TextOut(hdc, 0, count * 20, txtLength[count], strlen(txtLength[count]));

			SetCaretPos(txtSize.cx, count * 20);	// Move caret position
		}

        EndPaint(hWnd, &ps);
        break;

	case WM_CHAR:
		if ((wParam == VK_BACK) && (count > 0))
		{
			count--;
		}
		else if(VK_RETURN)
		{
			if (caretPosY < 9)
			{
				count = 0;
				caretPosX++;
			}
			else
			{
				if (count < 98)
				{
					txtLength[caretPosY][count++] = wParam;
				}
			}
		}

		txtLength[caretPosY][count] = '\0';
		InvalidateRgn(hWnd, NULL, TRUE);
		break;

    case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}