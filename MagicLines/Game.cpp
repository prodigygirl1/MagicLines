#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include <stdio.h>
#include "Game.h"
#define MAX_SIZE 7
#define BALLS_FIRST 3
#define SIZE_SQUARE 50
#define COLORS_NUM 5
#define IS_OCCUPIED 65500
#define BK_COLOR RGB(180, 200, 107)

struct point_t {
    int x; int y;
};
// ����
int board[MAX_SIZE][MAX_SIZE];
//������� ����
int n, m;

// ������ ������
COLORREF colors[COLORS_NUM] = { RGB(178, 236, 93), RGB(253, 189, 186), RGB(216, 191, 216), RGB(154, 206, 235), RGB(255, 207, 171) }; 
// ��������� ������
point_t selected_cell = { 0, 0 };
// ����������� ������
point_t saved_cell = { 0, 0 };
// ���� ������ ������
BOOL is_selected = false;

// ��� ����� � ���������� ����
char filename[] = "level.txt";


// ���������� �����
int points = 0;

// ������� random_num(int n1, int n2)
// ���������� ��������� ����� � ��������� ��������� (�� n1 �� n2)
int random_num(int n1, int n2) {
    return rand() % n2 + n1;
}


// void shuffle_values(point_t *coords) -- �� ���� ������ ��������� �� ������ ������� �������
// ������������ ������ �������
void shuffle_values(point_t* coords, int size) {
    int i = 0;
    int ind1, ind2;
    while (i < size) {
        ind1 = random_num(0, size);
        ind2 = random_num(0, size);
        struct point_t prev_val = coords[ind1];
        coords[ind1] = coords[ind2];
        coords[ind2] = prev_val;
        i++;
    }

}

// ������� int add_balls(int number_balls)
// ���������� ����� ������� �� ���� � ��������� �����
int add_balls(int number_balls, struct point_t* new_balls) {
    struct point_t coords[MAX_SIZE * MAX_SIZE];
    int start_n = 0;
    // ����� ��������� �����
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            if (board[i][j] == 0) {
                coords[start_n].x = i;
                coords[start_n].y = j;
                start_n++;
            }
        }
    }
    shuffle_values(&coords[0], start_n);
    // ���������� ��������
    for (int i = 0; i < min(number_balls, start_n); i++) {
        int color = random_num(0, COLORS_NUM);
        board[coords[i].x][coords[i].y] = color + 1;
        if (new_balls) {
            new_balls[i] = coords[i];
        }
    }
    return min(number_balls, start_n);
}

// ������� init_board(int number_balls)
// ��������� ���� ���������� ����������
void init_board(int number_balls) {
    int i = 0;
    int j;
    //struct point_t coords[MAX_SIZE * MAX_SIZE];

    int start_n = 0;
    // ���������� ������� board ������
    while (i < MAX_SIZE) {
        j = 0;
        while (j < MAX_SIZE) {
            board[i][j] = 0;
            j++;
        }
        i++;
    }

    add_balls(number_balls, NULL);
}
//// ������� ��������� ������� "MAGIC LINES" 
//void draw_name(HDC hdc) {
//    HFONT hFont;
//    hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0,
//        DEFAULT_CHARSET, 0, 0, 0, 0,
//        L"Times New Roman"
//    );
//    SelectObject(hdc, hFont);
//    SetTextColor(hdc, RGB(0, 64, 64));
//    SetBkColor(hdc, BK_COLOR);
//
//    TCHAR  string1[30];
//    swprintf_s(string1, L"���������� �����: %4d", points);
//    TextOut(hdc, 100, 500, (LPCWSTR)string1, _tcslen(string1));
//}


// ������� ��������� ����� 
void draw_points(HDC hdc) {
    HFONT hFont;
    hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0,
        DEFAULT_CHARSET, 0, 0, 0, 0,
        L"Times New Roman"
    );
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(0, 64, 64));
    SetBkColor(hdc, BK_COLOR);
    
    TCHAR  string1[30];
    swprintf_s(string1, L"���������� �����: %4d", points);
    TextOut(hdc, 100, 500, (LPCWSTR)string1, _tcslen(string1));
}


