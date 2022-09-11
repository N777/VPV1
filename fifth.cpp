#pragma once
#include "Log.h"
#include "gendata.cpp"

void fifth() {
    Log logger;
    double scale; // ������� ��������� ��������
    setlocale(LC_CTYPE, "rus");
    double res;
    vector<double> result;
    vector <vec> arr = gen_data();
    int step = 1e3;
    for (int i = 0; i < 1000; i++)
    {
        clock_t start = clock();
        for (int j = 0; j < i*step; j++)
        {
            res = arr[i].a* log((1 + arr[i].x) / (1 - arr[i].x)) / arr[i].b;
        }
        clock_t end = clock();
        result.push_back(double(end - start)/CLOCKS_PER_SEC);
    }

    cout << "\n\n������ ������������� 1000 ��������� clock - ��������� � 200\n";
    logger.config({ {PREC_AVG, 2},  // ��� �������� � ��� ������ ����� ������� ��������
                    {FILTR_MIN,0},{FILTR_MAX, 0} }); // ��� ����������
    logger.set(result).set_scale(MCS_IN_SEC)
            .calc().stat("").printk().write_in_file("impClock.txt").print(10);

    logger.clear();
    result.clear();
    for (int i = 0; i < 1000; i++)
    {
        LARGE_INTEGER t_start, t_finish, freq;
        __int64 t_code;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&t_start);
        for (int j = 0; j < i*step; j++)
        {
            res = arr[i].a* log((1 + arr[i].x) / (1 - arr[i].x)) / arr[i].b;
        }
        QueryPerformanceCounter(&t_finish);
        t_code = t_finish.QuadPart - t_start.QuadPart;
        result.push_back(double(t_code) / freq.QuadPart);
    }

    cout << "\n\n������ ������������� 1000 ��������� QPC - ��������� � 200 \n";
    logger.set(result).set_scale(NS_IN_SEC)
            .calc().stat( "").printk().write_in_file("impQPC.txt").print(10);
    logger.clear();
    result.clear();
    for (int i = 0; i < 1000; i++)
    {
        __int64 t_start, t_finish;
        t_start = __rdtsc();
        for (int j = 0; j < i*step; j++)
        {
            res = arr[i].a* log((1 + arr[i].x) / (1 - arr[i].x)) / arr[i].b;
        }
        t_finish = __rdtsc();
        result.push_back(double(t_finish - t_start) / getFrequency());
    }

    cout << "\n\n������ ������������� 1000 ��������� TSC - ��������� � 200 \n";
    logger.set(result).set_scale(NS_IN_SEC)
            .calc().stat("").printk().write_in_file("impTSC.txt").print(10);
}

