#pragma once
#include "Math.h"

#define FORWARD Vector2(1,0)

class Transform
{
public:
	Matrix WorldMatrix();

	Vector2 pos = Vector2(0.0f,0.0f);
	Vector2 dir = FORWARD;
	Vector2 scale = Vector2(1.0f, 1.0f);
};

