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
	// キー操作でプレイヤーが移動する
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

	// 地面が見つからなければ落ちる
	Stage* st = ObjectManager::FindGameObject<Stage>();
	if (st->IsLandBlock(transform.position) == false) {
		transform.position += VECTOR3(0, -0.1f, 0);
	}

	// プレイヤーを追いかけるカメラにする
	GameDevice()->m_mView = XMMatrixLookAtLH(
		transform.position + VECTOR3(0, 3.0f, -5.0f), // カメラ位置
		transform.position + VECTOR3(0, 1.5f, 0), // 注視点
		VECTOR3(0, 1.0f, 1.0f)); // 上ベクトル
}

void Player::Draw()
{
	Object3D::Draw();
}
