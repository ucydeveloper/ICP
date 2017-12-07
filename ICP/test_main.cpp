#include "icp.h"
#include < stdlib.h >
#include < math.h >
#include < stdio.h >

double randomNumber(int hi)  //the correct random number generator for [0,hi]
{
	// scale in range [0,1)
	double scale;
	scale = (double)rand();
	scale /= (double)(RAND_MAX);
	// return range [0,hi]
	return (scale*hi); // implicit cast and truncation in return
}
void GenerateRandomPointSet(Point2D* pSet, int p_size)
{
	int i;
	for(i=0; i < p_size; i++){
		pSet[i].x = randomNumber(1000);
		pSet[i].y = randomNumber(1000);
	}
}

void MovePointSet(Point2D* pSet, int p_size, double dx, double dy)
{
	int i;
	for(i=0; i < p_size; i++){
		pSet[i].x += dx;
		pSet[i].y += dy;
	}
}

void RotatePointSet(Point2D* pSet, int p_size, double dth)
{
	int i;
	double tmp_x, tmp_y;
	for(i=0; i < p_size; i++){
		tmp_x = pSet[i].x * cos(dth) - pSet[i].y * sin(dth);
		tmp_y = pSet[i].x * sin(dth) + pSet[i].y * cos(dth);
		pSet[i].x = tmp_x;
		pSet[i].y = tmp_y;
	}
}

int main()
{
	Point2D tgtPset[1000];
	Point2D refPset[1000];
	double error;
	double dx, dy, dth;

	int i;

	GenerateRandomPointSet(tgtPset, 1000);

	for(i=0; i < 1000; i++){
		refPset[i] = tgtPset[i];
	}

	MovePointSet(tgtPset, 1000, 10, 12);
	RotatePointSet(tgtPset, 1000, 0.02);

	error = icp(&dx, &dy, &dth, tgtPset, 1000, refPset, 1000);

	printf("final result: err = %lf, dx = %lf, dy = %lf, dth = %lf\n", error, dx, dy, dth);
	system("pause");
	return 0;
}