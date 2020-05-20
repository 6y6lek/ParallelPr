
#include <iostream>
#include <ctime>
#include "omp.h"

using namespace std;


struct Point
{
	double x, y;
};

int main()
{
	const int NumberOfThreads = 4;
	int n;
	int i;
	long double drob=1, drobSum = 0;
	n = 50000;
	double angle = 1.57;
	double CosMy = 0, SinMy = 0;
	Point rectangle[4];
	Point p, p2, p3, p4;
	p.x = 0; p.y = 0; rectangle[0] = p;
	p2.x = 4; p2.y = 0; rectangle[1] = p2;
	p3.x = 4; p3.y = 2; rectangle[2] = p3;
	p4.x = 0; p4.y = 2; rectangle[3] = p4;
	omp_set_num_threads(NumberOfThreads);

	clock_t starttime, endtime;
	starttime = clock();
	int k; int j;
	for (k = 0; k <= n; k++)
	{
		drob = 1; 
		#pragma omp parallel for private(j) reduction(*:drob)
		for (j = 1; j <= 2 * k; j++)
		{
			drob *= angle / j;
		}
		if (k % 2 == 0)
			CosMy += drob;
		else CosMy -= drob;

	}
	int h;
	for (i = 0; i <= n; i++)
	{
		drob = 1;
		#pragma omp parallel for private(h) reduction(*:drob)
		for (h = 1; h <= 2 * i + 1; h++)
		{
			drob *= angle / h;
		}
		if (i % 2 == 0)
			SinMy += drob;
		else SinMy -= drob;
	}
	cout << "angle = " << angle << endl;
	cout.precision(10);

	cout << "Cos = " << CosMy << endl;
	cout << "Sin = " << SinMy << endl;
	Point rectangleNew[4];
	for (int i = 0; i < 4; i++)
	{
		Point pnew;
		pnew.x = CosMy * rectangle[i].x - SinMy * rectangle[i].y;
		pnew.y = SinMy * rectangle[i].x + CosMy * rectangle[i].y;
		rectangleNew[i] = pnew;
	}
	endtime = clock();
	cout << "Time: " << ((float)(endtime - starttime)) / CLOCKS_PER_SEC << endl;
	cout.precision(10);
	cout << "New coordinate:\n";
	for (int i = 0; i < 4; i++)
	{
		cout << rectangleNew[i].x << " " << rectangleNew[i].y << endl;
	}

	return 0;
}