// ������� board_paint(HDC hdc, int square_size)
// ������ ���� � ������ �� ���
void board_paint(HDC hdc, int square_size)
{
    HBRUSH hBrush;
    HPEN hPen;
    int x0 = 100, y0 = 100;
    int x = 100, y = 100;
    // ��������� ����� �� ���������
    while (x < MAX_SIZE * square_size + x0) {
        y = 100;
        while (y < MAX_SIZE * square_size + y0) {
            RECT square = { x, y, x + square_size, y + square_size };
            Rectangle(hdc, square.left, square.top, square.right, square.bottom);
            y = y + square_size;
        }
        x = x + square_size;
    }

    // ��������� �������
    int i = 0, j;
    int delta_zise = 5;
    while (i < MAX_SIZE) {
        j = 0;
        while (j < MAX_SIZE) {
            if (board[i][j] > 0) {
                RECT square = { i * square_size + x0 + delta_zise, j * square_size + y0 + delta_zise, i * square_size + square_size + x0 - delta_zise, j * square_size + square_size + y0 - delta_zise };
                hBrush = CreateSolidBrush(colors[board[i][j] - 1]);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                Ellipse(hdc, square.left, square.top, square.right, square.bottom);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hBrush);
            }
            j++;
        }
        i++;
    }
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    // ��������� ������, �� ������� ������
    hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    RECT square = { selected_cell.x * square_size + x0, selected_cell.y * square_size + y0, selected_cell.x * square_size + square_size + x0, selected_cell.y * square_size + square_size + y0 };
    Rectangle(hdc, square.left, square.top, square.right, square.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    // ��������� ��������� ��� ����������� ������, ���� ��� ����
    if (is_selected) {
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
        hOldPen = (HPEN)SelectObject(hdc, hPen);
        square = { saved_cell.x * square_size + x0, saved_cell.y * square_size + y0, saved_cell.x * square_size + square_size + x0, saved_cell.y * square_size + square_size + y0 };
        Rectangle(hdc, square.left, square.top, square.right, square.bottom);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
    draw_points(hdc);

}
// ������� ��������� ������ !!!
void Ball(HDC hdc, RECT square, HBRUSH hBrush, COLORREF color) {
    hBrush = CreateSolidBrush(color);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, square.left, square.top, square.right, square.bottom);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    // �����
    // �������� ���� ����� �� ����� ������
    //hBrush = CreateSolidBrush(color);
    //HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    //Ellipse(hdc, square.left + 5, square.top + 5, square.right - 5, square.bottom - 5);
    //SelectObject(hdc, hOldBrush);
    //DeleteObject(hBrush);
    // �������� ���� �� ����� �������
    //hBrush = CreateSolidBrush(color);
    //HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    //Ellipse(hdc, square.left, square.top, square.right, square.bottom - (square.top - square.bottom) / 2);
    //SelectObject(hdc, hOldBrush);
    //DeleteObject(hBrush);
}



bool point_in_range(int i, int j) {
    if ((i >= 0) && (i < MAX_SIZE) && (j >= 0) && (j < MAX_SIZE)) return true;
    return false;
}

// ������� int wave_alorithm(int x1, int y1, int x2, int y2)
// x1, x2 - ������� �����
// x2,y2 - �������� �����
// ������� ��������� ��������� 
bool wave_algorithm(int x1, int y1, int x2, int y2, int d, int dublicat_board[MAX_SIZE][MAX_SIZE]) {
    int i = 0, j = 0;
    int value = 1;
    // �������� ��������������� �����
    do {
        for (i = 0; i < MAX_SIZE; i++) {
            for (j = 0; j < MAX_SIZE; j++) {
                if ((dublicat_board[i][j] == value)) {
                    if (point_in_range(i - 1, j))
                        if ((dublicat_board[i - 1][j] != IS_OCCUPIED) && (dublicat_board[i - 1][j] == 0)) dublicat_board[i - 1][j] = value + 1;
                    if (point_in_range(i + 1, j))
                        if ((dublicat_board[i + 1][j] != IS_OCCUPIED) && (dublicat_board[i + 1][j] == 0)) dublicat_board[i + 1][j] = value + 1;
                    if (point_in_range(i, j - 1))
                        if ((dublicat_board[i][j - 1] != IS_OCCUPIED) && (dublicat_board[i][j - 1] == 0)) dublicat_board[i][j - 1] = value + 1;
                    if (point_in_range(i, j + 1))
                        if ((dublicat_board[i][j + 1] != IS_OCCUPIED) && (dublicat_board[i][j + 1] == 0)) dublicat_board[i][j + 1] = value + 1;
                }
            }
        }
        value = value + 1;
    } while ((dublicat_board[x2][y2] == 0) && (value < MAX_SIZE * MAX_SIZE));
    // ���� ��������� �������� ������
    if (dublicat_board[x2][y2] != 0) return true;
    return false;

}

