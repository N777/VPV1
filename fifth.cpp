#pragma once
#include "Log.h"
#include "gendata.cpp"

void fifth() {
    Log logger;
    logger.config({ {PREC_AVG, 2},  // ��� �������� � ��� ������ ����� ������� ��������
                    {FILTR_MIN,0},{FILTR_MAX, 0} }).set_scale(NS_IN_SEC); // ��� ����������
    double scale; // ������� ��������� ��������
    scale = 1; // �������� ����� �������� � �������������
    setlocale(LC_CTYPE, "rus");
    int nPasses = 10;
    vector<double> result;
    vector <vec> arr = gen_data();
    retry = 1e2;
    cout.precision(6);

    for (int n = 9; n <= nPasses; n++) {
        cout << "\n����� " << n << endl << endl;
        long multi = 2e2;
        logger.series_with_parametr(true, 1000, �lockInterval, arr, multi).printk().diffK()
                .calc().stat("����� ���������� ����� clock (��� ����������)");
    }

    for (int n = 8; n <= nPasses; n++) {
        cout << "\n����� " << n << endl << endl;
        long multi = 1e2;
        logger.series_with_parametr(true, 1000, �lockIntervalUsingQPC, arr, multi).printk().diffK()
                .calc().stat("����� ���������� ����� QPC (��� ����������)");
    }

    for (int n = 8; n <= nPasses; n++) {
        cout << "\n����� " << n << endl << endl;
        long multi = 2e2;
        logger.series_with_parametr(true, 1000, �lockIntervalUsingTSC, arr, multi).printk().diffK()
                .calc().stat("����� ���������� ����� TSC (��� ����������)");
    }
}

