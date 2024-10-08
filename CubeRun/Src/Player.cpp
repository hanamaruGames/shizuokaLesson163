#include "Player.h"
#include "Stage.h"
Player::Player()
{
	mesh = new CFbxMesh();
	mesh->Load("data/models/char01.mesh");
	transform.position = VECTOR3(0, 1, 0);

	jumping = false;
	velocityY = 0.0f;
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

	if (!jumping) {
		if (di->CheckKey(KD_TRG, DIK_SPACE)) {
			jumping = true;
			velocityY = 0.1f; // ��ւ̈ړ���
		}
	}
	transform.position.y += velocityY;
	velocityY -= 0.005f; // �d�͉����x
	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (st->IsLandBlock(transform.position)) {
		jumping = false;
		velocityY = 0.0f;
	}

	SphereCollider collider;
	collider.center = transform.position + VECTOR3(0, 0.5f, 0);
	collider.radius = 0.5f;
	VECTOR3 push;
	if (st->HitSphere(collider, &push)) {
		transform.position += push;
	}

	// �v���C���[��ǂ�������J�����ɂ���
	VECTOR3 p = transform.position;
	p.y = 1.0f;//�n�ʂ̍���
	GameDevice()->m_mView = XMMatrixLookAtLH(
		p + VECTOR3(0, 3.0f, -5.0f), // �J�����ʒu
		p + VECTOR3(0, 1.5f, 0), // �����_
		VECTOR3(0, 1.0f, 1.0f)); // ��x�N�g��
}

void Player::Draw()
{
	Object3D::Draw();
}
