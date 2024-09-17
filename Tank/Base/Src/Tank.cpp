#include "Tank.h"
#include "Bullet.h"
#include "Reticle.h"

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
		mesh = nullptr;
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
		                      //回ってない時の移動量 * 回転行列
		transform.position += VECTOR3(0, 0, 0.1f) *	XMMatrixRotationY(transform.rotation.y);
	}
}

TankTower::TankTower(Object3D* parentModel)
{
	parent = parentModel;
	transform.SetParent(parent); // transformにも親を教える

	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/tank_tower_001.mesh");
	transform.position = VECTOR3(0, 2, 0); // 親に対して、(0,2,0)の位置

	new TankGun(this);
}

TankTower::~TankTower()
{
}

void TankTower::Update()
{
	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_DAT, DIK_RIGHT)) {
		transform.rotation.y += 3.0f * DegToRad; // 親に対しての回転量
	}
	if (di->CheckKey(KD_DAT, DIK_LEFT)) {
		transform.rotation.y -= 3.0f * DegToRad; // 親に対しての回転量
	}
}

TankGun::TankGun(Object3D* parentModel)
{
	parent = parentModel;
	transform.SetParent(parent); // transformにも親を教える

	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/tank_gun_001.mesh");
	transform.position = VECTOR3(0, 0.535, 1.467); // 親に対しての位置
}

TankGun::~TankGun()
{
}

void TankGun::Update()
{
	Reticle* reticle = ObjectManager::FindGameObject<Reticle>();

	auto di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_DAT, DIK_UP)) {
		transform.rotation.x -= 3.0f * DegToRad; // 親に対しての回転量
		if (transform.rotation.x < -90.0f * DegToRad) {
			transform.rotation.x = -90.0f * DegToRad;
		}
		VECTOR3 pos = reticle->Position();
		pos.y -= 5.0f;
		reticle->SetPosition(pos);
	}
	if (di->CheckKey(KD_DAT, DIK_DOWN)) {
		transform.rotation.x += 3.0f * DegToRad; // 親に対しての回転量
		if (transform.rotation.x > 10.0f * DegToRad) {
			transform.rotation.x = 10.0f * DegToRad;
		}
		VECTOR3 pos = reticle->Position();
		pos.y += 5.0f;
		reticle->SetPosition(pos);
	}
	if (di->CheckKey(KD_TRG, DIK_SPACE)) {
		VECTOR3 shotPos = GameDevice()->m_vEyePt;

		VECTOR3 dir = reticle->GetWorldPos() - shotPos;
		VECTOR3 shotDir = XMVector3Normalize(dir);
		new Bullet(shotPos+shotDir*4.0f, shotDir * 2.0f);
	}
}
