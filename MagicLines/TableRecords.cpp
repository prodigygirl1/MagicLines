#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include <stdio.h>
#include <stdlib.h>


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

char filenameRecords[] = "records.txt";

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

bool record_is_filled(int i) {
    return ( records[i].name );
}

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
        char s[8][20];
        if (record_is_filled(i)) {
            // ��������� � ����� ������ ���� ������� �������
            strcpy(s[0], records[i].name);
            fprintf(fout, "%s %s %s %s %s %s %s %s\n",
                encode_str(s[0]),             
                encode_str(_itoa(records[i].points, s[1], 10)),
                encode_str(_itoa(records[i].year, s[2], 10)),
                encode_str(_itoa(records[i].month, s[3], 10)),
                encode_str(_itoa(records[i].day, s[4], 10)),
                encode_str(_itoa(records[i].hour, s[5], 10)),
                encode_str(_itoa(records[i].minute, s[6], 10)),
                encode_str(_itoa(records[i].second, s[7], 10))
            );
        }
        ;
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

    char s[20];
    int i, j, in[7];
    for (i = 0; i < numRecords; i++) {
        // ��������� �� ����� ������ ���� ������� �������
        fscanf(fout, "%s", s);
        strcpy(records[i].name, decode_str(s));
        for (j = 0; j < 7; j++) {
            fscanf(fout, "%s", s);
            in[j] = atoi(decode_str(s));
        }
        records[i].points = in[0];
        records[i].year = in[1];
        records[i].month = in[2];
        records[i].day = in[3];
        records[i].hour = in[4];
        records[i].minute = in[5];
        records[i].second = in[6];
    }

    // ��������� ����
    fclose(fout);
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
    /*if (numRecords) {
        MoveToEx(hdc, 50, 70, NULL);
        LineTo(hdc, 550, 70);
        LineTo(hdc, 550, numRecords * 30 + 170);
        LineTo(hdc, 50, numRecords * 30 + 170);
        LineTo(hdc, 50, 70);
    }*/
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
    SaveRecords();
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






