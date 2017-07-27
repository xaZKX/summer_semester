#include <windows.h>
#include "gobang.h"

#define CLASS_NAME1 "background_class" //窗口类名
#define CLASS_NAME2  "gobang_class"      //窗口类名
#define TITLE_BAR   "Five-In-a-Row"         //窗口标题名

// 画图时使用的像素点个数。
#define CELL_PIXEL			30
//棋盘大小15*15
#define MAX  15    

//判断是玩家1还是玩家2
BOOL Player = TRUE;

//画图时所用颜色
#define COLOR_CHESS1 RGB(0,0,0)               //玩家1黑棋
#define COLOR_CHESS2 RGB(255,255,255)  //玩家2白棋
#define COLOR_CHESSBOARD RGB(235,218,100)   //棋盘颜色
#define COLOR_DOT RGB(98, 0, 98)     //标记点颜色

// 全局变量
HINSTANCE hinst; /// HINSTANCE是用来表示程序运行实例的句柄，某些API函数会使用到这个变量。
int game[MAX][MAX];   //用来储存棋子位置的二维数组
POINT point;  //鼠标点击位置
HDC hdc;
HBITMAP hbmp;
int sum = 0;   //棋子总数
Present p1;
Previous p2;

//##########   入口函数WinMain    ##########
int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{
	//注册窗口类的 API函数RegisterClass的参数，是一个结构体类型，用来存储一个窗口类的各种属性
	WNDCLASS wc;

	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;
	MSG msg;
	hinst = hinstance;
	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// 窗口类的样式
	wc.style = CS_DBLCLKS;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式

	wc.hCursor = LoadCursor(NULL,
		IDC_ARROW);
	//窗口背景颜色
	wc.hbrBackground = NULL;
	// 窗口的菜单的资源名。
	wc.lpszMenuName = NULL;
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = CLASS_NAME1;

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "注册窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。


	// Create the main window. 

	hwnd = CreateWindow(
		CLASS_NAME1,			// 窗口类名，必须是已经注册了的窗口类
		TITLE_BAR,		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
		300,			// 窗口水平位置default horizontal POINT 
		150,			// 窗口垂直位置default vertical POINT 
		700,		    // 窗口宽度 default width 
		580,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);
	// 以下进入消息循环。获取消息--翻译消息--分配消息（由窗口的消息处理函数来处理消息）
	while (GetMessage(&msg, (HWND)NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void BackGroundPaint(HWND hwnd)
{

	HDC hdcMem;
	RECT rect;
	// Create a memory device compatible with the above DC variable
	hdcMem = CreateCompatibleDC(hdc);
	// Select the new bitmap
	SelectObject(hdcMem, hbmp);

	// Copy the bits from the memory DC into the current dc
	GetClientRect(hwnd, &rect);
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);

	// Restore the old bitmap
	DeleteDC(hdcMem);

}

void GamePaint(HWND hwnd)
{

	//画棋盘格
	for (int i = 1; i <= MAX; i++)
	{
		MoveToEx(hdc, CELL_PIXEL, i * CELL_PIXEL, NULL);
		LineTo(hdc, MAX  * CELL_PIXEL, i * CELL_PIXEL);
	}
	for (int j = 1; j <= MAX; j++)
	{
		MoveToEx(hdc, j * CELL_PIXEL, CELL_PIXEL, NULL);
		LineTo(hdc, j * CELL_PIXEL, MAX  * CELL_PIXEL);
	}

	//画标记点
	for (int i = 1; i <= 3; i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			HBRUSH hbr0 = CreateSolidBrush(RGB(98, 0, 98));
			int x = 4 * i*CELL_PIXEL;
			int y = 4 * j*CELL_PIXEL;
			int r = 4;
			SelectObject(hdc, hbr0);
			Ellipse(hdc, x - r, y - r, x + r, y + r);
			DeleteObject(hbr0);
		}
	}

	//画棋子
	for (int i = 0; i<15; i++)
	{
		for (int j = 0; j<15; j++)
		{
			if (game[i][j] == 0) continue;
			int x = (i + 1)*CELL_PIXEL;
			int y = (j + 1)*CELL_PIXEL;
			int r = CELL_PIXEL / 2.1;
			HBRUSH hbr = CreateSolidBrush((game[i][j] == 1) ? COLOR_CHESS1 : COLOR_CHESS2);
			SelectObject(hdc, hbr);
			Ellipse(hdc, x - r, y - r, x + r, y + r);
		}
	}

	//记录棋子数 同时判断轮到谁落子:
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(235, 218, 100));
	char* who = (Player == 1 ? "Player1" : "Player2");
	char ch[50] = { 0 };
	wsprintf(ch, "现在是   %s   ！   总棋子数:%2d", who, sum);
	TextOut(hdc, 80, 465, ch, strlen(ch));

	//设置右边提示区域
	TextOut(hdc, 545, 60, "五 子 棋", strlen("五 子 棋")); //从坐标(545,60)开始绘制
	Rectangle(hdc, 485, 90, 700, 91);    //从坐标(485,90)到坐标(700,90)绘制宽度为1矩形条
	TextOut(hdc, 535, 150, "Player1: 黑棋", strlen("Player1: 黑棋"));//从坐标(535,150)开始绘制
	TextOut(hdc, 535, 210, "Player2: 白棋", strlen("Player2: 白棋"));//从坐标(535,210)开始绘制
	Rectangle(hdc, 485, 270, 700, 271);   //从坐标(485,270)到坐标(700,270)绘制宽度为1矩形条
	TextOut(hdc, 525, 310, "F1：重新开始游戏", strlen("F1：重新开始游戏")); //从坐标(525,310)开始绘制
	TextOut(hdc, 525, 350, "F2：悔棋", strlen("F2：悔棋")); //从坐标(525,350)开始绘制
	TextOut(hdc, 525, 390, "Esc：退出游戏", strlen("Esc：退出游戏"));         //从坐标(525,390)开始绘制
	Rectangle(hdc, 485, 430, 700, 431); //从坐标(485,430)到坐标(700,430)绘制宽度为1矩形条

										// DeleteObject 释放GDI对象
	DeleteObject(hdc);

	//#############  ReleaseDC 函数  ################
	ReleaseDC(hwnd, hdc);
}

