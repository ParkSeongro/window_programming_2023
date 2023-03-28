#include <tchar.h>
#include <windows.h>  //--- 윈도우 헤더 파일
#include <cstring>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 2-4";
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
  PAINTSTRUCT ps;
  HDC hDC;
  SIZE size;
  static int count;
  int i;
  static TCHAR str[100];
  TCHAR lpOut[15];
  int num[4];
  char cStr[100];
  char* ptr;
  static int x, y, table, operand;
  static bool isEnter;
  
  switch (uMsg) {
    case WM_CREATE:
      CreateCaret(hWnd, NULL, 5, 15);  //--- 캐럿 만들기
      ShowCaret(hWnd);  //--- 빈 화면에 캐럿 표시 count = 0;
      count = 0;
      break;

    case WM_CHAR:
      if (wParam == VK_BACK) {
        if (count > 0) str[--count] = '\0';
      } else if (wParam == VK_RETURN) {
        if (str[0] == L'0') PostQuitMessage(0); 
        isEnter = true;
        WideCharToMultiByte(CP_ACP, 0, str, lstrlen(str), cStr, 100, NULL,
                            NULL); // strtok() 함수 사용을 위해 TCHAR -> CHAR 변환
        ptr = strtok(cStr, " ");
        i = 0;
        while (ptr != NULL) {
          num[i++] = atoi(ptr);
          ptr = strtok(NULL, " ");
        }
        x = num[0];
        y = num[1];
        table = num[2];
        operand = num[3];
        count = 0;
        str[0] = '\0';
      }
      else {
        str[count++] = wParam;  //--- 문자저장 후 인덱스 증가
        str[count] = '\0';      //--- 문자열은 null(‘\0’)로 끝남
      }  
      InvalidateRect(hWnd, NULL, TRUE);
      break;
    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      GetTextExtentPoint32(hDC, str, lstrlen(str), &size);  //--- 문자열 길이 알아내기
      TextOut(hDC, 0, 0, str, lstrlen(str));
      SetCaretPos(size.cx, 0);  //--- 캐럿 위치하기
      if (isEnter == true) {
        for (int i = 0; i < operand; ++i) {
          wsprintf(lpOut, L"%d x %d = %d", table, i + 1, table * (i + 1));
          y += 15;
          TextOut(hDC, x, y, lpOut, lstrlen(lpOut));
        }
        isEnter == false;
      }
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