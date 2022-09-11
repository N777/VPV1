#pragma once
#include "Log.h"
#include "gendata.cpp"

unsigned __int64 fibRecursive(unsigned __int64 n) {
    if (n < 2) return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

double �lockInterval() {
    unsigned __int64 res;
    clock_t t_start, t_finish;
    clock_t t_clock = clock();
    while (clock() < t_clock + 1);
    t_start = clock();
    for (int i = 0; i < retry; i++)
    {
        res = fibRecursive(countOfNumbers);
    }
    while (clock() < t_clock + 2);
    t_finish = clock();
    trash(res);
    return double(t_finish - t_start) / retry / CLOCKS_PER_SEC;
}


// ������������ ������ clock-��������� � ������� QPC
double �lockIntervalUsingQPC() {
    LARGE_INTEGER t_start, t_finish, freq;
    __int64 t_code;
    QueryPerformanceFrequency(&freq);
    clock_t t_clock = clock();
    while (clock() < t_clock + 1);
    QueryPerformanceCounter(&t_start);
    for (int i = 0; i < retry; i++)
    {
        fibRecursive(countOfNumbers);
    }
    while (clock() < t_clock + 2);
    QueryPerformanceCounter(&t_finish);
    t_code = t_finish.QuadPart - t_start.QuadPart;
    return double(t_code) / freq.QuadPart / retry;
};

// ������������ ������ �lock-��������� � ������ TSC
double �lockIntervalUsingTSC() {
    __int64 t_start, t_finish;
    clock_t t_clock = clock();
    while (clock() < t_clock + 1);
    t_start = __rdtsc();
    fibRecursive(countOfNumbers);
    while (clock() < t_clock + 2);
    t_finish = __rdtsc();
    return double(t_finish - t_start) / getFrequency();
};

void second(int count_try = 1000) {
    Log log;
    double scale; // ������� ��������� ��������
    scale = MCS_IN_SEC; // �������� ����� �������� � �������������
    int nPasses = 1;  // ����� �������� ��� ���������� ����� ���������
    setlocale(LC_CTYPE, "rus");
    log.config({ {PREC_AVG, 2},  // ��� �������� � ��� ������ ����� ������� ��������
                 {FILTR_MIN,0},{FILTR_MAX, 0} }); // ��� ����������

    log.series(true, 1, �lockInterval)
            .calc().set_scale(NS_IN_SEC).stat("������ ������������� ��������� ����� clock");

    log.series(true, count_try, �lockInterval)
            .calc().set_scale(NS_IN_SEC).stat("������ ������������� ��������� ����� clock");

    log.series(true, count_try, �lockIntervalUsingQPC)
            .calc().set_scale(NS_IN_SEC).stat("������ ������������� ��������� ����� QPC ��� ����������");

    log.series(true, count_try, �lockIntervalUsingTSC)
            .calc().set_scale(scale).stat("������ ������������� ��������� ����� TSC ��� ����������");

}