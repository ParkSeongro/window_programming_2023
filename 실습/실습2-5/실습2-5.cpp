#include <tchar.h>
#include <windows.h>  //--- 윈도우 헤더 파일
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 2-5";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);

class StrPos {
 public:
  int x, y;
  StrPos() : x(0), y(0) {}
};

class CaretPos {
 public:
  int x, y;
  CaretPos() : x(1), y(0) {}
};


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
  PAINTSTRUCT ps;
  HDC hDC;
  SIZE size;
  static TCHAR str[1000];
  static int count;
  static bool isEnter;
  static StrPos strPos;
  static CaretPos caretPos;
  static int row;

  switch (uMsg) {
    case WM_CREATE:  
      count = 0;
      row = 0;
      isEnter = false;
      CreateCaret(hWnd, NULL, 5,
                  15);  //--- 캐럿 만들기
      ShowCaret(hWnd);
      break;

    case WM_CHAR:
      if (wParam == VK_BACK) {
        if (count > 0) --count;
      } else if (wParam == VK_RETURN) {
        count = 0;
        ++row;
        strPos.y += 15;
        caretPos.y += 15;
      } else {
        if (count == 30) {
          count = 0;
          ++row;
          strPos.y += 15;
          caretPos.y += 15;
        }        
        if (row >= 10) {
          count = 0;
          strPos.y = 0;
          caretPos.y = 0;
          row = 0;
        }
        str[count++] = wParam;
      }     
      str[count] = '\0';
      InvalidateRect(hWnd, NULL, false);
      break;

    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      GetTextExtentPoint32(hDC, str, lstrlen(str),
                           &size);  //--- 문자열 길이 알아내기
      
      TextOut(hDC, 0, strPos.y, str, lstrlen(str));
      SetCaretPos(size.cx, caretPos.y);
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      HideCaret(hWnd);  //--- 캐럿 숨기기
      DestroyCaret();   //--- 캐럿 삭제하기
      PostQuitMessage(0);
      break;
  }
  return DefWindowProc(hWnd, uMsg, wParam,
                       lParam);  //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}