#include "Dancer.h"
#include "Player.h"
#include "Score.h"

Dancer::Dancer()
{
	animator = new Animator(); // �C���X�^���X���쐬

	mesh = new CFbxMesh();
	mesh->Load("Data/Dancer/Character.mesh");
	mesh->LoadAnimation(0, "Data/Dancer/Dancing.anmx", true);
	animator->SetModel(mesh); // ���̃��f���ŃA�j���[�V��������
	animator->Play(0);
	animator->SetPlaySpeed(1.0f);

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);

	transform.position = VECTOR3(20, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);

	state = sNormal;
	hitPoint = 30;

	aroundID = 0;
}

Dancer::~Dancer()
{
}

void Dancer::Start()
{
	areaCenter = transform.position;
}

void Dancer::Update()
{
	switch (state) {
	case sNormal:
		animator->Update();
		updateNormal();
		break;
	case sReach:
		updateReach();
		break;
	case sBack:
		updateBack();
		break;
	case sDead:
		updateDead();
		break;
	}
}

SphereCollider Dancer::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}

void Dancer::AddDamage(float damage, VECTOR3 pPos)
{
	hitPoint -= damage;
	if (hitPoint > 0) {
		// �v���C���[�̍��W����ɓ����
		VECTOR3 push = transform.position - pPos;	// ��ԕ����̃x�N�g�������
		push.y = 0;
		push = XMVector3Normalize(push) * 0.2f;	// ���̃x�N�g���̒�����0.2�ɂ���
		transform.position += push;	// transform.position�Ƀx�N�g����������
		transform.rotation.y = atan2f(-push.x, -push.z);
	}
	else {
		Score* sc = ObjectManager::FindGameObject<Score>();
		sc->AddScore(100);

		VECTOR3 push = transform.position - pPos;	// ��ԕ����̃x�N�g�������
		transform.rotation.y = atan2f(-push.x, -push.z);
		deadTimer = 0.0f;
		state = sDead;
	}
}

void Dancer::updateNormal()
{
	// ���񃋁[�g
	std::vector<VECTOR3> around = {
		VECTOR3(0,0,0),
		VECTOR3(0,0,2),
		VECTOR3(2,0,2),
		VECTOR3(0,0,2),
	};
	VECTOR3 target = around[aroundID] + areaCenter;
	moveToTarget(target);
	if ((target - transform.position).Length() < 0.5f) {
//		if (aroundID == around.size()-1)
//			aroundID = 0;
//		else
//			aroundID++;

//		if (++aroundID >= around.size())
//			aroundID = 0;

		aroundID = (aroundID + 1) % around.size();
	}

	// �����̎���ɓ�������A�v���C���[�Ɍ������Ă���
	Player* player = ObjectManager::FindGameObject<Player>();
	VECTOR3 toPlayer = player->Position() - transform.position;
	if (toPlayer.Length() < 10.0f) {
		// �����̐��ʂ̃x�N�g�������
		VECTOR3 front = VECTOR3(0, 0, 1) * XMMatrixRotationY(transform.rotation.y);
		// �v���C���[�ւ̃x�N�g���̒����P�̂���
		VECTOR3 toPlayer1 = XMVector3Normalize(toPlayer);
		// ���ς���� �����ꂪcos��
		float ip = Dot(front, toPlayer1);
		if (ip >= cosf(60.0f * DegToRad)) {
			// ����ɓ�����
			state = sReach;
		}
	}
}

void Dancer::updateReach()
{
	Player* player = ObjectManager::FindGameObject<Player>();
	moveToTarget(player->Position());

	// �G���A�̊O�ɏo����Astate��Back�ɕς��遨�������updateBack���Ă΂��
	VECTOR3 diff = areaCenter - transform.position;
	if (diff.Length() > 6.0f) {
		state = sBack;
	}
}

void Dancer::updateBack()
{
	// �G���A�̒��S�Ɍ������Ĉړ�����
	moveToTarget(areaCenter);

	// �G���A�̒��S�ɋ߂Â�����state��Normal�ɕς���
	VECTOR3 diff = areaCenter - transform.position;
	if (diff.Length() < 1.0f) {
		state = sNormal;
	}
}

void Dancer::updateDead()
{
	deadTimer += SceneManager::DeltaTime();
	if (deadTimer <= 2.0f) {
		float rate = deadTimer / 2.0f;
		rate = rate * rate * rate;
		// ���X�ɓ|���
		transform.rotation.x = -90.0f * DegToRad * rate;
	}
	else if (deadTimer >= 4.0f) {
		DestroyMe();
	}
}

void Dancer::moveToTarget(VECTOR3 target)
{
	VECTOR3 toTarget = target - transform.position;

	// �����̐��ʂ̃x�N�g�������
	MATRIX4X4 myRot = XMMatrixRotationY(transform.rotation.y);
	VECTOR3 front = VECTOR3(0, 0, 1) * myRot;
	transform.position += front * 0.01f; // �Ƃ肠����
	VECTOR3 right = VECTOR3(1, 0, 0) * myRot;

	// �v���C���[�ւ̃x�N�g���̒����P�̂���
	VECTOR3 toTarget1 = XMVector3Normalize(toTarget);
	// ���ς���� �����ꂪcos��
	float ip = Dot(front, toTarget1);
	float rSpeed = 3.0f * DegToRad;
	if (ip >= cosf(rSpeed)) {
		transform.rotation.y = atan2f(toTarget.x, toTarget.z);
	}
	else if (Dot(right, toTarget) > 0) {
		transform.rotation.y += rSpeed; // �E�ɋȂ���
	}
	else {
		transform.rotation.y -= rSpeed; // ���ɋȂ���
	}
}
