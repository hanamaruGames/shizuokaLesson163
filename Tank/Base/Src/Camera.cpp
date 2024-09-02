#include "Camera.h"

Camera::Camera()
{
	SetDrawOrder(-1000);
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 5, -20),
		VECTOR3(0, 2, 0),
		VECTOR3(0, 1, 0));
}

Camera::~Camera()
{
}

void Camera::Update()
{
}

void Camera::Draw()
{
}
