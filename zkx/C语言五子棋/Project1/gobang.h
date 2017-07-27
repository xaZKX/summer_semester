#include <windows.h>

//用来记录当前棋子位置
typedef struct _Present
{
	int x;
	int y;
}Present;

//用来记录前一个棋子位置
typedef struct _Previous
{
	int x;
	int y;
}Previous;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

void BackGroundPaint(HWND hwnd);

LRESULT CALLBACK GameWindowProc(HWND, UINT, WPARAM, LPARAM);

HWND GameWindowCreate(HINSTANCE hinstance);

BOOL GameWindwowRegister(HINSTANCE hinstance);

void GamePaint(HWND hwnd);

void Judge(int u, int v, HWND hwnd);

int Win(int x, int y, HWND hwnd);
