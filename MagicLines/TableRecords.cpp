#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include <stdio.h>


#define MAX_NUM_RECORD 10
#define BK_COLOR RGB(180, 200, 107)

extern int points;
// ��������� � ����������� � �������
struct Record {
    char name[20];
    int points;
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
};

// ������������ ���������� �������� � �������
#define MAX_NUM_RECORDS 10

// ������� ��������
struct Record records[MAX_NUM_RECORDS + 1];
// ������� ���������� �������� � �������
int numRecords = 0;

// ������� ���������� 
char* encode_str(char s[]) {
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] += 15;
    }
    return s;
}

// ������� ������������ 
char* decode_str(char s[]) {
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] -= 15;
    }
    return s;
}

int record_is_filled(int i) {
    if (records[i].day && records[i].month && records[i].year &&
        records[i].hour && records[i].minute && records[i].second &&
        records[i].name && records[i].points)
        return 1;
    return 0;
}

void DrawRecords(HDC hdc) {
    HFONT hFont;
    hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0,
        DEFAULT_CHARSET, 0, 0, 0, 0,
        L"Arial"
    );
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(0, 64, 64));
    SetBkColor(hdc, BK_COLOR);

    
    TextOut(hdc, 60, 30, L"������� ��������", 17);
    //SetBkColor(hdc, RGB(239, 228, 176));
    TCHAR  string1[] = _T(" �   ����            �����      ���               ����   ");
    TextOut(hdc, 60, 80, (LPCWSTR)string1, _tcslen(string1));

    int i;
    for (i = 0; i < numRecords; i++) {
        TCHAR  string2[80];
        char str[80];
        int n = 9;
        if (record_is_filled(i)) {
            sprintf(str, "  %2d   %02d.%02d.%04d  %02d:%02d:%02d  %-18s  %-4d", 
                i + 1,
                records[i].day, records[i].month, records[i].year,
                records[i].hour, records[i].minute, records[i].second,
                records[i].name, records[i].points
            );
            
            OemToChar(str, string2);
            TextOut(hdc, 60, 24 * (i + 1) + 80, (LPCWSTR)string2, _tcslen(string2));
        }
    }
    MoveToEx(hdc, 50, 70, NULL);
    LineTo(hdc, 550, 70);
    LineTo(hdc, 550, numRecords * 30);
    LineTo(hdc, 50, numRecords * 30);
    LineTo(hdc, 50, 70);
    DeleteObject(hFont);
}


int CompareRecords(int index1, int index2)
{
    if (records[index1].points < records[index2].points)
        return -1;
    if (records[index1].points > records[index2].points)
        return +1;

    return 0;
   

}

void InsertRecord(char name[])
{
    strcpy(records[numRecords].name, name);
    records[numRecords].points = points;
   

    SYSTEMTIME st;
    // �������� ������� �����
    GetLocalTime(&st);

    // � ������������ ��� �� ����� � ������� ��������
    records[numRecords].year = st.wYear;
    records[numRecords].month = st.wMonth;
    records[numRecords].day = st.wDay;

    records[numRecords].hour = st.wHour;
    records[numRecords].minute = st.wMinute;
    records[numRecords].second = st.wSecond;
    // ���������� ������ � ������ ������� - ���� � ��� 
    // ������� ���������
    int i = numRecords;
    while (i > 0) {
        if (CompareRecords(i - 1, i) < 0) {
            struct Record temp = records[i];
            records[i] = records[i - 1];
            records[i - 1] = temp;
        }
        i--;
    }
    // ���� ������� ��������� �� ���������
    if (numRecords < MAX_NUM_RECORDS)
        // ��������� ��� ����� ������ ����� ������� � ����� �������
        numRecords++;
}

void addRecord(char name[])
{
    //if (numRecords >= MAX_NUM_RECORDS) {
    //numRecords = numRecords - 1;
    //}

    strcpy(records[numRecords].name, name);
    records[numRecords].points = points;
    

    SYSTEMTIME st;
    // �������� ������� �����
    GetLocalTime(&st);

    // � ������������ ��� �� ����� � ������� ��������
    records[numRecords].year = st.wYear;
    records[numRecords].month = st.wMonth;
    records[numRecords].day = st.wDay;

    records[numRecords].hour = st.wHour;
    records[numRecords].minute = st.wMinute;
    records[numRecords].second = st.wSecond;
    // ��������� ��� ����� ���������� ������ � ��������� �������	
    numRecords++;
}

char filenameRecords[] = "records.txt";

void SaveRecords() {
    // ������ � �������� ����
    FILE* fout = fopen(filenameRecords, "wt");
    if (fout == NULL) {
        // �������, �� �������� ���������� � ����
        return;
    }

    fprintf(fout, "%d\n", numRecords);

    int i;
    for (i = 0; i < numRecords; i++) {
        // ��������� � ����� ������ ���� ������� �������
        fprintf(fout, "%s %d %d %d %d %d %d %d\n",
            encode_str(records[i].name),
            records[i].points,
            records[i].year,
            records[i].month,
            records[i].day,
            records[i].hour,
            records[i].minute,
            records[i].second
        );
    }
    // ��������� ����
    fclose(fout);
}

void LoadRecords() {
    // ��������� ���� � ��������� �� ������
    FILE* fout = fopen(filenameRecords, "rt");
    if (fout == NULL) {
        // �������, �� �������� ������� �� �����
        return;
    }

    fscanf(fout, "%d", &numRecords); // ���������� �������� � �������

    int i;
    for (i = 0; i < numRecords; i++) {
        // ��������� �� ����� ������ ���� ������� �������
        fscanf(fout, "%s%d%d%d%d%d%d%d\n",
            records[i].name,
            &records[i].points,
            &records[i].year,
            &records[i].month,
            &records[i].day,
            &records[i].hour,
            &records[i].minute,
            &records[i].second
        );
        strcpy(records[i].name, decode_str(records[i].name));
            /*& records[i].points,
            & records[i].year,
            & records[i].month,
            & records[i].day,
            & records[i].hour,
            & records[i].minute,
            & records[i].second*/
    }
    
    // ��������� ����
    fclose(fout);
}

