// MagicLines.cpp : Определяет точку входа для приложения.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "MagicLines.h"
#include "TableRecords.h"
#include "Game.h"
#include <ctime>


#define MAX_LOADSTRING 100
#define WIDTH_WND 700
#define HEIGHT_WND 600

#define IDBUTTON_1 100
#define IDM_NEWGAME 102
#define IDM_SAVE 103
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
           
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND hEdt1;
HWND hBtnSave;


//---------------------------------------------------------------------------------
#define MAX_SIZE 7
#define BALLS_FIRST 3
#define SIZE_SQUARE 50
#define COLORS_NUM 5
#define IS_OCCUPIED 65500

// режим показа содержимого: 1 - игра, 0 - таблица рекордов и кнопки
int gamemode = 1;



struct point_t {
    int x; int y;
};

extern int board[MAX_SIZE][MAX_SIZE];
extern COLORREF colors[COLORS_NUM];
extern point_t selected_cell;
extern point_t saved_cell;
extern BOOL is_selected;


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
    //init_board(BALLS_FIRST);
    
    //test_delete_balls(); // тестовое заполнение поля!!!
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
    HBRUSH background_brush = CreateSolidBrush(RGB(180, 200, 107));

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAGICLINES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.hbrBackground = background_brush;
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
      CW_USEDEFAULT, CW_USEDEFAULT, WIDTH_WND, HEIGHT_WND,  nullptr, nullptr, hInstance, nullptr);
   
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void ShowTextEdit(HWND hWnd) {
    // Создаем и показываем поле редактирования - для ввода имени рекордсмена
    hEdt1 = CreateWindowW(_T("edit"), _T("Name"),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 500, 50, 150, 30,
        hWnd, 0, hInst, NULL);
    ShowWindow(hEdt1, SW_SHOWNORMAL);

    // Создаем и показываем кнопку
    hBtnSave = CreateWindowW(_T("button"), _T("Сохранить"),
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        500, 100, 150, 30, hWnd, (HMENU)IDBUTTON_1, hInst, NULL);
    ShowWindow(hBtnSave, SW_SHOWNORMAL);
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
    //HWND hEdt1;
    //HWND hBtn;
    switch (message)
    {
    case WM_COMMAND:
        {
            if ( LOWORD(wParam) == IDBUTTON_1)    // если нажали на кнопку
            {
                TCHAR StrT[20];
                char str[20];

                // Берем имя из элемента редактирования и помещаем в строку Windows
                GetWindowText(hEdt1, StrT, sizeof(StrT));

                // Конвертирует строку Windows в строку Си 
                // !!!! ВАЖНО - корректно работает ТОЛЬКО для латинских букв!
                wcstombs(str, StrT, 20);

                // Фокус возвращаем в игру
                // нажатия клавиш снова управляют игрой!
                SetFocus(hWnd);

                // добавляем рекорд в таблицу рекордов
                //addRecord(str); // новый рекорд просто вставляем снизу в таблицу
                InsertRecord(str); // новый рекорд вставляем в таблицу, сохраняя сортировку
                ShowWindow((HWND)lParam, SW_HIDE);
                ShowWindow(hEdt1, SW_HIDE);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else {
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
                case IDM_NEWGAME:
                    new_game();
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case IDM_SAVE:
                    saveLevel();
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
        }
        break;
    case WM_CREATE:
        {   
            // СОЗДАНИЕ МЕНЮ 
            HMENU hMenu = CreateMenu();
            AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDM_NEWGAME, L"Новая игра");
            AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDM_SAVE, L"Сохранить");
            SetMenu(hWnd, hMenu);

            LoadRecords();
            int id_btn = MessageBox(hWnd, L"Вы хотите загрузить сохранение?", L"Уведомление", MB_YESNO);
            if (id_btn == IDYES) {
                loadLevel();
            }
            else if (id_btn == IDNO) {
                new_game();
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (gamemode == 1) {
                board_paint(hdc, SIZE_SQUARE);
            }
            else if (gamemode == 0)
                DrawRecords(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN: // Закрывается при нажатии кнопки !!!
        switch (wParam)
        {
        case VK_ESCAPE:
            gamemode = !gamemode;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_DOWN:
            if (selected_cell.y < MAX_SIZE - 1) {
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
            if (selected_cell.x < MAX_SIZE - 1) {
                selected_cell.x++;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_SPACE:
            // если ячейка с шаром еще не выбрана
            if (is_selected == false) {
                // если по координатам выбранной ячейки есть шарик, то сохраняем ее координаты
                if (board[selected_cell.x][selected_cell.y] > 0) 
                {
                    saved_cell = { selected_cell.x, selected_cell.y };
                    is_selected = true;
                }
            }
            else {
                // если ячейка для перемещения шара свободна, то проверяем возможность перемещения на это место шарика
                if (board[selected_cell.x][selected_cell.y] == 0) {
                    if (check_path(saved_cell.x, saved_cell.y, selected_cell.x, selected_cell.y) == true) {                          
                        move_ball(saved_cell.x, saved_cell.y, selected_cell.x, selected_cell.y); // перемещение шарика
                        is_selected = false; // исходная ячейка не выбрана
                        saved_cell = { 0, 0 }; // новая ячейка не выбрана
                        if (not delete_balls(selected_cell.x, selected_cell.y)) {
                            struct point_t coords_new_balls[BALLS_FIRST];
                            int number_balls = add_balls(BALLS_FIRST, &coords_new_balls[0]);
                            for (int i = 0; i < number_balls; i++) {
                                delete_balls(coords_new_balls[i].x, coords_new_balls[i].y);
                            }
                            //InvalidateRect(hWnd, NULL, TRUE);
                            // если поле заполнено, то высвечиваем сообщение об окончании игры с предложением начать новую
                            if (board_is_full()) {
                                
                                ShowTextEdit(hWnd);
                                /*if (MessageBox(hWnd, L"Игра закончена. Хотите начать сначала?", L"Окончание игры", MB_OKCANCEL) == IDOK) {
                                    new_game();
                                }*/
                            }
                        }
                    }
                    else {
                        MessageBox(hWnd, L"Нет пути в данную ячейку", L"Оповещение", MB_OK);
                    }
                }
                else { // если несвободна, то происходит перевыбор ячейки перемещения
                    saved_cell = { selected_cell.x, selected_cell.y };                    
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
            
        }
        break;
    case WM_CLOSE:
        if (MessageBox(hWnd, L"Сохранить игру?", L"Выход из игры", MB_YESNO) == IDYES) saveLevel();
        PostQuitMessage(0);
        break;
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
