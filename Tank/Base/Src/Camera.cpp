#include "Camera.h"
#include "Tank.h"

static VECTOR3 CamPos = VECTOR3(0, 10, -20);
static VECTOR3 LookPos = VECTOR3(0, 4, 0);
//上から見たカメラ
//static VECTOR3 CamPos = VECTOR3(0, 100, -20);
//static VECTOR3 LookPos = VECTOR3(0, 4, 20);

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
	TankTower* tank = ObjectManager::FindGameObject<TankTower>();
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
		GameDevice()->m_vEyePt = transform.position; // カメラの座標
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
