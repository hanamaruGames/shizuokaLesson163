#include "Player.h"
#include "../Libs/Imgui/imgui.h"
#include "Dancer.h"
#include "Door.h"

namespace { // このcpp以外では使えない
	static const float Gravity = 0.025f; // 重力加速度(正の値)
	// C++の定数定義（型が付く）
	static const float JumpPower = 0.3f;
	static const float RotationSpeed = 3.0f; // 回転速度(度)
	static const float MoveSpeed = 0.1f;
};

Player::Player()
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();
	mesh->Load("Data/Player/PlayerChara.mesh");
	mesh->LoadAnimation(aIdle, "Data/Player/Idle.anmx", true);
	mesh->LoadAnimation(aRun, "Data/Player/Run.anmx", true);
	mesh->LoadAnimation(aAttack1, "Data/Player/attack1.anmx", false);
	mesh->LoadAnimation(aAttack2, "Data/Player/attack2.anmx", false);
	mesh->LoadAnimation(aAttack3, "Data/Player/attack3.anmx", false);
	animator->SetModel(mesh); // このモデルでアニメーションする
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
	animator->Update(); // 毎フレーム、Updateを呼ぶ

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

	// Dancerにめり込まないようにする
	// 自分の座標は、transform.position
	// Dancerの座標を知る
	std::list<Dancer*> dancers = 
			ObjectManager::FindGameObjects<Dancer>();
	// �@イテレーター版
	//for (std::list<Dancer*>::iterator itr = dancers.begin();
	//				itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �Aイテレータの変更
	//for (auto itr = dancers.begin(); itr != dancers.end(); itr++) {
	//	Dancer* dancer = *itr;
	// �Bfor(:)で回す
	for (Dancer* dancer : dancers) {
	// 以上３バージョン
		SphereCollider tCol = dancer->Collider();
		SphereCollider pCol = Collider();
		VECTOR3 pushVec = pCol.center - tCol.center;
		float rSum = pCol.radius + tCol.radius;
		if (pushVec.LengthSquare() < rSum * rSum) { // 球の当たり判定
			// 当たってる
			// 押し出す方向はpushVec
			// 押し出す長さを求める
			float pushLen = rSum - pushVec.Length();
			pushVec = XMVector3Normalize(pushVec); // pushVecの長さを１にする
			transform.position += pushVec * pushLen;
		}
	}
	std::list<Object3D*> doors = ObjectManager::FindGameObjectsWithTag<Object3D>("STAGEOBJ"); // ドアのオブジェクトを見つける
#if 1
	for (auto door : doors) {
		SphereCollider coll;
		coll.center = transform.position + VECTOR3(0, 1.0f, 0); // 自分の球を作る
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
	Object3D::Draw(); // 継承元の関数を呼ぶ

//	MATRIX4X4 tip = XMMatrixRotationRollPitchYawFromVector(VECTOR3(-33, 82, 0) * DegToRad);
//	VECTOR3 tipPos = VECTOR3(0, 0, 1.2f) * tip;
	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix(); // 世界（ワールド）の中で、プレイヤーの位置と向き
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // プレイヤーの原点からの手首の位置(34は手首)
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
		// 三角関数でやる場合
//		position.z += cosf(rotation.y) * 0.1;
//		position.x += sinf(rotation.y) * 0.1;
		// 行列でやる場合
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		VECTOR3 move = forward * rotY; // キャラの向いてる方への移動量
		transform.position += move;
		animator->MergePlay(aRun);
	} else if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_S)) {
		// 三角関数でやる場合
//		position.z -= cosf(rotation.y) * 0.1;
//		position.x -= sinf(rotation.y) * 0.1;
		// 行列でやる場合
		VECTOR3 forward = VECTOR3(0, 0, MoveSpeed); // 回転してない時の移動量
		MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y); // Yの回転行列
		VECTOR3 move = forward * rotY; // キャラの向いてる方への移動量
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
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_N)) { // 攻撃ボタン
		animator->MergePlay(aAttack1);
		state = sAttack;
	}
}

void Player::UpdateJump()
{
	transform.position.y += speedY;
	speedY -= Gravity;
	if (transform.position.y <= 0) {
		// ジャンプ終了
		transform.position.y = 0;
		state = sOnGround;
	}
}

void Player::UpdateAttack()
{
	// 敵に攻撃を当てる
	std::list<Dancer*> dancers = ObjectManager::FindGameObjects<Dancer>();

	VECTOR3 tipPos = VECTOR3(0.9966, 0.6536, 0.140);
	MATRIX4X4 mat = transform.matrix(); // 世界（ワールド）の中で、プレイヤーの位置と向き
	MATRIX4X4 bone = mesh->GetFrameMatrices(34); // プレイヤーの原点からの手首の位置(34は手首)
	VECTOR3 start = VECTOR3(0, 0, 0) * bone * mat;
	VECTOR3 end = tipPos * bone * mat;

	for (Dancer* d : dancers) {
		VECTOR3 hit;
		if (d->HitLineToMesh(start, end, &hit)) {
			d->AddDamage(10, transform.position); // 敵に当てた
		}
	}
	if (animator->Finished()) {
		state = sOnGround;
	}
}
