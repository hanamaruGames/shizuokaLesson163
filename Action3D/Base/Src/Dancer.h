#pragma once
#include "Object3D.h"
#include "Animator.h"

class Dancer : public Object3D {
public:
	Dancer();
	~Dancer();
	void Start() override; // �ŏ���Update���ĂԑO��1��Ă΂��
	void Update() override;
	SphereCollider Collider() override;
	void AddDamage(float damage, VECTOR3 pPos);
private:
	Animator* animator;
	float hitPoint;

	enum State {
		sNormal = 0, // �ʏ���(����)
		sReach,      // �ǂ�������
		sBack,       // �A��
		sDead, // ���S
	};
	State state;
	void updateNormal();
	void updateReach();
	void updateBack();
	void updateDead();
	float deadTimer; // ���S����

	VECTOR3 areaCenter; // �G���A�̒��S���W

	void moveToTarget(VECTOR3 target);

	int aroundID;
};