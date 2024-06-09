#pragma once

#include <vector>
#include "Object3D.h"

/// <summary>
/// ���f���f�[�^�Ƃ̓����蔻����s���܂�
/// �������Ă���|���S���̒��_���W�A�@���A���������_��Ԃ��܂�
/// �����A�܂��́A���ŁA������s���܂�
/// </summary>
class MeshCollider
{
public:
	struct CollInfo {
		VECTOR3 hitPosition; // ���������ʒu
		VECTOR3 normal; // �@��
		VECTOR3 triangle[3]; // ���_
	};

	MeshCollider();
	MeshCollider(Object3D* object);
	~MeshCollider();

	/// <summary>
	/// FbxMesh�f�[�^����A����f�[�^���쐬����
	/// </summary>
	/// <param name="mesh">���b�V���f�[�^</param>
	void MakeFromMesh(CFbxMesh* mesh);

	/// <summary>
	/// mesh�t�@�C������A����f�[�^���쐬����
	/// �����蔻��p�Ɍy�ʉ����f�����g���ꍇ�͂�������g���Ă�������
	/// </summary>
	/// <param name="fileName">���f���f�[�^�̃p�X</param>
	void MakeFromFile(std::string fileName);

	/// <summary>
	/// �����Ƃ̓����蔻����s��
	/// �n�_�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="from">�����̎n�_</param>
	/// <param name="to">�����̏I�_</param>
	/// <param name="info">���������|���S���̏�������ꏊ</param>
	/// <returns>�������Ă����true</returns>
	bool CheckCollisionLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to, CollInfo* hitOut = nullptr);

	/// <summary>
	/// ���̂Ƃ̓����蔻����s��
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// 
	/// ���̒��S�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// info��nullptr�̏ꍇ�́A���������Ԃ����A�����������̂ݕԂ��܂�
	/// 
	/// �����̃|���S����������\��������̂ŁACheckCollisionSphereList�֐����g�p���邱�Ƃ𐄏����܂�
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="center">���̒��S���W</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="info">���������|���S���̏�������ꏊ</param>
	/// <returns>�������Ă����true</returns>
	bool CheckCollisionSphere(const MATRIX4X4& trans, const VECTOR3& center, float radius, CollInfo* hitOut = nullptr);

	/// <summary>
	/// ���̂Ƃ̓����蔻����s��
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// 
	/// �������Ă���|���S�����ׂĂ̏���Ԃ��܂�
	/// �������Ă�����̂��Ȃ���΁Areturn��size��0�ɂȂ�܂�
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="center">���̒��S���W</param>
	/// <param name="radius">���̔��a</param>
	/// <returns>���������|���S���S�Ă̈ʒu���</returns>
	std::list<MeshCollider::CollInfo> CheckCollisionSphereList(const MATRIX4X4& trans, const VECTOR3& center, float radius);

	/// <summary>
	/// �J�v�Z���Ƃ̓����蔻����s��
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// 
	/// �J�v�Z����p1�̓_�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// info��nullptr�̏ꍇ�́A���������Ԃ����A�����������̂ݕԂ��܂�
	/// 
	/// �����̃|���S����������\��������̂ŁACheckCollisionCapsuleList�֐����g�p���邱�Ƃ𐄏����܂�
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="p1">�J�v�Z���̓_�P</param>
	/// <param name="p2">�J�v�Z���̓_�Q</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="info">���������|���S���̏�������ꏊ</param>
	/// <returns>�������Ă����true</returns>
	//bool CheckCollisionCapsule(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius, CollInfo* hitOut = nullptr);

	/// <summary>
	/// �J�v�Z���Ƃ̓����蔻����s��
	/// �������Ă���|���S�����ׂĂ̏���Ԃ��܂�
	/// 
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="p1">�J�v�Z���̓_�P</param>
	/// <param name="p2">�J�v�Z���̓_�Q</param>
	/// <param name="radius">���̔��a</param>
	/// <returns>���������|���S���S�Ă̈ʒu���</returns>
	//std::list<MeshCollider::CollInfo> CheckCollisionCapsuleList(const MATRIX4X4& trans, const VECTOR3& p1, const VECTOR3& p2, float radius);

	//bool CheckCollisionTriangle(const MATRIX4X4& trans, const VECTOR3* vertexes, CollInfo* info = nullptr);

	/// <summary>
	/// �����Ƃ̓����蔻����s��
	/// �n�_�����ԋ߂��|���S���̓����蔻�����Ԃ��܂�
	/// �|���S���̕\�ʂ̂ݔ��肵�A���ʂ͔��肵�܂���
	/// </summary>
	/// <param name="trans">���̃R���C�_�[�̈ʒu���</param>
	/// <param name="from">�����̎n�_</param>
	/// <param name="to">�����̏I�_</param>
	/// <param name="info">���������|���S���̏�������ꏊ</param>
	/// <returns>�������Ă����true</returns>
	bool CheckBoundingLine(const MATRIX4X4& trans, const VECTOR3& from, const VECTOR3& to);


private:
	Object3D* parent;

	struct BoundingBox {
		VECTOR3 min;
		VECTOR3 max;
		BoundingBox() {
			min = VECTOR3(0, 0, 0), max = VECTOR3(0, 0, 0);
		}
	};
	struct BoundingBall {
		VECTOR3 center;
		float radius;
		BoundingBall() {
			center = VECTOR3(0, 0, 0); radius = 0.0f;
		}
	};
	struct PolygonInfo {
		int indices[3]; // ���_�ԍ�
		int mesh;
		VECTOR3 normal; // �ʂ̖@��
	};
	std::vector<PolygonInfo> polygons;
	std::vector<VECTOR3> vertices;
	BoundingBox bBox;
	BoundingBall bBall;

	bool checkPolygonToLine(const PolygonInfo& info, const VECTOR3& from, const VECTOR3& to, CollInfo* hit = nullptr);
	bool checkPolygonToSphere(const PolygonInfo& info, const VECTOR3& center, float radius, CollInfo* hit = nullptr);
};