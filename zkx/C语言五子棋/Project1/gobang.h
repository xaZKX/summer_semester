#include <windows.h>

//������¼��ǰ����λ��
typedef struct _Present
{
	int x;
	int y;
}Present;

//������¼ǰһ������λ��
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
