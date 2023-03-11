// QQMsgList.cpp : 定义应用程序的入口点。

#include "framework.h"
#include "QQMsgList.h"
#include "resource.h"

#define MAX_LOADSTRING 100
#define ID_LIST        1001
#define ITEM_LEN       5

typedef struct INFO_STRUCT {
    UINT bitmapId;
    char userName[MAX_LOADSTRING];
    char newData[MAX_LOADSTRING];
}INFO_STRUCT;


UINT bitmapId_arr[ITEM_LEN] = {
    IDB_BITMAP1,IDB_BITMAP2,IDB_BITMAP3,IDB_BITMAP4,IDB_BITMAP5
};

char userName_arr[ITEM_LEN][MAX_LOADSTRING] = {
    "孤月沧照客心愁",
    "薄雾浓云愁永昼",
    "暴风雨中的白杨",
    "爱恨情长愁似水",
    "久不离の久不弃"
};

char newData_arr[ITEM_LEN][MAX_LOADSTRING] = {
    "北冥有鱼，其名为鲲",
    "鲲之大，不知其几千里也",
    "化而为鸟，其名为鹏",
    "鹏之背，不知其几千里也",
    "怒而飞，其翼若垂天之云。"
};

INFO_STRUCT info_arr[ITEM_LEN];

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND hListBox;                                  // ListBox 句柄
char buf[10];                                   // 小buf

const TCHAR* strArray[] = { TEXT("星期一"), TEXT("星期二"), TEXT("星期三"),
                          TEXT("星期四"), TEXT("星期五"), TEXT("星期六"), TEXT("星期日") };

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Init_InfoArr() {
    for (int i = 0; i < ITEM_LEN; i++) {
        info_arr[i].bitmapId = bitmapId_arr[i];
        strcpy_s(info_arr[i].userName, MAX_LOADSTRING, userName_arr[i]);
        strcpy_s(info_arr[i].newData, MAX_LOADSTRING, newData_arr[i]);
    }
}

void SetListBoxItemRect(LPARAM lParam) {
    PMEASUREITEMSTRUCT pmr = (PMEASUREITEMSTRUCT)lParam;
    if (pmr->CtlID == ID_LIST) {
        pmr->itemHeight = 58;
    }
}

void DrawListBoxItem(LPARAM lParam) {
    TCHAR* pVarName;
    PDRAWITEMSTRUCT pdr = (PDRAWITEMSTRUCT)(lParam);
    COLORREF nColor = RGB(66, 66, 66);
    HBRUSH hBursh = NULL;   
    if(pdr->itemState & ODS_SELECTED ) // 选中状态
        hBursh = CreateSolidBrush(RGB(255, 128, 0));
    else
        hBursh = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(pdr->hDC, &pdr->rcItem, hBursh);
    DeleteObject(hBursh);
    int itemLength = SendMessage(hListBox, LB_GETTEXTLEN, pdr->itemID, 0) + 1;
    pVarName = (TCHAR*)malloc(itemLength * sizeof(TCHAR));
    SendMessage(hListBox, LB_GETTEXT, pdr->itemID, (LPARAM)pVarName);

    if (pVarName == NULL) return;
    HBITMAP hBmp = LoadBitmap(hInst, (char*)info_arr[atoi(pVarName)].bitmapId);
    HDC Bhdc = CreateCompatibleDC(pdr->hDC);
    HGDIOBJ oldBMP = SelectObject(Bhdc, hBmp);
    StretchBlt(pdr->hDC, pdr->rcItem.left + 1, pdr->rcItem.top + 1, 56, 56, Bhdc, 0, 0, 100, 100, SRCCOPY);
    HFONT font1 = CreateFont(
        20,
        0,
        0,
        0,
        900,
        NULL,
        NULL,
        NULL,
        GB2312_CHARSET,
        NULL,
        NULL,
        NULL,
        NULL,
        "仿宋"
    );
    HFONT font2 = CreateFont(
        20,
        0,
        0,
        0,
        800,
        NULL,
        NULL,
        NULL,
        GB2312_CHARSET,
        NULL,
        NULL,
        NULL,
        NULL,
        "方正舒体"
    );
    SetBkMode(pdr->hDC, TRANSPARENT);
    HGDIOBJ oldfont = SelectObject(pdr->hDC, font1);
    nColor = RGB(0, 0, 0);
    SetTextColor(pdr->hDC, nColor);
    TextOut(pdr->hDC, pdr->rcItem.left + 65, pdr->rcItem.top + 5, info_arr[atoi(pVarName)].userName, strlen(info_arr[atoi(pVarName)].userName));
    SelectObject(pdr->hDC, font2);
    nColor = RGB(133, 193, 246);
    SetTextColor(pdr->hDC, nColor);
    TextOut(pdr->hDC, pdr->rcItem.left + 65, pdr->rcItem.top + 30, info_arr[atoi(pVarName)].newData, strlen(info_arr[atoi(pVarName)].newData));
    SelectObject(pdr->hDC, oldfont);
    SelectObject(Bhdc, oldBMP);
    DeleteObject(hBmp);
    DeleteObject(font1);
    DeleteObject(font2);
    DeleteDC(Bhdc);
    free(pVarName);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    // 初始化全局字符串
    LoadString(hInstance, IDS_APP_TITLE, (LPSTR)szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_QQMSGLIST, (LPSTR)szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QQMSGLIST));

    MSG msg;

    // 主消息循环:
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



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QQMSGLIST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QQMSGLIST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0,335, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        Init_InfoArr();
        hListBox = CreateWindowEx(0, TEXT("ListBox"),  // LBS_STANDARD
            NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_HASSTRINGS | LBS_OWNERDRAWVARIABLE|WS_VSCROLL, 10, 10, 300, 400, hWnd, (HMENU)ID_LIST, ((LPCREATESTRUCT)lParam)->hInstance, 0);
        
        for (int i = 0; i < ITEM_LEN; i++) {
            _itoa_s(i, buf, 10);
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buf);
        }
        break;
    case WM_DRAWITEM:
        DrawListBoxItem(lParam);
        return 0;
    case WM_MEASUREITEM:
        SetListBoxItemRect(lParam);
        return 0;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            return 0;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
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

// “关于”框的消息处理程序。
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
