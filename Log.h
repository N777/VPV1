#pragma once
#include <iostream> 
#include <vector> 
#include <sstream> 
#include <numeric> 
#include <fstream>
#include <cmath> 
#include <intrin.h>
#include <algorithm>
#include <thread>
#include <omp.h>
#include <iomanip>
#include <map>


using namespace std;

struct vec
{
    int a;
    double x;
    int b;
};

// ��� ��������������: ������������, ������� �����������, ������� ������������
enum Order { O_NATURAL, O_MIN, O_MAX };
/* ��������� ���������������� ������� ���������� � ����������
   PREC_VAL - ����� ������ ����� ������� � ���������� ����������� � ��������� ��������
   PREC_AVG - ����� ������ ����� ������� � ����������
   FILTR_MIN, FILTR_MAX - ����� ������������� ���������� � ���������� ����� ��������� �������� � ���
*/
enum Config { CONFIG_FIRST = 0, PREC_VAL = 0, PREC_AVG, FILTR_MIN, FILTR_MAX, CONFIG_SIZE };

// ��������� ���������� � ���������� ����� ������� ������� __cpuid(unsigned *) info, funcCode),
// ������� ��������� ��������  ������� CPUID, �������������� �������� � EAX ��� ������� funcCode. 
// CPUID ���������� 4 �������������� ����: 0:EAX, 1:EBX, 2:ECX, 3:EDX, 
// � ������� __cpuid ���������� ��� ����� �� ������ info
void cpuInfo() {
	// int regs[4]; // 0:EAX, 1:EBX, 2:ECX, 3:EDX 
	char nameCPU[80]; // ��� ����������
	// ��������� ����� ����������
	// ���� 0x80000002..0x80000004 ��������� �������� ������ ��� CPU �� 16 ������
	__cpuid((int*)nameCPU, 0x80000002);
	__cpuid((int*)nameCPU + 4, 0x80000003);
	__cpuid((int*)nameCPU + 8, 0x80000004);
	int cores = thread::hardware_concurrency();

	cout << endl << nameCPU << "\t�������: " << cores << endl << endl;
}
// ����� ����������������, ���������, ������, ����������� ����� ���������
class Log {
public:
	double dmin = 1.0E10, dmax = 0.; // ����������� � ������������ �������
	double avg = 0., sqdev = 0., sqdev_perc = 0; // �������, ���, ���%
	vector <double> arr;
	double scale = 1., k = 0;
	int conf[CONFIG_SIZE] = { 0, 0, 0, 0 };
	ostringstream msgfiltr;
	Log() {}
	Log& calc() {
		vector<double> vect = arr;
		msgfiltr.str("");
		if (conf[FILTR_MIN] > 0) {
			sort(vect.begin(), vect.end());
			vect.erase(vect.begin(), vect.begin() + conf[FILTR_MIN]);
			msgfiltr << "\n������� " << conf[FILTR_MIN] << " ����������";
		}
		if (conf[FILTR_MAX] > 0) {
			sort(vect.begin(), vect.end(), [](double a, double b) {
				return a > b; });
			vect.erase(vect.begin(), vect.begin() + conf[FILTR_MAX]);
			msgfiltr << "  ������� " << conf[FILTR_MAX] << " ����������";
		}
		avg = accumulate(vect.begin(), vect.end(), 0.0, [&](double x, double y) {return x + y / vect.size(); });
		sqdev = sqrt(accumulate(vect.begin(), vect.end(), 0.,
			[&](double sq, double v) {double q = avg - v; return sq + q * q; })
			/ vect.size());
		sqdev_perc = 100 * sqdev / avg;
		dmin = *min_element(vect.begin(), vect.end());
		dmax = *max_element(vect.begin(), vect.end());
		//cout << "min: " << dmin << "  max: " << dmax << endl;
		return *this;
	}
	Log& config(map< int, int > param) {
		for (auto it = param.begin(); it != param.end(); it++) {
			int n = (*it).first;
			if (n >= CONFIG_FIRST && n < CONFIG_SIZE)
				conf[n] = (*it).second;
		}
		return *this;
	}
	// ����� ���������, ������ �� ������� ����������� �������� meter, 
	// ������������ ����� � ��������  
	Log& series(bool clear, int count, double (meter)()) {
		if (clear) arr.clear();
		for (int n = 0; n < count; n++) {
			arr.push_back(meter());
		}
		return *this;
	}

    Log& series_with_parametr(bool clear, int count, double (meter)(vec, int), vector<vec> data, int num = 0) {
        if (clear) arr.clear();
        for (int n = 0; n < count; n++) {
            arr.push_back(meter(data[n], num ? n * num : 1));
        }
        return *this;
    }

    Log& diffK(){
        for (int i = 0; i < arr.size(); ++i) {
            arr[i] = abs(arr[i] - k * i);
        }
        return *this;
    }

	// ������� ������ ���������
	Log& clear() {
		arr.clear();
		return *this;
	}
	// ���������� ������ ����������� ���������
	Log& set(vector <double> source) {
		for (double v : source)
			arr.push_back(v);
		return *this;
	}
	// ������������ ���������� ����������� ���������
	Log& stat(string unit) {
		if (dmin > dmax&& arr.size() > 0) {
			cout << "\n������� ������� ���������� ��� ���������� calc()" << endl;
		}
		cout.setf(ios::fixed);
		cout << "���������� " << arr.size() << " ���������(" << unit << ")" << msgfiltr.str() << endl
			<< setprecision(conf[PREC_VAL]) << "�������: " << scale * dmin << "  ��������: " << scale * dmax
			<< setprecision(conf[PREC_AVG]) << "  �������: " << scale * avg
			<< "  ���: " << scale * sqdev << "  ���%: " << sqdev_perc << endl << endl;
		return *this;
	}
	Log& write_in_file(string name) {
		ofstream out;
		out.open(name);
		for (int i = 0; i < arr.size(); i++) {
			out << i+1 << "$" << arr[i] * scale << endl;
		}
		out.close();
		return *this;
	}
	Log& set_scale(double newscale) {
		scale = newscale;
		return *this;
	}
	Log& printk() {
        k = arr[arr.size() - 1] / arr.size();
		cout << "k = " << k * scale << endl;
		return *this;
	}
	// ������������� ����� � ��������� scale ������ len � ����� �� ���� ��������:
	// O_NORM - ������������, O_MIN - �����������, O_MAX - ������������
	Log& print(unsigned len) {
		vector <double> vect = arr;
		string head, suffix = "";
		if (conf[FILTR_MIN] + conf[FILTR_MAX] > 0)
			suffix = " �� ����������";
		int nsetw = int(ceil(log10(dmax * scale))) + conf[PREC_VAL] + 3;
		for (int i = 0; i < 3; i++) {
			switch (i) {
			case O_NATURAL:
				head = "������ ��������" + suffix + ": ";
				break;
			case O_MIN:
				sort(vect.begin(), vect.end());
				head = "����������" + suffix + ": ";
				break;
			case O_MAX:
				sort(vect.begin(), vect.end(), [](double a, double b) { return a > b; });
				head = "����������" + suffix + ": ";
				break;
			}
			cout.setf(ios::fixed);
			cout << head << setprecision(conf[PREC_VAL]) << endl;
			for (unsigned n = 0; n < min(len, unsigned(vect.size())); n++)
				cout << setw(nsetw) << scale * vect[n];
			cout << endl;
		}
		return *this;
	}
};
