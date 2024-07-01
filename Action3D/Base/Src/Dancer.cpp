#include "Dancer.h"
#include "Player.h"

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
	hitPoint = 10;
}

Dancer::~Dancer()
{
}

void Dancer::Update()
{
	switch (state) {
	case sNormal:
		animator->Update();
		updateNormal();
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
		VECTOR3 push = transform.position - pPos;	// ��ԕ����̃x�N�g�������
		transform.rotation.y = atan2f(-push.x, -push.z);
		state = sDead;
	}
}

void Dancer::updateNormal()
{
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
			transform.position += front * 0.01f; // �Ƃ肠����
			VECTOR3 right = VECTOR3(1, 0, 0) * XMMatrixRotationY(transform.rotation.y);
			if (ip >= cosf(3.0f * DegToRad)) {
				transform.rotation.y = atan2f(toPlayer.x, toPlayer.z);
			}
			else if (Dot(right, toPlayer) > 0) {
				transform.rotation.y += 3.0f * DegToRad; // �E�ɋȂ���
			}
			else {
				transform.rotation.y -= 3.0f * DegToRad; // ���ɋȂ���
			}
		}
	}
}

void Dancer::updateDead()
{
	transform.rotation.x = -XM_PI / 2;
}
