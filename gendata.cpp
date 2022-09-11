#pragma once
#include "Log.h"
#include <random>


#define NS_IN_SEC 1.E9 // число наносекунд в секунде
#define MCS_IN_SEC 1.E6 // число микросекунд в секунде
int countOfNumbers = 10; // сколько чисел Фибаначи требуется
unsigned __int64 retry = 2000;

void trash(unsigned __int64 val) {
    ofstream out;
    out.open("trash.txt");
    out << val;
    out.close();
}

vector <vec> gen_data() {
    vector <vec> arr;
    random_device rd;   // non-deterministic generator
    default_random_engine gen(rd());
    uniform_real_distribution<float> gendouble(0, 1);
    uniform_real_distribution<> genint(1, 100);
    for (int i = 0; i < 1000; i++)
    {
        vec temp;
        temp.a = genint(gen);
        temp.b = genint(gen);
        temp.x = gendouble(gen);
        arr.push_back(temp);
    }
    return arr;
}

double function(vec x, int n){
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += x.a* log((1 + x.x) / (1 - x.x)) / x.b;
    }
    return sum;
}


double сlockInterval(vec x, int n = 1) {
    double res;
    clock_t t_start, t_finish;
    clock_t t_clock = clock();
    while (clock() < t_clock + 1);
    t_start = clock();
    for (int i = 0; i < retry; i++)
    {
        res = function(x, n);
    }
    while (clock() < t_clock + 2);
    t_finish = clock();
    trash(res);
    return double(t_finish - t_start) / retry / CLOCKS_PER_SEC;
}


// Длительность одного clock-интервала с помощью QPC
double сlockIntervalUsingQPC(vec x, int n = 1) {
    double res;
    LARGE_INTEGER t_start, t_finish, freq;
    __int64 t_code;
    QueryPerformanceFrequency(&freq);
    clock_t t_clock = clock();
    while (clock() < t_clock + 1);
    QueryPerformanceCounter(&t_start);
    for (int i = 0; i < retry; i++)
    {
        res = function(x, n);
    }
    while (clock() < t_clock + 2);
    QueryPerformanceCounter(&t_finish);
    t_code = t_finish.QuadPart - t_start.QuadPart;
    trash(res);
    return double(t_code) / freq.QuadPart / retry;
};

unsigned long long getFrequency() {
    clock_t tclock = clock();
    while (clock() < tclock + 1); // ожидание конца начавшегося такта
    tclock = clock();
    unsigned long long tsc = __rdtsc();
    while (clock() < tclock + 1);	// ожидание конца начавшегося такта
    unsigned long long tscEnd = __rdtsc();
    unsigned long long tscDelta = tscEnd - tsc;// сколько тактов TSC прошло за один такт clock
    unsigned long long Fl = (tscDelta * CLOCKS_PER_SEC); // частота процессора
    tclock = clock(); tsc = __rdtsc();
    while (clock() < tclock + 1); // ожидание конца начавшегося такта
    tscEnd = __rdtsc(); tscDelta = tscEnd - tsc;// сколько тактов TSC прошло за один такт clock
    unsigned long long F2 = (tscDelta * CLOCKS_PER_SEC); // частота процессора
    return min(Fl, F2);
}

// Длительность одного сlock-интервала в тактах TSC
double сlockIntervalUsingTSC(vec x, int n = 1) {
    double res;
    __int64 t_start, t_finish;
    clock_t t_clock = clock();
    while (clock() < t_clock + 1);
    t_start = __rdtsc();
    res = function(x, n);
    while (clock() < t_clock + 2);
    t_finish = __rdtsc();
    trash(res);
    return double(t_finish - t_start) / getFrequency();
};