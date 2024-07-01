#include "Camera.h"
#include "Player.h"
//                                      ������_�@�@�@�@�@�@�^�㎋�_
static const VECTOR3 CameraPos[] = { VECTOR3(0, 2, -5), VECTOR3(0, 10, -0.5) };
static const VECTOR3 LookPos[] =   { VECTOR3(0, 1,  5), VECTOR3(0,  1,  1  ) };
static const float CHANGE_TIME_LIMIT = 0.5f; // �b

Camera::Camera()
{
	SetDrawOrder(-1000);
	viewType = 0;
	changeTime = CHANGE_TIME_LIMIT; // �؂�ւ��Ȃ�
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// �Q�̎��_��'L'�L�[�ɂ���Đ؂�ւ���
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_L)) {
		changePosStart = CameraPos[viewType];
		changeLookStart = LookPos[viewType];
		viewType += 1;
		if (viewType >= sizeof(CameraPos) / sizeof(CameraPos[0])) {
			viewType = 0;
		}
		changePosGoal = CameraPos[viewType];
		changeLookGoal = LookPos[viewType];
		changeTime = 0.0f;
	}

	// �v���C���[�̍s������߂�
	Player* player = ObjectManager::FindGameObject<Player>();
	MATRIX4X4 rotY = XMMatrixRotationY(player->Rotation().y);
	MATRIX4X4 trans = XMMatrixTranslation(
		player->Position().x, 0.0f, player->Position().z);
	MATRIX4X4 m = rotY * trans;
	// �v���C���[����]�E�ړ����ĂȂ����̃J�����ʒu��
	// �v���C���[�̉�]�E�ړ��s����|����ƁA
	if (changeTime >= CHANGE_TIME_LIMIT) {
		transform.position = CameraPos[viewType] * m;
		lookPosition = LookPos[viewType] * m;
	}
	else { // ���_�؂�ւ���
		changeTime += 1.0f / 60.0f;
		float timeRate = changeTime / CHANGE_TIME_LIMIT; // 0.0�`1.0
		float rate = timeRate;
		VECTOR3 position = (changePosGoal - changePosStart) * rate + changePosStart;
		VECTOR3 look = (changeLookGoal - changeLookStart) * rate + changeLookStart;
		transform.position = position * m;
		lookPosition = look * m;
	}
	// �J�������ǂɂ߂荞�܂Ȃ��悤�ɂ���
	VECTOR3 start = player->Position() + VECTOR3(0, 1.5f, 0);
	VECTOR3 end = transform.position;
	// start����end�Ɍ������x�N�g�������A������0.2��������
	VECTOR3 camVec = end - start;
	camVec = XMVector3Normalize(camVec) * (camVec.Length() + 0.2f);
	end = start + camVec;

	std::list<Object3D*> grounds = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ");
	for (Object3D* g : grounds)
	{
		VECTOR3 hit;
		if (g->HitLineToMesh(start, end, &hit)) {
			end = hit;
		}
	}
	//end����0.2��O�ɒu��;
	transform.position = XMVector3Normalize(camVec) * ((end - start).Length() - 0.2f)
																				+ start;
}

void Camera::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position, // �J�������W
		lookPosition, // �����_
		VECTOR3(0, 1, 0));
}
