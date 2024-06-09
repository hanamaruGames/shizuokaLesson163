#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Dancer.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 0.025f; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.1f;
	static const VECTOR3 CameraPos = VECTOR3(0, 2, -5);
	static const VECTOR3 LookPos = VECTOR3(0, 1, 5);
};

Player::Player()
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();
	mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(aRun);
	animator->SetPlaySpeed(1.0f);

	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	state = sOnGround;
	speedY = 0;
}

Player::~Player()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
	if (animator != nullptr) {
		delete animator;
		animator = nullptr;
	}
}

void Player::Update()
{
	animator->Update(); // ���t���[���AUpdate���Ă�

	switch (state) {
	case sOnGround:
		UpdateOnGround();
		break;
	case sJump:
		UpdateJump();
		break;
	case sAttack:
		UpdateAttack();
		break;
	}
	ImGui::Begin("Sample");
	ImGui::InputInt("State", (int*)(&state));
	ImGui::InputFloat("SP", &speedY);
	ImGui::End();

	{
		// �v���C���[�̍s������߂�
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y);
		MATRIX4X4 trans = XMMatrixTranslation(
			transform.position.x, 0.0f, transform.position.z);
		MATRIX4X4 m = rotY * trans;
		// �v���C���[����]�E�ړ����ĂȂ����̃J�����ʒu��
		// �v���C���[�̉�]�E�ړ��s����|����ƁA
		VECTOR3 cameraPos = CameraPos * m;
		VECTOR3 lookPos = LookPos * m;
		GameDevice()->m_mView = XMMatrixLookAtLH(
			cameraPos,
			lookPos,
			VECTOR3(0, 1, 0));
	}

	// Dancer�ɂ߂荞�܂Ȃ��悤�ɂ���
	// �����̍��W�́Atransform.position
	// Dancer�̍��W��m��
	std::list<Dancer*> dancers = 
			ObjectManager::FindGameObjects<Dancer>();
	// �@�C�e���[�^�[��
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �A�C�e���[�^�̕ύX
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �Bfor(:)�ŉ�
	for (Dancer* dancer : dancers) {
	// �ȏ�R�o�[�W����
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // ���̓����蔻��
			// �������Ă�
			// �����o��������pushVec
			// �����o�����������߂�
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVec�̒������P�ɂ���
			transform.position += pushVec * pushLen;
		}
	}
}

SphereCollider Player::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}

void Player::UpdateOnGround()
{
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_W)) {
		// �O�p�֐��ł��ꍇ
//		position.z += cosf(rotation.y) * 0.1;
//		position.x += sinf(rotation.y) * 0.1;
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position += move;
		animator->MergePlay(aRun);
	} else if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_S)) {
		// �O�p�֐��ł��ꍇ
//		position.z -= cosf(rotation.y) * 0.1;
//		position.x -= sinf(rotation.y) * 0.1;
		// �s��ł��ꍇ
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // ��]���ĂȂ����̈ړ���
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Y�̉�]�s��
		VECTOR3 move = forward * rotY; // �L�����̌����Ă���ւ̈ړ���
		transform.position -= move;
		animator->MergePlay(aRun);
	}
	else {
		animator->MergePlay(aIdle);
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_A)) {
		transform.rotation.y -= RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_D)) {
		transform.rotation.y += RotationSpeed / 180.0f * XM_PI;
	}
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE)) {
		speedY = JumpPower;
		state = sJump;
	}
}

void Player::UpdateJump()
{
	transform.position.y += speedY;
	speedY -= Gravity;
	if (transform.position.y <= 0) {
		// �W�����v�I��
		transform.position.y = 0;
		state = sOnGround;
	}
}

void Player::UpdateAttack()
{
}
