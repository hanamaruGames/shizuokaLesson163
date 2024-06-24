#include "Dancer.h"
#include "Player.h"

Dancer::Dancer()
{
	animator = new Animator(); // インスタンスを作成

	mesh = new CFbxMesh();
	mesh->Load("Data/Dancer/Character.mesh");
	mesh->LoadAnimation(0, "Data/Dancer/Dancing.anmx", true);
	animator->SetModel(mesh); // このモデルでアニメーションする
	animator->Play(0);
	animator->SetPlaySpeed(1.0f);

	transform.position = VECTOR3(20, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
}

Dancer::~Dancer()
{
}

void Dancer::Update()
{
	animator->Update();

	// 自分の視野に入ったら、プレイヤーに向かってくる
	Player* player = ObjectManager::FindGameObject<Player>();
	VECTOR3 toPlayer = player->Position() - transform.position;
	if (toPlayer.Length() < 10.0f) {
		// 自分の正面のベクトルを作る
		VECTOR3 front = VECTOR3(0, 0, 1)* XMMatrixRotationY(transform.rotation.y);
		// プレイヤーへのベクトルの長さ１のもの
		VECTOR3 toPlayer1 = XMVector3Normalize(toPlayer);
		// 内積を取る →これがcosθ
		float ip = Dot(front, toPlayer1);
		if (ip >= cosf(60.0f * DegToRad)) {
			// 視野に入った
			transform.position += front * 0.01f; // とりあえず
			VECTOR3 right = VECTOR3(1, 0, 0) * XMMatrixRotationY(transform.rotation.y);
			if (ip >= cosf(3.0f * DegToRad)) {
				transform.rotation.y = atan2f(toPlayer.x, toPlayer.z);
			} else if (Dot(right, toPlayer) > 0) {
				transform.rotation.y += 3.0f*DegToRad; // 右に曲げる
			}
			else {
				transform.rotation.y -= 3.0f*DegToRad; // 左に曲げる
			}
		}
	}
}

SphereCollider Dancer::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}
