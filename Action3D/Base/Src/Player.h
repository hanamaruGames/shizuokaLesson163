#pragma once
// Player.h
#include "Object3D.h"
#include "Animator.h"

class Player : public Object3D {
public:
	Player();
	~Player();
	void Update() override;
	SphereCollider Collider() override;

private:
	float speedY; // Y�̑��x

	Animator* animator; // ���i�̃C���X�^���X���|�C���^�[�^�ŏ���
	enum AnimID {
		aIdle = 0,
		aRun = 1,
	};

	enum State {
		sOnGround = 0,
		sJump,
		sAttack,
	};
	State state;
	void UpdateOnGround();
	void UpdateJump();
	void UpdateAttack();

	int frame; // �A�j���[�V�����̃t���[���𐔂���
};