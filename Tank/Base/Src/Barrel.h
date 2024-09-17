#pragma once
#include "Object3D.h"

class Barrel : public Object3D {
public:
	Barrel(VECTOR3 pos); // 座標だけもらう
	~Barrel();
//	void Update() override; // 動かないので、要らない
//	void Draw() override; // Object3DのDrawを使うので、要らない

	void OnCollision(Object3D* other); // 当たった時に呼んでもらう
};