// ������� bool check_path(x1, y1, x2, y2) !!!! ��������
// ���������, ���� �� ���� �� ����� (x1, y1) �� ����� (x2, y2). ���������� "��������" ��������
bool check_path(int x1, int y1, int x2, int y2) {
    int dublikat[MAX_SIZE][MAX_SIZE];
    int i = 0, j = 0;
    // �������� ���� � ������� ��������(0) � �������������(65535)
    while (i < MAX_SIZE) {
        j = 0;
        while (j < MAX_SIZE) {
            dublikat[i][j] = 0;
            if (board[i][j] > 0)
                dublikat[i][j] = IS_OCCUPIED;
            j++;
        }
        i++;
    }
    dublikat[x1][y1] = 1; // ����� � ������� ???
    // ���������� ����-��������� ������ � ���� � ������� ������� wave_algorithm(). �� ���� ������ ��������� �� ������ ������� ���������� ������� dublicat
    bool res = wave_algorithm(x1, y1, x2, y2, 0, dublikat);
    return res;
}


// ������� void move_ball(x1, y1, x2, y2)
// ���������� ����� � ������� x1, y1 �� ������� x2, y2
void move_ball(int x1, int y1, int x2, int y2) {
    board[x2][y2] = board[x1][y1];
    board[x1][y1] = 0;
}

// ������� void delete_balls(int x, int y)
// ������� ������ �� ��������� � �����������
bool delete_balls(int x, int y) {
    if (board[x][y] == 0) {
        return false;
    }
    struct point_t coords_to_delete[MAX_SIZE * MAX_SIZE];
    struct point_t temp[MAX_SIZE * MAX_SIZE];
    int start_n = 0;
    int number = 0;
    int i;
    // ����� ���������� ���������� �� �����������
    // ������
    for (i = x + 1; (i < MAX_SIZE) && (board[i][y] == board[x][y]); i++) {
        struct point_t p = { i, y };
        temp[start_n] = p;
        start_n++;
    }
    // �����
    for (i = x; (i > -1); i--) {
        struct point_t p = { i, y };
        if (board[i][y] != board[x][y]) break;
        temp[start_n] = p;
        start_n++;
    }

    if (start_n > 3) {
        for (i = 0; i < start_n; i++) {
            coords_to_delete[i] = temp[i];
        }
        number = start_n; // ���������� ��������� ���������� ����� �� ��������
    }
    // ����
    start_n = 0;
    for (i = y + 1; (i < MAX_SIZE) && (board[x][i] == board[x][y]); i++) {
        struct point_t p = { x, i };
        temp[start_n] = p;
        start_n++;
    }
    // �����
    for (i = y; (i > -1); i--) {
        if (board[x][i] != board[x][y]) break;
        temp[start_n].x = x;
        temp[start_n].y = i;
        start_n++;
    }

    if (start_n > 3) {
        for (i = 0; i < start_n; i++) {
            coords_to_delete[i + number] = temp[i];
        }
        number += start_n; // ���������� ��������� ���������� ����� �� �������� 
    }

    // �������� ��������� ����� ������� (��������� �������� �� ����)
    bool flag = false;
    for (i = 0; i < number; i++) {
        board[coords_to_delete[i].x][coords_to_delete[i].y] = 0;
        flag = true;
        points += 10;
    }
    return flag;
}

// ������� test_delete_balls()
// ��������� ���� ��������� ����������
void test_delete_balls() {
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 2) {
                board[i][j] = 2;
            }
        }
    }
}

// ������� bool board_is full()
// ��������� ������� ��������� ������ �� ����
bool board_is_full() {
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

// ������� ���������� ����
int saveLevel() {
    FILE* fout = fopen(filename, "w");

    if (fout == NULL) {
        printf("File %s is not opened", filename);
        return 0;
    }
    fprintf(fout, "%d ", n);
    fprintf(fout, "%d\n", m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(fout, "%d ", board[i][j]);
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "%d\n", points);
    fclose(fout);
    return 1;
}

// ������� �������� ����
int loadLevel() {
    FILE* fin = fopen(filename, "rt");

    if (fin == NULL) {
        printf("File %s is not opened", filename);
        return 0;
    }

    fscanf(fin, "%d", &n);
    fscanf(fin, "%d", &m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(fin, "%d", &board[i][j]);
        }
    }
    fscanf(fin, "%d", &points);
    fclose(fin);
    return 1;
}

void new_game() {
    // ������������� ����
    init_board(BALLS_FIRST);

    // ��������� ����������
    // ��������� ������
    selected_cell = { 0, 0 };
    // ����������� ������
    saved_cell = { 0, 0 };
    // ���� ������ ������
    is_selected = false;

    n = MAX_SIZE;
    m = MAX_SIZE;

    points = 0;
}