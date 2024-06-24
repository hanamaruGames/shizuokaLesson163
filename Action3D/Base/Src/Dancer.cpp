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

	transform.position = VECTOR3(20, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
}

Dancer::~Dancer()
{
}

void Dancer::Update()
{
	animator->Update();

	// �����̎���ɓ�������A�v���C���[�Ɍ������Ă���
	Player* player = ObjectManager::FindGameObject<Player>();
	VECTOR3 toPlayer = player->Position() - transform.position;
	if (toPlayer.Length() < 10.0f) {
		// �����̐��ʂ̃x�N�g�������
		VECTOR3 front = VECTOR3(0, 0, 1)* XMMatrixRotationY(transform.rotation.y);
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
			} else if (Dot(right, toPlayer) > 0) {
				transform.rotation.y += 3.0f*DegToRad; // �E�ɋȂ���
			}
			else {
				transform.rotation.y -= 3.0f*DegToRad; // ���ɋȂ���
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
