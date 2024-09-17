#pragma once
// FireSplash.h
#include "Particle.h"

class FireSplash : public Particle {
public:
	FireSplash();
	~FireSplash();
	void Update() override;
private:
	struct ChipControl {
		VECTOR3 velocity; // ˆÚ“®‘¬“x
	};
	std::list<ChipControl> controls;
};