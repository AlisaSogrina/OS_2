#include <iostream>
#include <thread>
#include <string>
#include "windows.h"

using namespace std;

struct Parameter {
	int size;
	double* coefficients;
	double x;
	double result = 0;
};

DWORD WINAPI ThreadProc(LPVOID lpParameters) {
	Parameter* parameter = (Parameter*)lpParameters;

	double result = 0;
	for (int i = 0; i < parameter->size; i++) {
		double addend = parameter->coefficients[i];
		for (int j = 0; j < parameter->size - i; j++) {
			addend *= parameter->x;
		}
		result += addend;
		Sleep(15);
	}

	parameter->result = result;
	cout << "p(" << parameter->x << ") = " << result << endl;
	return 0;
}

HANDLE startPolynomThread(Parameter& parameter) {
	DWORD lpThreadId;
	HANDLE handle;

	handle = CreateThread(NULL, 0, ThreadProc, (void*)&parameter, 0, &lpThreadId);
	if (!handle) {
		cout << "Thread wasn't created" << endl;
	}

	return handle;
}

int main() {
	cout << "Please enter up polynom power" << endl;
	int n;
	cin >> n;

	cout << "\n" << "Please enter up polynom coefficients" << endl;
	double* upCoefficients = new double[n];
	for (int i = 0; i < n; i++) {
		cin >> upCoefficients[i];
	}

	cout << "\n" << "Please enter down polynom power" << endl;
	int m;
	cin >> m;

	cout << "\n" << "Please enter down polynom coefficients" << endl;
	double* downCoefficients = new double[m];
	for (int i = 0; i < m; i++) {
		cin >> downCoefficients[i];
	}

	cout << endl << "Please enter x" << endl;
	double x;
	cin >> x;

	Parameter parameter1;
	parameter1.x = x;
	parameter1.size = n;
	parameter1.coefficients = upCoefficients;

	Parameter parameter2;
	parameter2.x = x;
	parameter2.size = m;
	parameter2.coefficients = downCoefficients;

	HANDLE* handles = new HANDLE[2];
	handles[0] = startPolynomThread(parameter1);
	handles[1] = startPolynomThread(parameter2);

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);

	CloseHandle(handles[0]);
	CloseHandle(handles[1]);

	cout << "f(" << x << ") = " << parameter1.result / parameter2.result;

	return 0;
}