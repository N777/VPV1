#pragma once
#include "Log.h"
#include "gendata.cpp"

unsigned __int64 fibRecursive(unsigned __int64 n) {
    if (n < 2) return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

double сlockInterval() {
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


// Длительность одного clock-интервала с помощью QPC
double сlockIntervalUsingQPC() {
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

// Длительность одного сlock-интервала в тактах TSC
double сlockIntervalUsingTSC() {
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
    double scale; // масштаб выводимых значений
    scale = MCS_IN_SEC; // Значения будем выводить в микросекундах
    int nPasses = 1;  // число проходов при выполнении серий измерений
    setlocale(LC_CTYPE, "rus");
    log.config({ {PREC_AVG, 2},  // Для среднего и СКО задаем более высокую точность
                 {FILTR_MIN,0},{FILTR_MAX, 0} }); // нет фильтрации

    log.series(true, 1, сlockInterval)
            .calc().set_scale(NS_IN_SEC).stat("Оценка повторяемости интервала через clock");

    log.series(true, count_try, сlockInterval)
            .calc().set_scale(NS_IN_SEC).stat("Оценка повторяемости интервала через clock");

    log.series(true, count_try, сlockIntervalUsingQPC)
            .calc().set_scale(NS_IN_SEC).stat("Оценка повторяемости интервала через QPC без фильтрации");

    log.series(true, count_try, сlockIntervalUsingTSC)
            .calc().set_scale(scale).stat("Оценка повторяемости интервала через TSC без фильтрации");

}