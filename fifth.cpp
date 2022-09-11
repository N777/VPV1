#pragma once
#include "Log.h"
#include "gendata.cpp"

void fifth() {
    Log logger;
    logger.config({ {PREC_AVG, 2},  // Для среднего и СКО задаем более высокую точность
                    {FILTR_MIN,0},{FILTR_MAX, 0} }).set_scale(NS_IN_SEC); // нет фильтрации
    double scale; // масштаб выводимых значений
    scale = 1; // Значения будем выводить в микросекундах
    setlocale(LC_CTYPE, "rus");
    int nPasses = 10;
    vector<double> result;
    vector <vec> arr = gen_data();
    retry = 1e2;
    cout.precision(6);

    for (int n = 9; n <= nPasses; n++) {
        cout << "\nСерия " << n << endl << endl;
        long multi = 2e2;
        logger.series_with_parametr(true, 1000, сlockInterval, arr, multi).printk().diffK()
                .calc().stat("Число наносекунд через clock (без фильтрации)");
    }

    for (int n = 8; n <= nPasses; n++) {
        cout << "\nСерия " << n << endl << endl;
        long multi = 1e2;
        logger.series_with_parametr(true, 1000, сlockIntervalUsingQPC, arr, multi).printk().diffK()
                .calc().stat("Число наносекунд через QPC (без фильтрации)");
    }

    for (int n = 8; n <= nPasses; n++) {
        cout << "\nСерия " << n << endl << endl;
        long multi = 2e2;
        logger.series_with_parametr(true, 1000, сlockIntervalUsingTSC, arr, multi).printk().diffK()
                .calc().stat("Число наносекунд через TSC (без фильтрации)");
    }
}

