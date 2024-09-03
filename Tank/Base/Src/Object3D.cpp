#include "Object3D.h"
#include "MeshCollider.h"

Object3D::Object3D()
{
	mesh = nullptr;
	meshCol = nullptr;
}

Object3D::~Object3D()
{
	if (mesh != nullptr) {
		delete mesh;
	}
	if (meshCol != nullptr) {
		delete meshCol;
	}
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
	mesh->Render(transform.matrix());
}

void Object3D::SetPosition(const VECTOR3& pos)
{
	transform.position = pos;
}

void Object3D::SetPosition(float x, float y, float z)
{
	SetPosition(VECTOR3(x, y, z));
}

void Object3D::SetRotation(const VECTOR3& rot)
{
	transform.rotation = rot;
}

void Object3D::SetRotation(float x, float y, float z)
{
	SetRotation(VECTOR3(x, y, z));
}

void Object3D::SetScale(const VECTOR3& sca)
{
	transform.scale = sca;
}

void Object3D::SetScale(float x, float y, float z)
{
	SetScale(VECTOR3(x, y, z));
}

SphereCollider Object3D::Collider()
{
	SphereCollider sphere;
	sphere.center = transform.position + VECTOR3(0, 1, 0);
	sphere.radius = 0.5f;
	return sphere;
}

bool Object3D::HitSphereToMesh(const SphereCollider& sphere, VECTOR3* push)
{
	if (meshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	std::list<MeshCollider::CollInfo> meshes = meshCol->CheckCollisionSphereList(mat, sphere.center, sphere.radius);
	if (meshes.size() == 0)
		return false;
	if (push != nullptr) { // ���W���K�v�Ȃ̂ł����
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // �ŏI�I�ɉ����x�N�g��
		for (const MeshCollider::CollInfo& m : meshes) { // �������Ă��邷�ׂẴ|���S����
			VECTOR3 move = sphere.center - m.hitPosition;
			float len = move.Length(); // ���������_���璆�S�ւ̋���
			move = move * ((sphere.radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // �����Ԃ������x�N�g��
			// ����pushVec�ƍ�������
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // �����ς݃x�N�g���̌���
			float dot = Dot(push, pushVecNorm);	// ���̐����̒���
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // ���̐��������炵�Ă���
			}
			else {
				pushVec = push;
			}
		}
		*push = pushVec;
	}
	return true;
}

bool Object3D::HitLineToMesh(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit)
{
	if (meshCol == nullptr)
		return false;
	MATRIX4X4 mat = transform.matrix();
	MeshCollider::CollInfo coll;
	bool ret = meshCol->CheckCollisionLine(mat, from, to, &coll);
	if (ret) {
		if (hit != nullptr) {
			*hit = coll.hitPosition;
		}
		return true;
	}
	return false;
}

float Object3D::HitSphereToSphere(const SphereCollider& target, bool withY)
{
	SphereCollider my = Collider();
	VECTOR3 diff = target.center - my.center;
	if (withY == false)
		diff.y = 0.0f;
	float rsum = target.radius + my.radius;
	if (diff.LengthSquare() < rsum * rsum) {
		return rsum - diff.Length();
	}
	return 0.0f;
}

const MATRIX4X4 Transform::matrix()
{
	MATRIX4X4 scaleM = XMMatrixScaling(
		scale.x, scale.y, scale.z);
	MATRIX4X4 rotX = XMMatrixRotationX(
		rotation.x);
	MATRIX4X4 rotY = XMMatrixRotationY(
		rotation.y);
	MATRIX4X4 rotZ = XMMatrixRotationZ(
		rotation.z);
	MATRIX4X4 trans = XMMatrixTranslation(
		position.x, position.y, position.z);
	//		return scaleM * rotZ * rotX * rotY * trans;
	MATRIX4X4 mat = scaleM * rotZ * rotX * rotY * trans;
	if (parent != nullptr) {
		mat = mat * parent->Matrix();
	}
	return mat;
}
