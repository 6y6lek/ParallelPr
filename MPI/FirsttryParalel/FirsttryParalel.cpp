#include <iostream>
#include "mpi.h"
#include <time.h> 
#include <windows.h>
#include <cmath>
using namespace std;
int ProcNum;   
int ProcRank;


struct Point
{
	double x, y;
};

int main(int argc, char* argv[])
{
	
	SetConsoleOutputCP(1251);
	int n;
	int myid;
	int numprocs;
	int i;
	int rc;
	long double drob, drobSum = 0, Result, sum;
	double startwtime = 0.0;
	double endwtime;
	int x = 2;
	n = 50000;
	double angle=1.57;
	double CosMy, SinMy=0;
	Point rectangle[4];
	Point p,p2,p3,p4;
	p.x = 0; p.y = 0; rectangle[0] = p;
	p2.x = 4; p2.y = 0; rectangle[1] = p2;
	p3.x = 4; p3.y = 2; rectangle[2] = p3;
	p4.x = 0; p4.y = 2; rectangle[3] = p4;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	int j;
	if (myid == 0)
	{
		startwtime = MPI_Wtime();
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for (i = myid; i <= n; i += numprocs)
	{
		drob = 1;
		for (j = 1; j <= 2 * i; j++)
		{
			drob *= angle / j;
		}
		if (i % 2 == 0)
			drobSum += drob;
		else drobSum -= drob;
	}

	MPI_Reduce(&drobSum, &CosMy, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	drobSum = 0;

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	for (i = myid; i <= n; i += numprocs)
	{
		drob = 1;
		for (int j = 1; j <= 2 * i + 1; j++)
		{
			drob *= angle / j;
		}
		if (i % 2 == 0)
			drobSum += drob;
		else drobSum -= drob;
	}
	
	MPI_Reduce(&drobSum, &SinMy, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	
	if (myid == 0)
	{
		cout << "angle = "<< angle<<endl;
		cout.precision(10);
		cout <<"Cos = "<< CosMy << endl;
		cout <<"Sin = "<< SinMy << endl;
		Point rectangleNew[4];
		for (int i = 0; i < 4; i++)
		{
			Point pnew;
			pnew.x = CosMy * rectangle[i].x - SinMy * rectangle[i].y;
			pnew.y = SinMy * rectangle[i].x + CosMy * rectangle[i].y;
			rectangleNew[i] = pnew;
		}
		endwtime = MPI_Wtime();
		cout<<"Time:" << (endwtime - startwtime)<< endl;
		cout << "New coordinate:\n";
		cout.precision(10);
		for (int i = 0; i < 4; i++)
		{
			cout << rectangleNew[i].x << " " << rectangleNew[i].y << endl;
		}
	}
	MPI_Finalize();
	return 0;
}
