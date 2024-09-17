#include "Camera.h"
#include "Tank.h"

static VECTOR3 CamPos = VECTOR3(0, 10, -20);
static VECTOR3 LookPos = VECTOR3(0, 4, 0);
//�ォ�猩���J����
//static VECTOR3 CamPos = VECTOR3(0, 100, -20);
//static VECTOR3 LookPos = VECTOR3(0, 4, 20);

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
	TankTower* tank = ObjectManager::FindGameObject<TankTower>();
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
		GameDevice()->m_vEyePt = transform.position; // �J�����̍��W
	}
}

void Camera::Draw()
{
	ImGui::Begin("CamPos");
	ImGui::InputFloat("X", &CamPos.x);
	ImGui::InputFloat("Y", &CamPos.y);
	ImGui::InputFloat("Z", &CamPos.z);
	ImGui::End();
	ImGui::Begin("LookPos");
	ImGui::InputFloat("X", &LookPos.x);
	ImGui::InputFloat("Y", &LookPos.y);
	ImGui::InputFloat("Z", &LookPos.z);
	ImGui::End();
}
