#ifndef _ICP_H
#define _ICP_H

typedef struct Point2D
{
	double x;
	double y;
} Point2D;

double p2p_length(Point2D p, Point2D q);

Point2D find_closest_point(Point2D target, Point2D* refPset, int size);

void icp_step(double* dx, double* dy, double* dth, Point2D* tgtPset, int t_size, Point2D* refPset, int r_size);

double icp(double* dx, double* dy, double* dth, Point2D* tgtPset, int t_size, Point2D* refPset, int r_size);

#endif