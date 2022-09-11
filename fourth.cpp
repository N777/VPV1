#pragma once
#include "Log.h"
#include "gendata.cpp"


void fourth() {
    Log logger;
    logger.config({ {PREC_AVG, 2},  // ��� �������� � ��� ������ ����� ������� ��������
                 {FILTR_MIN,0},{FILTR_MAX, 0} }); // ��� ����������
    setlocale(LC_CTYPE, "rus");
    int nPasses = 10;
    vector<double> result;
    vector <vec> arr = gen_data();
    retry = 1e5;


    for (int n = 1; n <= nPasses; n++) {
        cout << "\n����� " << n << endl << endl;
        logger.series_with_parametr(true, 1000, �lockInterval, arr).set_scale(NS_IN_SEC)
                .calc().stat("����� ���������� ����� clock (��� ����������)").print(10);
    }

    for (int n = 1; n <= nPasses; n++) {
        cout << "\n����� " << n << endl << endl;
        logger.series_with_parametr(true, 1000, �lockIntervalUsingQPC, arr)
                .calc().stat("����� ���������� ����� QPC (��� ����������)").print(10);
    }

    for (int n = 1; n <= nPasses; n++) {
        cout << "\n����� " << n << endl << endl;
        logger.series_with_parametr(true, 1000, �lockIntervalUsingTSC, arr)
                .calc().stat("����� ���������� ����� TSC (��� ����������)").print(10);
    }
}
