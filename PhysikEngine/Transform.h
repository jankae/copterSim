/*
 * Transform.h
 *
 *  Created on: Oct 16, 2015
 *      Author: jan
 */

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "../common/vect.h"
#include "cmath"
#include "../common/defines.h"

class Transform {
public:
	double nx, ny, nz;
	double ox, oy, oz;
	double ax, ay, az;
	double px, py, pz;

	Transform(vect n, vect o, vect a, vect p);
	Transform(vect rpy, vect p);
	vect transform_Vect(vect v);
	vect transformInv_Vect(vect v);
	vect toRPY(void);
	void move(vect pd);
	void rotateRPY(vect angvel);
	void rotate(int axis, double theta);
};

#endif /* TRANSFORM_H_ */
