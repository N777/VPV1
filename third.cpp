#pragma once
#include "Log.h"
#include "gendata.cpp"

void third() {
    setprecision(0);
    int sum = 0;
    int num = 0;
    double arr[2] = { 0,0 };
    while (arr[0] == 0 || arr[1] == 0) {
        num += 10;
        for (int i = 0; i < 2; i++)
        {
            clock_t start = clock();
            for (int i = 0; i < num; i++)
            {
                sum += 1;
            }
            clock_t end = clock();
            arr[i] = double(end - start) / CLOCKS_PER_SEC;
        }
    }
    cout << "clock = " << arr[0] << endl;

    sum = 0;
    num = 0;
    arr[0] = 0;
    arr[1] = 0;
    while (arr[0] == 0 || arr[1] == 0) {
        num += 1;
        for (int i = 0; i < 2; i++)
        {
            LARGE_INTEGER t_start, t_finish, freq;
            __int64 t_code;
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&t_start);
            for (int i = 0; i < num; i++)
            {
                sum += 1;
            }
            QueryPerformanceCounter(&t_finish);
            t_code = t_finish.QuadPart - t_start.QuadPart;
            arr[i] = double(t_code) / freq.QuadPart;
        }
    }
    cout << "QPC = " << arr[0] << endl;

    sum = 0;
    num = 0;
    arr[0] = 0;
    arr[1] = 0;
    while (arr[0] == 0 || arr[1] == 0) {
        num += 1;
        for (int i = 0; i < 2; i++)
        {
            __int64 t_start, t_finish;
            t_start = __rdtsc();
            t_finish = __rdtsc();
            arr[i] = double(t_finish - t_start) / getFrequency();
        }
    }
    cout << "TSC = " << arr[0] << endl;
}