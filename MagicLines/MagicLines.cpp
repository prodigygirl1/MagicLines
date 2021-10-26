// MagicLines.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "MagicLines.h"
#include <ctime>

#define CRT_NO_SECURE_WARNINGS
#define MAX_LOADSTRING 100


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//---------------------------------------------------------------------------------
#define MAX_SIZE 7
#define BALLS_FIRST 3
#define SIZE_SQUARE 50
#define COLORS_NUM 5
struct point_t {
    int x; int y;
};


// Поле
int board[MAX_SIZE][MAX_SIZE];
// Массив цветов
COLORREF colors[COLORS_NUM] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(127, 47, 0), RGB(34, 78, 99) }; // !!! Добавить красивые цвета
// Выбранная ячейка
point_t selected_cell = {0, 0};
// Флаг выбора ячейки
BOOL is_chosen = false;

// Функция random_num(int n1, int n2)
// Возвращает случайное число в заданнном диапазоне (от n1 до n2)
int random_num(int n1, int n2) {
    return rand() % n2 + n1;
}

// Функция init_board(int number_balls)
// Заполняет поле начальными значениями
void init_board(int number_balls) {
    int i = 0;
    int j;
    struct point_t coords[MAX_SIZE * MAX_SIZE];

    int start_n = 0;
    // заполнение массива board нулями
    while (i < MAX_SIZE) {
        j = 0;
        while (j < MAX_SIZE) {
            board[i][j] = 0;
            j++;
        }
        i++;
    }
    // заполнение массива coords координатами всех ячеек
    while (start_n < MAX_SIZE * MAX_SIZE) {
        coords[start_n].x = start_n / MAX_SIZE;
        coords[start_n].y = start_n % MAX_SIZE;
        start_n++;
    }
    // перемешивание значений массива coords
    i = 0;
    struct point_t point1, point2;
    while (i < MAX_SIZE * MAX_SIZE) {
        point1 = { random_num(0, MAX_SIZE), random_num(0, MAX_SIZE) };
        point2 = { random_num(0, MAX_SIZE), random_num(0, MAX_SIZE) };
        coords[point1.x * MAX_SIZE + point1.y] = point2;
        coords[point2.x * MAX_SIZE + point2.y] = point1;
        i++;
    }
   // размещение первых шаров
    i = 0;
    while (i < BALLS_FIRST) {
        int color = random_num(0, COLORS_NUM);
        board[coords[i].x][coords[i].y] = color + 1;
        i++;
    } 
}
// Функция board_paint(HDC hdc, int square_size)
// Рисует поле и шарики на нем
void board_paint(HDC hdc, int square_size)
{
    HBRUSH hBrush;
    HPEN hPen;
    int x0 = 100, y0 = 100;
    int x = 100, y = 100;
    // отрисовка сетки из квадратов
    while (x < MAX_SIZE * square_size + x0) {
        y = 100;
        while (y < MAX_SIZE * square_size + y0) {
            RECT square = {x, y, x + square_size, y + square_size};
            Rectangle(hdc, square.left, square.top, square.right, square.bottom);
            y = y + square_size;
        }
        x = x + square_size;
    }
    // отрисовка шариков
    int i = 0, j;
    int delta_zise = 5;
    while (i < MAX_SIZE) {
        j = 0;
        while (j < MAX_SIZE) {
            if (board[i][j] > 0) {
                RECT square = { i * square_size + x0 + delta_zise, j * square_size + y0 + delta_zise, i * square_size + square_size + x0 - delta_zise, j * square_size + square_size + y0 - delta_zise};
                hBrush = CreateSolidBrush(colors[board[i][j] - 1]);
                SelectObject(hdc, hBrush);
                Ellipse(hdc, square.left, square.top, square.right, square.bottom);
                
            }
            j++;
        }
        i++;
    }
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, hPen);
    RECT square = { selected_cell.x * square_size + x0, selected_cell.y * square_size + y0, selected_cell.x * square_size + square_size + x0, selected_cell.y * square_size + square_size + y0};
    Rectangle(hdc, square.left, square.top, square.right, square.bottom);
}
// 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    // Инициализация генератора случайных чисел
    srand(time(0));
    // Создание поля
    init_board(BALLS_FIRST);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAGICLINES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAGICLINES));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAGICLINES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAGICLINES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            board_paint(hdc, SIZE_SQUARE);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_DOWN:
            if (selected_cell.y < MAX_SIZE) {
                selected_cell.y++;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            if (selected_cell.y > 0) {
                selected_cell.y--;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            if (selected_cell.x > 0) {
                selected_cell.x--;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            if (selected_cell.x < MAX_SIZE) {
                selected_cell.x++;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
