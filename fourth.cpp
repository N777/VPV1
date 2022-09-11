#pragma once
#include "Log.h"
#include "gendata.cpp"


void fourth() {
    Log logger;
    logger.config({ {PREC_AVG, 2},  // Для среднего и СКО задаем более высокую точность
                 {FILTR_MIN,0},{FILTR_MAX, 0} }); // нет фильтрации
    setlocale(LC_CTYPE, "rus");
    int nPasses = 10;
    vector<double> result;
    vector <vec> arr = gen_data();
    retry = 1e5;


    for (int n = 1; n <= nPasses; n++) {
        cout << "\nСерия " << n << endl << endl;
        logger.series_with_parametr(true, 1000, сlockInterval, arr).set_scale(NS_IN_SEC)
                .calc().stat("Число наносекунд через clock (без фильтрации)").print(10);
    }

    for (int n = 1; n <= nPasses; n++) {
        cout << "\nСерия " << n << endl << endl;
        logger.series_with_parametr(true, 1000, сlockIntervalUsingQPC, arr)
                .calc().stat("Число наносекунд через QPC (без фильтрации)").print(10);
    }

    for (int n = 1; n <= nPasses; n++) {
        cout << "\nСерия " << n << endl << endl;
        logger.series_with_parametr(true, 1000, сlockIntervalUsingTSC, arr)
                .calc().stat("Число наносекунд через TSC (без фильтрации)").print(10);
    }
}
