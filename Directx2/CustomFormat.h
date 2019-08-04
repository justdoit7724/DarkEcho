#pragma once
#include "Math.h"

struct Vertex {
	Vector2 pos;
	Vector2 tex;

	Vertex(Vector2 _pos, Vector2 _tex) {
		pos = _pos;
		tex = _tex;
	}
};

struct Transformation {
	Matrix worldMat;
	Matrix VPMat;

	Transformation(Matrix w, Matrix vp) {
		worldMat = w;
		VPMat = vp;
	}
};