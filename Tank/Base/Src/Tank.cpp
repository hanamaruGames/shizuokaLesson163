#include "Tank.h"

Tank::Tank()
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/tank_001.mesh");
	new TankTower(this);
}

Tank::~Tank()
{
	if (mesh != nullptr) {
		delete mesh;
	}
}

void Tank::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_DAT, DIK_D)) {
		transform.rotation.y += 5.0f * DegToRad;
	}
	if (di->CheckKey(KD_DAT, DIK_A)) {
		transform.rotation.y -= 5.0f * DegToRad;
	}
	if (di->CheckKey(KD_DAT, DIK_W)) {
		                      //‰ñ‚Á‚Ä‚È‚¢Žž‚ÌˆÚ“®—Ê * ‰ñ“]s—ñ
		transform.position += VECTOR3(0, 0, 0.1f) *	XMMatrixRotationY(transform.rotation.y);
	}
}

TankTower::TankTower(Object3D* parentModel)
{
	parent = parentModel;
	transform.SetParent(parent); // transform‚É‚àe‚ð‹³‚¦‚é

	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/tank_tower_001.mesh");
	transform.position = VECTOR3(0, 2, 0);
}

TankTower::~TankTower()
{
}

void TankTower::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_DAT, DIK_RIGHT)) {
		transform.rotation.y += 3.0f * DegToRad;
	}
	if (di->CheckKey(KD_DAT, DIK_LEFT)) {
		transform.rotation.y -= 3.0f * DegToRad;
	}
}
