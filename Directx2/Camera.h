#pragma once
#include "Math.h"

class Camera
{
public:

	static void Update(const Vector2);

	static Vector2 Pos(){return pos;}
	static Matrix ViewMat() {return viewMat;}

private:
	static Vector2 pos;
	static Matrix viewMat;
};

