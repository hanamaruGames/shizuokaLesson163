#include "Dancer.h"

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
	// 自分の視野に入ったら、プレイヤーに向かってくる

}

SphereCollider Dancer::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}
