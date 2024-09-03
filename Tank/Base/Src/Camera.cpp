#include "Camera.h"
#include "Tank.h"

static const VECTOR3 CamPos = VECTOR3(0, 5, -20);
static const VECTOR3 LookPos = VECTOR3(0, 2, 0);

Camera::Camera()
{
	SetDrawOrder(-1000);
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 5, -20), // カメラ座標
		VECTOR3(0, 2, 0),  // 注視点
		VECTOR3(0, 1, 0));  // 上を表すベクトル（固定）
}

Camera::~Camera()
{
}

void Camera::Update()
{
	Tank* tank = ObjectManager::FindGameObject<Tank>();
	if (tank != nullptr) {
		VECTOR3 tankPos = tank->Position(); // Tankの座標
		VECTOR3 tankRot = tank->Rotation(); // Tankの回転
		MATRIX4X4 tankMat = tank->Matrix(); // Tankの行列
//		MATRIX4X4 tankMat = XMMatrixRotationY(tankRot.y) *
//			XMMatrixTranslation(tankPos.x, tankPos.y, tankPos.z);

		transform.position = CamPos * tankMat;
		lookPosition = LookPos * tankMat;

		GameDevice()->m_mView = XMMatrixLookAtLH(
			transform.position, // カメラ座標
			lookPosition,  // 注視点
			VECTOR3(0, 1, 0));  // 上を表すベクトル（固定）
	}
}

void Camera::Draw()
{
}
