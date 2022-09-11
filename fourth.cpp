#pragma once
#include "Log.h"
#include "gendata.cpp"


void fourth() {
    Log logger;
    double scale; // ������� ��������� ��������
    scale = 1; // �������� ����� �������� � �������������
    setlocale(LC_CTYPE, "rus");
    int nPasses = 10;
    vector<double> result;
    vector <vec> arr = gen_data();
    __int64 stop = 1e5;

    while (1) {
        clock_t start = clock();
        for (int i = 0; i < stop; i++)
        {
            arr[0].a* log((1 + arr[0].x) / (1 - arr[0].x)) / arr[0].b;
        }
        clock_t end = clock();
        if (end - start >= 200)
            break;
        stop += 5e5;
    }



    for (int n = 1; n <= nPasses; n++) {
        cout << "\n����� " << n << endl << endl;
        logger.series_with_parametr(true, 1000, �lockInterval, arr)
                .calc().stat(scale, "����� ����������� � ����� clock-��������� ����� QPC (��� ����������)")
                .print(O_NATURAL, scale, 10).print(O_MIN, scale, 10).print(O_MAX, scale, 10);;
    }



    for (int i = 0; i < 1000; i++)
    {
        clock_t start = clock();
        for (int j = 0; j < stop; j++)
        {
            ;
        }
        clock_t end = clock();
        result.push_back(end - start);
    }

    cout << "\n\n������ ������������� 1000 ��������� clock - ��������� � 200\n";
    //cout << stop;
    logger.config({ {PREC_AVG, 2},  // ��� �������� � ��� ������ ����� ������� ��������
                    {FILTR_MIN,0},{FILTR_MAX, 0} }); // ��� ����������
    logger.set(result).set_scale(scale)
            .calc().stat("").write_in_file("Clock.txt").print(10);
    logger.clear();
    result.clear();
    stop = 1;
    while (1) {
        LARGE_INTEGER t_start, t_finish, freq;
        __int64 t_code;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&t_start);
        for (int i = 0; i < stop; i++)
        {
            arr[0].a* log((1 + arr[0].x) / (1 - arr[0].x)) / arr[0].b;
        }
        QueryPerformanceCounter(&t_finish);
        t_code = t_finish.QuadPart - t_start.QuadPart;
        if (t_code >= 200)
            break;
        stop += 1;
    }
    for (int i = 0; i < 1000; i++)
    {
        LARGE_INTEGER t_start, t_finish, freq;
        __int64 t_code;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&t_start);
        for (int j = 0; j < stop; j++)
        {
            arr[i].a* log((1 + arr[i].x) / (1 - arr[i].x)) / arr[i].b;
        }
        QueryPerformanceCounter(&t_finish);
        t_code = t_finish.QuadPart - t_start.QuadPart;
        result.push_back(double(t_code));
    }

    cout << "\n\n������ ������������� 1000 ��������� QPC - ��������� � 200 \n";
    //cout << stop;
    logger.config({ {PREC_AVG, 2},  // ��� �������� � ��� ������ ����� ������� ��������
                    {FILTR_MIN,0},{FILTR_MAX, 0} }); // ��� ����������
    logger.set(result).set_scale(scale)
            .calc().stat("").write_in_file("QPC.txt").print(10);
    logger.clear();
    result.clear();
    stop = 1;
    while (1) {
        __int64 t_start, t_finish;
        t_start = __rdtsc();
        for (int i = 0; i < stop; i++)
        {
            arr[0].a* log((1 + arr[0].x) / (1 - arr[0].x)) / arr[0].b;
        }
        t_finish = __rdtsc();
        if (t_finish - t_start >= 200)
            break;
        stop += 1;
    }
    for (int i = 0; i < 1000; i++)
    {
        __int64 t_start, t_finish;
        t_start = __rdtsc();
        for (int j = 0; j < stop; j++)
        {
            arr[i].a* log((1 + arr[i].x) / (1 - arr[i].x)) / arr[i].b;
        }
        t_finish = __rdtsc();
        result.push_back(double(t_finish - t_start));
    }

    cout << "\n\n������ ������������� 1000 ��������� TSC - ��������� � 200 \n";
    //cout << stop;
    logger.config({ {PREC_AVG, 2},  // ��� �������� � ��� ������ ����� ������� ��������
                    {FILTR_MIN,0},{FILTR_MAX, 0} }); // ��� ����������
    logger.set(result).set_scale(scale)
            .calc().stat("").write_in_file("TSC.txt").print(10);
}
