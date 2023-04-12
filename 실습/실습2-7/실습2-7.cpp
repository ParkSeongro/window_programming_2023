#include <tchar.h>
#include <cctype>
#include <windows.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 2-7 ";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);

void InsertStrElem(TCHAR* str, const TCHAR& c, const int& index);
void DeleteStrElem(TCHAR* str, const int& index);

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
  const int row_num = 5; 
  const int col_num = 5 + 1;
  const int max_row = row_num - 1;
  const int max_col = col_num - 1;
  PAINTSTRUCT ps;
  HDC hDC;
  SIZE size;
  static TCHAR str[row_num][col_num];
  static int row;
  static int col;
  static bool is_upper;

  switch (uMsg) {
    case WM_CREATE:
      row = col = 0;
      is_upper = false;
      CreateCaret(hWnd, NULL, 5, 15);
      ShowCaret(hWnd);
      break;

    case WM_CHAR:
      switch (wParam) {
        case VK_BACK:
          if (col > 0)
            DeleteStrElem(str[row], --col);
          else if (col == 0 && row > 0) {
            --row;
            col = lstrlen(str[row]);
          }
          break;
        case VK_RETURN:
          if (row > max_row)
            row = 0;
          else
            ++row;
          col = 0;
          break;

        case VK_ESCAPE:
          row = col = 0;
          for (int j = 0; j < max_col; ++j)
            for (int i = 0; i < max_row; ++i) str[j][i] = '\0';
          break;
        case VK_TAB:
          for (int i = 0; i < 4; ++i) {
            if (lstrlen(str[row]) < max_col)
              InsertStrElem(str[row], L' ', col++);
          }
          break;
        default:
          if (col == max_col) {
            ++row;
            col = 0;
          }
          if (row == max_row) row = 0;
          str[row][col++] = wParam;
            InsertStrElem(str[row], wParam, col++);
          break;
      }
      InvalidateRect(hWnd, NULL, TRUE);
      break;
    case WM_KEYDOWN:
      switch (wParam) {
        case VK_LEFT:
          if (col > 0) --col;
          break;
        case VK_UP:
          if (row > 0) --row;
          break;
        case VK_RIGHT:
          if (col < lstrlen(str[row])) ++col;
          break;
        case VK_DOWN:
          if (row < max_row) ++row;
          break;
        /*case VK_F1:
          if (!is_upper) {
            is_upper == true;
            str[row][col] = toupper(str[row][col]);
          } else {
            is_upper == false;
            str[row][col] = tolower(str[row][col]);
            break;*/
        case VK_HOME:
          col = 0;
          break;
        case VK_END:
          if (lstrlen(str[row]) == max_col) {
            ++row;
            col = 0;
          }
          col = lstrlen(str[row]);
          break;
        case VK_INSERT:
          break;
            default:
              break;
          }
        case WM_PAINT:
          hDC = BeginPaint(hWnd, &ps);
          GetTextExtentPoint32(hDC, str[row], col, &size);
          SetCaretPos(size.cx, row * 16);
          for (int i = 0; i < row_num; ++i)
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

 void InsertStrElem(TCHAR* str, const TCHAR& c, const int& index) {
  for (int i = lstrlen(str); i > index; --i) str[i] = str[i - 1];
  str[index] = c;
 }

 void DeleteStrElem(TCHAR* str, const int& index) {
  for (int i = index; i < lstrlen(str); ++i) str[i] = str[i + 1];
 }