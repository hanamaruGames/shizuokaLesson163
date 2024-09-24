#include "Player.h"
#include "Stage.h"
Player::Player()
{
	mesh = new CFbxMesh();
	mesh->Load("data/models/char01.mesh");
	transform.position = VECTOR3(0, 1, 0);
}

Player::~Player()
{
}

void Player::Update()
{
	// �L�[����Ńv���C���[���ړ�����
	CDirectInput* di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_DAT, DIK_W)) {
		transform.position += VECTOR3(0, 0, 0.1f);
	}
	if (di->CheckKey(KD_DAT, DIK_S)) {
		transform.position += VECTOR3(0, 0, -0.1f);
	}
	if (di->CheckKey(KD_DAT, DIK_A)) {
		transform.position += VECTOR3(-0.1f, 0, 0);
	}
	if (di->CheckKey(KD_DAT, DIK_D)) {
		transform.position += VECTOR3(0.1f, 0, 0);
	}

	// �n�ʂ�������Ȃ���Η�����
	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (st->IsLandBlock(transform.position) == false) {
		transform.position += VECTOR3(0, -0.1f, 0);
	}

	// �v���C���[��ǂ�������J�����ɂ���
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position + VECTOR3(0, 3.0f, -5.0f), // �J�����ʒu
		transform.position + VECTOR3(0, 1.5f, 0), // �����_
		VECTOR3(0, 1.0f, 1.0f)); // ��x�N�g��
}

void Player::Draw()
{
	Object3D::Draw();
}
