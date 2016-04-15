/*
 * vect.h
 *
 *  Created on: Oct 16, 2015
 *      Author: jan
 */

#ifndef VECT_H_
#define VECT_H_

struct vect {
	double x, y, z;
	vect operator+(vect a) {
		vect res;
		res.x = x + a.x;
		res.y = y + a.y;
		res.z = z + a.z;
		return res;
	}
	vect operator+=(vect a) {
		vect res;
		res.x = x += a.x;
		res.y = y += a.y;
		res.z = z += a.z;
		return res;
	}
	vect operator-(vect a) {
		vect res;
		res.x = x - a.x;
		res.y = y - a.y;
		res.z = z - a.z;
		return res;
	}
	vect operator-=(vect a) {
		vect res;
		res.x = x -= a.x;
		res.y = y -= a.y;
		res.z = z -= a.z;
		return res;
	}
	vect operator*(double s) {
		vect res;
		res.x = x * s;
		res.y = y * s;
		res.z = z * s;
		return res;
	}
	vect operator*=(double s) {
		vect res;
		res.x = x *= s;
		res.y = y *= s;
		res.z = z *= s;
		return res;
	}
	vect operator/(double s) {
		vect res;
		res.x = x / s;
		res.y = y / s;
		res.z = z / s;
		return res;
	}
	vect operator*(vect a) {
		vect res;
		res.x = y * a.z - z * a.y;
		res.y = z * a.x - x * a.z;
		res.z = x * a.y - y * a.x;
		return res;
	}
};

#endif /* VECT_H_ */
