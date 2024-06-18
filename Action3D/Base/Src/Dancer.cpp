#include "Dancer.h"

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
	// �����̎���ɓ�������A�v���C���[�Ɍ������Ă���

}

SphereCollider Dancer::Collider()
{
	SphereCollider col;
	col.center = transform.position;
	col.radius = 0.5f;
	return col;
}
