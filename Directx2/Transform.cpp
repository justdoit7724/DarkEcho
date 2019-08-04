#include "Transform.h"


Matrix Transform::WorldMatrix()
{
	return
		Matrix::Scale(scale.x, scale.y, 1.0f)*
		Matrix::RotationZ(Vector2::DirToRad(FORWARD, dir))*
		Matrix::Translate(pos.x, pos.y, 0);
}