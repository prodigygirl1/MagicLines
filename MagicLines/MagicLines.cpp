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



//// Сделать:
//// 1) Волновой алгоритм поиска пути +++
//// 2) Количество очков над полем
//// 3) Изменить цвета на более красивые
//// 5) прямоугольное поле ????
//// 6) Работа с файлом - сохранение игрового поля !!!!!!!!!!!!!!!
//// Если игрок выходит из игры до ее завершения, нужно предложить сохранить ее
//// Если игрок заходит в игру, но при этом файл сохранения не пустой, ему будет предложено возобновить игру
//
//// Поле
//int board[MAX_SIZE][MAX_SIZE];
//// Массив цветов
//COLORREF colors[COLORS_NUM] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(127, 47, 0), RGB(34, 78, 99) }; // !!! Добавить красивые цвета
//// Выбранная ячейка
//point_t selected_cell = {0, 0};
//// Сохраненная ячейка
//point_t saved_cell = {0, 0};
//// Флаг выбора ячейки
//BOOL is_selected = false;
//
//// Функция random_num(int n1, int n2)
//// Возвращает случайное число в заданнном диапазоне (от n1 до n2)
//int random_num(int n1, int n2) {
//    return rand() % n2 + n1;
//}
//
//// void shuffle_values(point_t *coords) -- на вход дается указатель на первый элемент массива
//// Перемешивает ячейки массива
//void shuffle_values(point_t *coords, int size) {
//    int i = 0;
//    int ind1, ind2;
//    while (i < size) {
//        ind1 = random_num(0, size);
//        ind2 = random_num(0, size);
//        struct point_t prev_val = coords[ind1];
//        coords[ind1] = coords[ind2];
//        coords[ind2] = prev_val;
//        i++;
//    }
//    
//}
//
//// Функция void add_balls(int number_balls)
//// Добавление новых шариков на поле в рандомные места
//int add_balls(int number_balls, struct point_t *new_balls) {
//    struct point_t coords[MAX_SIZE * MAX_SIZE];
//    int start_n = 0;
//    // поиск свободных ячеек
//    for (int i = 0; i < MAX_SIZE; i++) {
//        for (int j = 0; j < MAX_SIZE; j++) {
//            if (board[i][j] == 0) {
//                coords[start_n].x = i;
//                coords[start_n].y = j;
//                start_n++;
//            }
//        }
//    }
//    shuffle_values(&coords[0], start_n);
//    // заполнение шариками
//    for (int i = 0; i < min(number_balls, start_n); i++) {
//        int color = random_num(0, COLORS_NUM);
//        board[coords[i].x][coords[i].y] = color+1;
//        if (new_balls) {
//            new_balls[i] = coords[i];
//        }
//    }
//    return min(number_balls, start_n);
//}
//
//// Функция init_board(int number_balls)
//// Заполняет поле начальными значениями
//void init_board(int number_balls) {
//    int i = 0;
//    int j;
//    //struct point_t coords[MAX_SIZE * MAX_SIZE];
//
//    int start_n = 0;
//    // заполнение массива board нулями
//    while (i < MAX_SIZE) {
//        j = 0;
//        while (j < MAX_SIZE) {
//            board[i][j] = 0;
//            j++;
//        }
//        i++;
//    }
//   
//   add_balls(number_balls, NULL);
//}
//
//// Функция board_paint(HDC hdc, int square_size)
//// Рисует поле и шарики на нем
//void board_paint(HDC hdc, int square_size)
//{
//    HBRUSH hBrush;
//    HPEN hPen;
//    int x0 = 100, y0 = 100;
//    int x = 100, y = 100;
//    // отрисовка сетки из квадратов
//    while (x < MAX_SIZE * square_size + x0) {
//        y = 100;
//        while (y < MAX_SIZE * square_size + y0) {
//            RECT square = {x, y, x + square_size, y + square_size};
//            Rectangle(hdc, square.left, square.top, square.right, square.bottom);
//            y = y + square_size;
//        }
//        x = x + square_size;
//    }
//
//    // отрисовка шариков
//    int i = 0, j;
//    int delta_zise = 5;
//    while (i < MAX_SIZE) {
//        j = 0;
//        while (j < MAX_SIZE) {
//            if (board[i][j] > 0) {
//                RECT square = { i * square_size + x0 + delta_zise, j * square_size + y0 + delta_zise, i * square_size + square_size + x0 - delta_zise, j * square_size + square_size + y0 - delta_zise};
//                hBrush = CreateSolidBrush(colors[board[i][j] - 1]);
//                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//                Ellipse(hdc, square.left, square.top, square.right, square.bottom);
//                SelectObject(hdc, hOldBrush);
//                DeleteObject(hBrush);
//            }
//            j++;
//        }
//        i++;
//    }
//    SelectObject(hdc, GetStockObject(NULL_BRUSH));
//    // отрисовка ячейки, на которой курсор
//    hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); 
//    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
//    RECT square = { selected_cell.x * square_size + x0, selected_cell.y * square_size + y0, selected_cell.x * square_size + square_size + x0, selected_cell.y * square_size + square_size + y0};
//    Rectangle(hdc, square.left, square.top, square.right, square.bottom);
//    SelectObject(hdc, hOldPen);
//    DeleteObject(hPen);
//    // отрисовка выбранной для перемещения ячейки, если она есть
//    if (is_selected) {
//        hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
//        hOldPen = (HPEN)SelectObject(hdc, hPen);
//        square = { saved_cell.x * square_size + x0, saved_cell.y * square_size + y0, saved_cell.x * square_size + square_size + x0, saved_cell.y * square_size + square_size + y0 };
//        Rectangle(hdc, square.left, square.top, square.right, square.bottom);
//        SelectObject(hdc, hOldPen);
//        DeleteObject(hPen);
//    }
//    
//}
//
//bool point_in_range(int i, int j) {
//    if ((i >= 0) && (i < MAX_SIZE) && (j >= 0) && (j < MAX_SIZE)) return true;
//    return false;
//}
//
//// Функция int wave_alorithm(int x1, int y1, int x2, int y2)
//// x1, x2 - текущая точка
//// x2,y2 - конечная точка
//// Функция волнового алгоритма 
//bool wave_algorithm(int x1, int y1, int x2, int y2, int d, int dublicat_board[MAX_SIZE][MAX_SIZE]) {
//    int i = 0, j = 0;
//    int value = 1;
//    // алгоритм распространения волны
//    do {
//        for (i = 0; i < MAX_SIZE; i++) {
//            for (j = 0; j < MAX_SIZE; j++) {
//                if ((dublicat_board[i][j] == value)) {
//                    if (point_in_range(i - 1, j)) 
//                        if ((dublicat_board[i - 1][j] != IS_OCCUPIED) && (dublicat_board[i - 1][j] == 0)) dublicat_board[i - 1][j] = value + 1;
//                    if (point_in_range(i + 1, j)) 
//                        if ((dublicat_board[i + 1][j] != IS_OCCUPIED) && (dublicat_board[i + 1][j] == 0)) dublicat_board[i + 1][j] = value + 1;
//                    if (point_in_range(i, j - 1)) 
//                        if ((dublicat_board[i][j - 1] != IS_OCCUPIED) && (dublicat_board[i][j - 1] == 0)) dublicat_board[i][j - 1] = value + 1;
//                    if (point_in_range(i , j + 1))
//                        if ((dublicat_board[i][j + 1] != IS_OCCUPIED) && (dublicat_board[i][j + 1] == 0)) dublicat_board[i][j + 1] = value + 1;
//                }
//            }
//        }
//        value = value + 1;
//    } while ((dublicat_board[x2][y2] == 0) && (value < MAX_SIZE * MAX_SIZE));
//    // если заполнена конечная ячейка
//    if (dublicat_board[x2][y2] != 0) return true;
//    return false;
//    
//}
//
//// Функция bool check_path(x1, y1, x2, y2) !!!! доделать
//// Проверяет, есть ли путь из точки (x1, y1) до точки (x2, y2). Использует "волновой" алгоритм
//bool check_path(int x1, int y1, int x2, int y2) {
//    int dublikat[MAX_SIZE][MAX_SIZE];
//    int i = 0, j = 0;
//    // создание поля с пустыми ячейками(0) и препятствиями(65535)
//    while (i < MAX_SIZE) {
//        j = 0;
//        while (j < MAX_SIZE) {
//            dublikat[i][j] = 0;
//            if (board[i][j] > 0) 
//                dublikat[i][j] = IS_OCCUPIED;
//            j++;
//        }
//        i++;
//    }
//    dublikat[x1][y1] = 1; // точка с шариком ???
//    // заполнение поля-дубликата шагами в пути с помощью функции wave_algorithm(). На вход дается указатель на первый элемент двумерного массива dublicat
//    bool res = wave_algorithm(x1, y1, x2, y2, 0, dublikat);
//    return res;
//}
//
//
//// Функция void move_ball(x1, y1, x2, y2)
//// Перемещает шарик с позиции x1, y1 на позицию x2, y2
//void move_ball(int x1, int y1, int x2, int y2) {
//    board[x2][y2] = board[x1][y1];
//    board[x1][y1] = 0;
//}
//
//// Функция void delete_balls(int x, int y)
//// Удаляет шарики по вертикали и горизонтали
//bool delete_balls(int x, int y) {
//    if (board[x][y] == 0) {
//        return false;
//    }
//    struct point_t coords_to_delete[MAX_SIZE * MAX_SIZE];
//    struct point_t temp[MAX_SIZE * MAX_SIZE];
//    int start_n = 0;
//    int number = 0;
//    int i;
//    // поиск совпадющих эелементов по горизонтали
//    // вправо
//    for (i = x + 1; (i < MAX_SIZE) && (board[i][y] == board[x][y]); i++) {
//        struct point_t p = { i, y };
//        temp[start_n] = p;
//        start_n++;            
//    }
//    // влево
//    for (i = x; (i > -1); i--) {
//        struct point_t p = { i, y };
//        if (board[i][y] != board[x][y]) break;
//        temp[start_n] = p;
//        start_n++;
//    }
//
//    if (start_n > 3) {
//        for (i = 0; i < start_n; i++) {
//            coords_to_delete[i] = temp[i];       
//        }
//        number = start_n; // увеличение итогового количества точек на удаление
//    }
//    // вниз
//    start_n = 0;
//    for (i = y + 1; (i < MAX_SIZE) && (board[x][i] == board[x][y]); i++) {
//        struct point_t p = { x, i };
//        temp[start_n] = p;
//        start_n++; 
//    }
//    // вверх
//    for (i = y; (i > -1); i--) {
//        if (board[x][i] != board[x][y]) break;
//        temp[start_n].x = x;
//        temp[start_n].y = i;
//        start_n++;
//    }
//   
//    if (start_n > 3) {
//        for (i = 0; i < start_n; i++) {
//            coords_to_delete[i + number] = temp[i];
//        }
//        number += start_n; // увеличение итогового количества точек на удаление 
//    }
//
//    // удаление найденных линий шариков (обнуление значений на поле)
//    bool flag = false;
//    for (i = 0; i < number; i++) {
//        board[coords_to_delete[i].x][coords_to_delete[i].y] = 0;
//        bool flag = true;
//    }
//    return flag;
//}
//
//// Функция test_delete_balls()
//// Заполняет поле тестовыми значениями
//void test_delete_balls() {
//    for (int i = 0; i < MAX_SIZE; i++) {
//        for (int j = 0; j < 3; j++) {
//            if (i == 2) {
//                board[i][j] = 2;
//            }
//        }
//    }
//}
//// Функция bool board_is full()
//// Проверяет наличие свободных клеток на поле
//bool board_is_full() {
//    for (int i = 0; i < MAX_SIZE; i++) {
//        for (int j = 0; j < MAX_SIZE; j++) {
//            if (board[i][j] == 0) {
//                return false;
//            }
//        }
//    }
//    return true;
//}

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
        SaveRecords();
        PostQuitMessage(0);
        break;
    case WM_DESTROY:
        SaveRecords();
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
