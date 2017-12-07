#include "icp.h"
#include < math.h >
#include < stdlib.h >
#include < stdio.h >

double p2p_length(Point2D p, Point2D q)
{
	return sqrt(pow(p.x - q.x, 2)+pow(p.y - q.y, 2));
}

Point2D find_closest_point(Point2D target, Point2D* refPset, int size)
{
	Point2D ret;
	double min_distance;
	int i;

	ret = refPset[0];
	min_distance = p2p_length(target, refPset[0]);
	for(i=1; i < size; i++){
		if(p2p_length(target, refPset[i]) < min_distance){
			ret = refPset[i];
			min_distance = p2p_length(target, refPset[i]);
		}
	}
	return ret;
}

void icp_step(double* dx, double* dy, double* dth, Point2D* tgtPset, int t_size, Point2D* refPset, int r_size)
{
	double sum_bx_times_ay;
	double sum_by_times_ax;
	double sum_bx_times_ax;
	double sum_by_times_ay;
	double mean_bx;
	double mean_by;
	double mean_ax;
	double mean_ay;

	Point2D* a = (Point2D*)malloc(t_size*sizeof(Point2D));
	// accroding to paper, a is the closest point set
	// according to paper, b is the target point set (tgtPset in this function)

	int i;
	
	// find closest point set
	for(i=0; i < t_size; i++){
		a[i] = find_closest_point(tgtPset[i], refPset, r_size);
	}
	
	// initialize dx, dy, dth
	*dx = 0;
	*dy = 0;
	*dth = 0;
	mean_bx = 0;
	mean_by = 0;
	mean_ax = 0;
	mean_ay = 0;

	for(i=0; i < t_size; i++){
			mean_bx += tgtPset[i].x;
			mean_by += tgtPset[i].y;
			mean_ax += a[i].x;
			mean_ay += a[i].y;
	}
	
	mean_bx = mean_bx / t_size;
	mean_by = mean_by / t_size;
	mean_ax = mean_ax / t_size;
	mean_ay = mean_ay / t_size;

	sum_bx_times_ay = 0;
	sum_by_times_ax = 0;
	sum_bx_times_ax = 0;
	sum_by_times_ay = 0;

	for(i=0; i < t_size; i++){
		sum_bx_times_ay += (tgtPset[i].x - mean_bx)*(a[i].y - mean_ay);
		sum_by_times_ax += (tgtPset[i].y - mean_by)*(a[i].x - mean_ax);
		sum_bx_times_ax += (tgtPset[i].x - mean_bx)*(a[i].x - mean_ax);
		sum_by_times_ay += (tgtPset[i].y - mean_by)*(a[i].y - mean_ay);
	}

	*dth = atan2(sum_bx_times_ay - sum_by_times_ax, sum_bx_times_ax + sum_by_times_ay);
	*dx = mean_ax - ((mean_bx * cos(*dth)) - (mean_by * sin(*dth)));
	*dy = mean_ay - ((mean_bx * sin(*dth)) + (mean_by * cos(*dth)));

}

double icp(double* dx, double* dy, double* dth, Point2D* tgtPset, int t_size, Point2D* refPset, int r_size)
{
	double error = 0;
	double pre_err = 0;
	double step_dx, step_dy, step_dth;
	double tmp_x, tmp_y;
	Point2D* step_tgtPset = (Point2D*)malloc(t_size*sizeof(Point2D));

	int i, iter_count;

	for(i=0; i < t_size; i++){
		step_tgtPset[i] = tgtPset[i];
	}
	
	*dx = 0;
	*dy = 0;
	*dth = 0;
	iter_count = 0;
	do{
		iter_count++;

		icp_step(&step_dx, &step_dy, &step_dth, step_tgtPset, t_size, refPset, r_size);
		
		pre_err = error;
		error = 0;
		for(i=0; i < t_size; i++){
			tmp_x = (step_tgtPset[i].x * cos(step_dth)) - (step_tgtPset[i].y * sin(step_dth)) + (step_dx);
			tmp_y = (step_tgtPset[i].x * sin(step_dth)) + (step_tgtPset[i].y * cos(step_dth)) + (step_dy);
			step_tgtPset[i].x = tmp_x;
			step_tgtPset[i].y = tmp_y;

			error += (pow(step_tgtPset[i].x - refPset[i].x, 2) + pow(step_tgtPset[i].y - refPset[i].y, 2));
		}
		error /= t_size;

		*dx += step_dx;
		*dy += step_dy;
		*dth += step_dth;

		// just for debug
		printf("iter[%d]: err = %lf, dx = %lf, dy = %lf, dth = %lf\n", iter_count, error, *dx, *dy, *dth);
		system("pause");
		//
	} while(fabs(error - pre_err) > 0.00001);

	return error;
}