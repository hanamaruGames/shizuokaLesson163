#pragma once
#include "Particle.h"

class Explode : public Particle
{
public:
	Explode();
	~Explode();
	void Update() override;
private:
	int animTime; // �A�j���[�V�����Đ�����
};
