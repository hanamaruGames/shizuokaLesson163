#include "Bullet.h"
#include "Ground.h"
#include "Barrel.h"

Bullet::Bullet(VECTOR3 from, VECTOR3 direction)
{
	transform.position = from;
	velocity = direction;

	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/bullet.mesh");
	transform.scale = VECTOR3(0.5, 0.5, 0.5);
	destroyCount = 0;
	hitBarrel = nullptr;
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	if (destroyCount > 0) {
		destroyCount--;
		if (destroyCount == 0) {
			DestroyMe();
			if (hitBarrel != nullptr) {
				hitBarrel->DestroyMe();
			}
		}
		return;
	}

	VECTOR3 begin = transform.position;

	transform.position += velocity;
	VECTOR3 end = transform.position;

	// �܂��n�ʁE�ǂƓ��Ă�
	Ground* pG = ObjectManager::FindGameObject<Ground>();
	if (pG != nullptr) {
		VECTOR3 hit;
		if (pG->HitLineToMesh(begin, end, &hit)) {
			// �����ɗ������A�������Ă��āA�����������W��hit�ɂ���
//			DestroyMe();
			destroyCount = 5;
			transform.position = hit;
		}
	}
	// �h�����ʂƓ��Ă�
	std::list<Barrel*> pBarrels = ObjectManager::FindGameObjects<Barrel>();
	for (Barrel* b : pBarrels) {
		VECTOR3 hit;
		if (b->HitLineToMesh(begin, end, &hit)) {
//			DestroyMe();
//			b->DestroyMe();
			destroyCount = 5;
			transform.position = hit;
			hitBarrel = b;
		}
	}
}
