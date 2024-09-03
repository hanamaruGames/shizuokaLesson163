#include "Camera.h"
#include "Tank.h"

static const VECTOR3 CamPos = VECTOR3(0, 5, -20);
static const VECTOR3 LookPos = VECTOR3(0, 2, 0);

Camera::Camera()
{
	SetDrawOrder(-1000);
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 5, -20), // �J�������W
		VECTOR3(0, 2, 0),  // �����_
		VECTOR3(0, 1, 0));  // ���\���x�N�g���i�Œ�j
}

Camera::~Camera()
{
}

void Camera::Update()
{
	Tank* tank = ObjectManager::FindGameObject<Tank>();
	if (tank != nullptr) {
		VECTOR3 tankPos = tank->Position(); // Tank�̍��W
		VECTOR3 tankRot = tank->Rotation(); // Tank�̉�]
		MATRIX4X4 tankMat = tank->Matrix(); // Tank�̍s��
//		MATRIX4X4 tankMat = XMMatrixRotationY(tankRot.y) *
//			XMMatrixTranslation(tankPos.x, tankPos.y, tankPos.z);

		transform.position = CamPos * tankMat;
		lookPosition = LookPos * tankMat;

		GameDevice()->m_mView = XMMatrixLookAtLH(
			transform.position, // �J�������W
			lookPosition,  // �����_
			VECTOR3(0, 1, 0));  // ���\���x�N�g���i�Œ�j
	}
}

void Camera::Draw()
{
}
