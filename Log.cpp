/////////////////////////////////////////////////////////////
// Log.cpp - файл отладки-демонстрации использования класса 
// протоколирования, обработки, вывода результатов измерений 
// nvnulstu@gmail.com: февраль countOfNumbers22
// Предполагается, что файл класса Log.h находится в материнском директории,
// а несколько использующих его приложений находятся в дочерних

#include <iostream>
#include <Windows.h>
#include <time.h>
#include "Log.h"
#include <random>

using namespace std;

#define NS_IN_SEC 1.E9 // число наносекунд в секунде
#define MCS_IN_SEC 1.E6 // число микросекунд в секунде
int countOfNumbers = 10; // сколько чисел Фибаначи требуется
unsigned __int64 retry = 2000;
// Макросы повторения для генерации продуктов разворачивания циклов
#define Repeat10(x) x x x x x x x x x x 
#define Repeat100(x) Repeat10(Repeat10(x))
#define Repeat1000(x) Repeat100(Repeat10(x))
#define Repeat10000(x) Repeat100(Repeat100(x))


void trash(unsigned __int64 val) {
	ofstream out;
	out.open("trash.txt");
	out << val;
	out.close();
}

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

struct vec
{
	int a;
	double x;
	int b;
};

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

void fourth() {
	Log logger;
	double scale; // масштаб выводимых значений
	scale = 1; // Значения будем выводить в микросекундах
	setlocale(LC_CTYPE, "rus");
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
		for (int i = 0; i < 1000; i++)
		{
			clock_t start = clock();
			for (int j = 0; j < stop; j++)
			{
				arr[i].a* log((1 + arr[i].x) / (1 - arr[i].x)) / arr[i].b;
			}
			clock_t end = clock();
			result.push_back(end - start);
		}

		cout << "\n\nОценка повторяемости 1000 измерений clock - интервала в 200\n";
		//cout << stop;
		logger.config({ {PREC_AVG, 2},  // Для среднего и СКО задаем более высокую точность
			{FILTR_MIN,0},{FILTR_MAX, 0} }); // нет фильтрации
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

	cout << "\n\nОценка повторяемости 1000 измерений QPC - интервала в 200 \n";
	//cout << stop;
	logger.config({ {PREC_AVG, 2},  // Для среднего и СКО задаем более высокую точность
		{FILTR_MIN,0},{FILTR_MAX, 0} }); // нет фильтрации
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

	cout << "\n\nОценка повторяемости 1000 измерений TSC - интервала в 200 \n";
	//cout << stop;
	logger.config({ {PREC_AVG, 2},  // Для среднего и СКО задаем более высокую точность
		{FILTR_MIN,0},{FILTR_MAX, 0} }); // нет фильтрации
	logger.set(result).set_scale(scale)
		.calc().stat("").write_in_file("TSC.txt").print(10);
}

void fifth() {
	Log logger;
	double scale; // масштаб выводимых значений
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

		cout << "\n\nОценка повторяемости 1000 измерений clock - интервала в 200\n";
		logger.config({ {PREC_AVG, 2},  // Для среднего и СКО задаем более высокую точность
			{FILTR_MIN,0},{FILTR_MAX, 0} }); // нет фильтрации
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

	cout << "\n\nОценка повторяемости 1000 измерений QPC - интервала в 200 \n";
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

	cout << "\n\nОценка повторяемости 1000 измерений TSC - интервала в 200 \n";
	logger.set(result).set_scale(NS_IN_SEC)
		.calc().stat("").printk().write_in_file("impTSC.txt").print(10);
}


int main() {
	/*second();
	countOfNumbers = 40;
	retry = 1;
	second(50);*/
	//third();
	//fourth();
	fifth();
	return 0;
}