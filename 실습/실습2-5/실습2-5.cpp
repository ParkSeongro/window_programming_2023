#include <tchar.h>
#include <windows.h>  //--- 윈도우 헤더 파일
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 2-5";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdParam, int nCmdShow) {
  HWND hWnd;
  MSG Message;
  WNDCLASSEX WndClass;
  g_hInst = hInstance;
  WndClass.cbSize = sizeof(WndClass);
  WndClass.style = CS_HREDRAW | CS_VREDRAW;
  WndClass.lpfnWndProc = (WNDPROC)WndProc;
  WndClass.cbClsExtra = 0;
  WndClass.cbWndExtra = 0;
  WndClass.hInstance = hInstance;
  WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  WndClass.lpszMenuName = NULL;
  WndClass.lpszClassName = lpszClass;
  WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  RegisterClassEx(&WndClass);
  hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 50,
                      WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance,
                      NULL);
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  while (GetMessage(&Message, 0, 0, 0)) {
    TranslateMessage(&Message);
    DispatchMessage(&Message);
  }
  return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  const int max_row = 10; // 30x10
  const int max_col = 30;
  PAINTSTRUCT ps;
  HDC hDC;
  SIZE size;
  static TCHAR str[max_row][max_col + 1];
  static int row;
  static int col;

  switch (uMsg) {
    case WM_CREATE:  
     row = 0;
      col = 0;
     CreateCaret(hWnd, NULL, 5, 15);
      ShowCaret(hWnd);
      break;

    case WM_CHAR:
      switch (wParam) {
        case VK_BACK:

          if (col > 0) {
            --col;
            /*for (int i = col; i < lstrlen(str[row]); ++i)
              str[row][i] = str[row][i + 1];     */

            if (col == 0 && row > 0) {
              --row;
              col = max_col;
            }
            break;
            case VK_RETURN:
              ++row;
              if (row == max_row) row = 0;
              col = 0;
              break;
            default:
              if (col == max_col) {
                ++row;
                col = 0;
              }
              if (row == max_row) row = 0;
              str[row][col++] = wParam;
              break;
          }
      }
          
      InvalidateRect(hWnd, NULL, TRUE);
      break;

    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      GetTextExtentPoint32(hDC, str[row], col, &size);
      SetCaretPos(size.cx, row * 16);
      for (int i = 0; i < max_row; ++i)        
        TextOut(hDC, 0, i * 16, str[i], lstrlen(str[i]));
      
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      HideCaret(hWnd);
      DestroyCaret();
      PostQuitMessage(0);
      break;
  }
  return DefWindowProc(hWnd, uMsg, wParam,
                       lParam);  //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}