//横向，竖向，斜向上五颗连续的相同颜色的棋子，确认哪个玩家获胜
void Judge(int u, int v, HWND hwnd)
{
	int x;
	int y;
	int iCount;
	x = u;
	y = v;

	//跟左边棋子对比
	iCount = 1;
	u = x - 1;
	v = y;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		u--;
	}

	//跟右边棋子对比
	u = x + 1;
	v = y;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		u++;
	}

	//跟上面棋子对比
	iCount = 1;
	u = x;
	v = y - 1;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		v--;
	}

	//跟下面棋子对比
	u = x;
	v = y + 1;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		v++;
	}

	//与右下方棋子比较
	iCount = 1;
	u = x - 1;
	v = y - 1;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		u--;
		v--;
	}

	//与左上方棋子比较
	u = x + 1;
	v = y + 1;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		u++;
		v++;
	}
	//与右上方棋子比较
	iCount = 1;
	u = x + 1;
	v = y - 1;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		u++;
		v--;
	}
	//与左下方棋子比较
	u = x - 1;
	v = y + 1;
	while (game[x][y] == game[u][v])
	{
		iCount++;
		if (iCount == 5)
		{
			Win(x, y, hwnd);
		}
		u--;
		v++;
	}
}

//输出玩家获胜消息
int Win(int x, int y, HWND hwnd)
{
	char* ch;
	if (game[x][y] == 1)
	{
		ch = "Player1获胜! 按Esc退出，按F1重新开始游戏";
	}
	else
	{
		ch = "Player2获胜! 按Esc退出，按F1重新开始游戏";
	}
	int iYes = MessageBox(hwnd, ch, "提示", MB_OK);
	return iYes;
}

LRESULT CALLBACK GameWindowProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{

	PAINTSTRUCT ps;
	RECT rect;
	int u, v;      //棋子坐标
				   // 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{

	case WM_KEYDOWN:
		switch (wParam)
		{
			int iOK;

		case VK_ESCAPE:
			iOK = MessageBox(hwnd, "是否退出游戏", "提示", MB_YESNO);
			if (iOK == IDYES)
			{
				DestroyWindow(hwnd);
			}
			break;

		case VK_F1:
			iOK = MessageBox(hwnd, "重新开始游戏", "提示", MB_OK);
			if (iOK == IDOK)
			{
				for (int m = 0; m < 15; m++)
				{
					for (int n = 0; n < 15; n++)
					{
						game[m][n] = 0;
					}
				}
				InvalidateRect(hwnd, NULL, TRUE);
				sum = 0;
				return 0;
			}
			break;

		case VK_F2:
			iOK = MessageBox(hwnd, "是否悔棋", "提示", MB_YESNO);
			if (iOK == IDYES)
			{
				game[p1.x][p1.y] = 0;
				game[p2.x][p2.y] = 0;
			}
			sum -= 2;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}


	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GamePaint(hwnd);

		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:

		sum++;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		GetClientRect(hwnd, &rect);
		GamePaint(hwnd);
		u = (point.x - CELL_PIXEL) / (float)CELL_PIXEL + 0.5;
		v = (point.y - CELL_PIXEL) / (float)CELL_PIXEL + 0.5;

		if (game[u][v] != 0) break;
		if (u<0 || v<0 || u >14 || v >14) break;

		game[u][v] = (Player ? 1 : 2);
		if (game[u][v] == 1)
		{
			p1.x = u;
			p1.y = v;
		}
		if (game[u][v] == 2)
		{
			p2.x = u;
			p2.y = v;
		}

		Player = !Player;

		InvalidateRect(hwnd, &rect, TRUE);
		Judge(u, v, hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

BOOL GameWindwowRegister(HINSTANCE hinstance)
{
	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = GameWindowProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = NULL;
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_ARROW);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = CreateSolidBrush(COLOR_CHESSBOARD);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = NULL;
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = CLASS_NAME2;

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	// 窗口注册成功，继续运行。
	return TRUE;
}

HWND GameWindowCreate(HINSTANCE hinstance)
{

	// Create the main window. 
	HWND hwnd;
	hwnd = CreateWindow(
		CLASS_NAME2,			// 窗口类名，必须是已经注册了的窗口类
		"",		// title-bar string 
		WS_POPUP,	// 窗口的style，这个表示为top-level window 
		308,			// 窗口水平位置default horizontal POINT 
		180,			// 窗口垂直位置default vertical POINT 
		685,			// 窗口宽度 default width 
		545,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return NULL;
	}



	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, SW_SHOW);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);
	return hwnd;

}

LRESULT CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	PAINTSTRUCT Ps;


	switch (msg)
	{
	case WM_CREATE:
		hbmp = LoadImage(NULL, "3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hwnd = CreateWindow(TEXT("BUTTON"), TEXT("开始游戏"), WS_CHILD | WS_VISIBLE, 295, 370, 120, 50, hwnd, NULL, NULL, NULL);

		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case BN_CLICKED:
			if (GameWindwowRegister(hinst))
			{
				hwnd = GameWindowCreate(hinst);
			}
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &Ps);
		BackGroundPaint(hwnd);
		EndPaint(hwnd, &Ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}