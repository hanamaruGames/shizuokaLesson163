#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Dancer.h"
#include "Door.h"

namespace { // ����cpp�ȊO�ł͎g���Ȃ�
	static const float Gravity = 0.025f; // �d�͉����x(���̒l)
	// C++�̒萔��`�i�^���t���j
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // ��]���x(�x)
	static const float MoveSpeed = 0.1f;
};

Player::Player()
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();
	mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1, "Data/Player/attack1.anmx", false);
	mesh->LoadAnimation(aAttack2, "Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3, "Data/Player/attack3.anmx", false);
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
	std::list<Object3D*> doors = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ"); // �h�A�̃I�u�W�F�N�g��������
#if 0
	int size = doors.size(); // �v�f������ɓ���
//	for (std::list<Door*>::iterator itr = doors.begin(); itr != doors.end(); itr++) {
	for (auto itr = doors.begin(); itr != doors.end(); itr++) {
		Door* door = *itr;
	}
#endif
#if 1
	for (auto door : doors) {
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0, 1.0f, 0); // �����̋������
		coll.radius = 0.5;
		VECTOR3 push;
		if (door->HitSphereToMesh(coll, &push)) {
			transform.position += push;
		}
	}
#endif
}

void Player::Draw()
{
	Object3D::Draw(); // �p�����̊֐����Ă�

//	MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
//	VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix(); // ���E�i���[���h�j�̒��ŁA�v���C���[�̈ʒu�ƌ���
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // �v���C���[�̌��_����̎��̈ʒu(34�͎��)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	CSprite spr;
	spr.DrawLine3D(start, end, RGB(255, 0, 0));
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
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_N)) { // �U���{�^��
		animator->MergePlay(aAttack1);
		state = sAttack;
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
	// �G�ɍU���𓖂Ă�
	std::list<Dancer*> dancers = ObjectManager::FindGameObjects<Dancer>();

	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix(); // ���E�i���[���h�j�̒��ŁA�v���C���[�̈ʒu�ƌ���
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // �v���C���[�̌��_����̎��̈ʒu(34�͎��)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	for (Dancer* d : dancers) {
		VECTOR3 hit;
		if (d->HitLineToMesh(start, end, &hit)) {
			d->AddDamage(10, transform.position); // �G�ɓ��Ă�
		}
	}
	if (animator->Finished()) {
		state = sOnGround;
	}
}
