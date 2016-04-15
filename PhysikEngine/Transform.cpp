/*
 * Transform.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: jan
 */

#include "Transform.h"

Transform::Transform(vect n, vect o, vect a, vect p) {
	nx = n.x;
	ny = n.y;
	nz = n.z;

	ox = o.x;
	oy = o.y;
	oz = o.z;

	ax = a.x;
	ay = a.y;
	az = a.z;

	px = p.x;
	py = p.y;
	pz = p.z;
}

Transform::Transform(vect rpy, vect p) {
	// orientation: transform rpy values into homogeneous transformation matrix
	nx = cos(rpy.z) * cos(rpy.y);
	ny = sin(rpy.z) * cos(rpy.y);
	nz = -sin(rpy.y);

	ox = cos(rpy.z) * sin(rpy.y) * sin(rpy.x) - sin(rpy.z) * cos(rpy.x);
	oy = sin(rpy.z) * sin(rpy.y) * sin(rpy.x) + cos(rpy.z) * cos(rpy.x);
	oz = cos(rpy.y) * sin(rpy.x);

	ax = cos(rpy.z) * sin(rpy.y) * cos(rpy.x) + sin(rpy.z) * sin(rpy.x);
	ay = sin(rpy.z) * sin(rpy.y) * cos(rpy.x) - cos(rpy.z) * sin(rpy.x);
	az = cos(rpy.y) * cos(rpy.x);

	// position
	px = p.x;
	py = p.y;
	pz = p.z;
}

vect Transform::transform_Vect(vect v) {
	vect res;
	res.x = v.x * nx + v.y * ox + v.z * ax;
	res.y = v.x * ny + v.y * oy + v.z * ay;
	res.z = v.x * nz + v.y * oz + v.z * az;
	return res;
}

vect Transform::transformInv_Vect(vect v) {
	vect res;
	res.x = v.x * nx + v.y * ny + v.z * nz;
	res.y = v.x * ox + v.y * oy + v.z * oz;
	res.z = v.x * ax + v.y * ay + v.z * az;
	return res;
}

vect Transform::toRPY(void) {
	vect rpy;
	rpy.z = atan2(ny, nx);
	rpy.y = atan2(-nz, nx * cos(rpy.z) + ny * sin(rpy.z));
	rpy.x = atan2(ax * sin(rpy.z) - ay * cos(rpy.z),
			oy * cos(rpy.z) - ox * sin(rpy.z));
	return rpy;
}

void Transform::move(vect pd) {
	px += pd.x;
	py += pd.y;
	pz += pd.z;
}

void Transform::rotateRPY(vect angvel) {
	// small angle approximation
	rotate(YAW_AXIS, angvel.z);
	rotate(PITCH_AXIS, angvel.y);
	rotate(ROLL_AXIS, angvel.x);
}

void Transform::rotate(int axis, double theta) {
	double cTheta = cos(theta);
	double sTheta = sin(theta);
	double bnx = nx;
	double bny = ny;
	double bnz = nz;
	double box = ox;
	double boy = oy;
	double boz = oz;
	double bax = ax;
	double bay = ay;
	double baz = az;
	switch (axis) {
	case ROLL_AXIS:
		ox = box * cTheta + bax * sTheta;
		ax = box * -sTheta + bax * cTheta;
		oy = boy * cTheta + bay * sTheta;
		ay = boy * -sTheta + bay * cTheta;
		oz = boz * cTheta + baz * sTheta;
		az = boz * -sTheta + baz * cTheta;
		break;
	case PITCH_AXIS:
		nx = bnx * cTheta + bax * -sTheta;
		ax = bnx * sTheta + bax * cTheta;
		ny = bny * cTheta + bay * -sTheta;
		ay = bny * sTheta + bay * cTheta;
		nz = bnz * cTheta + baz * -sTheta;
		az = bnz * sTheta + baz * cTheta;
		break;
	case YAW_AXIS:
		nx = bnx * cTheta + box * sTheta;
		ox = bnx * -sTheta + box * cTheta;
		ny = bny * cTheta + boy * sTheta;
		oy = bny * -sTheta + boy * cTheta;
		nz = bnz * cTheta + boz * sTheta;
		oz = bnz * -sTheta + boz * cTheta;
		break;
	}
}

