#pragma once
#include "GameObject.h"
#include "FbxMesh.h"
#include "MeshCollider.h"

class Transform {
public:
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	Transform() {
		position = VECTOR3(0, 0, 0);
		rotation = VECTOR3(0, 0, 0);
		scale = VECTOR3(1, 1, 1);
	}
	const MATRIX4X4 matrix() {
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
		return scaleM * rotZ * rotX * rotY * trans;
	}
};

class SphereCollider {
public:
	VECTOR3 center;
	float radius;
	SphereCollider() {
		center = VECTOR3(0, 0, 0);
		radius = 0.0f;
	}
};

class Object3D : public GameObject {
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update() override;
	virtual void Draw() override;

	const VECTOR3 Position() {
		return transform.position;
	};
	const VECTOR3 Rotation() {
		return transform.rotation;
	};
	const VECTOR3 Scale() {
		return transform.scale;
	};
	const MATRIX4X4 Matrix() {
		return transform.matrix();
	}

	void SetPosition(const VECTOR3& pos);
	void SetPosition(float x, float y, float z);
	void SetRotation(const VECTOR3& pos);
	void SetRotation(float x, float y, float z);
	void SetScale(const VECTOR3& pos);
	void SetScale(float x, float y, float z);

	virtual SphereCollider Collider();

	/// <summary>
	/// ���ƃ��b�V���̓����蔻�������
	/// ���������ꍇ�ɂ̂݁Apush�ɉ����Ԃ��ꏊ��Ԃ�
	/// </summary>
	/// <param name="sphere">����</param>
	/// <param name="push">�����Ԃ��ꏊ���i�[����ꏊ</param>
	/// <returns>���������ꍇ��true</returns>
	virtual bool HitSphereToMesh(const SphereCollider& sphere, VECTOR3* push = nullptr);

	/// <summary>
	/// �����ƃ��b�V���̓����蔻�������
	/// ���������ꍇ�́A�����������W��Ԃ�
	/// </summary>
	/// <param name="from">�����̎n�_</param>
	/// <param name="to">�����̏I�_</param>
	/// <param name="hit">��_���i�[����ꏊ</param>
	/// <returns>���������ꍇ��true</returns>
	virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit = nullptr);

	/// <summary>
	/// ���Ƌ��̓����蔻�������
	/// �����̋��́ACollider()�Ŏ擾����
	/// </summary>
	/// <param name="target">����̋�</param>
	/// <param name="withY">false�ɂ����Y�̍��W���𖳎�����</param>
	/// <returns>�d�Ȃ��</returns>
	virtual float HitSphereToSphere(const SphereCollider& target, bool withY=true);
protected:
	CFbxMesh* mesh;
	MeshCollider* meshCol;
	Transform transform;
};