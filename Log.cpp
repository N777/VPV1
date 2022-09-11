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
#include "second.cpp"
#include "third.cpp"
#include "fourth.cpp"
#include "fifth.cpp"

using namespace std;


// Макросы повторения для генерации продуктов разворачивания циклов
#define Repeat10(x) x x x x x x x x x x 
#define Repeat100(x) Repeat10(Repeat10(x))
#define Repeat1000(x) Repeat100(Repeat10(x))
#define Repeat10000(x) Repeat100(Repeat100(x))

#define R2 0
#define R3 0
#define R4 1
#define R5 0



int main() {

#if R2
	second();
	countOfNumbers = 40;
	retry = 1;
	second(50);
#endif
#if R3
	third();
#endif
#if R4
	fourth();
#endif
#if R5
	fifth();
#endif
	return 0;
}