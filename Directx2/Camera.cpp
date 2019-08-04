#include "Camera.h"
#include "Game_info.h"

Vector2 Camera::pos;
Matrix Camera::viewMat;

void Camera::Update(const Vector2 _pos)
{
	pos = _pos;

	viewMat =
		Matrix::Translate(-pos.x, -pos.y, 0)*
		Matrix::Scale(2.0f / SCREEN_WIDTH, 2.0f / SCREEN_HEIGHT, 1);
}
