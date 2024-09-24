#pragma once
// Stage.h
#include "Object3D.h"

using namespace std;

class Stage : public Object3D {
public:
	Stage(int stageNumber);
	~Stage();
	void Update() override;
	void Draw() override;
	void Load(int n);
	bool IsLandBlock(VECTOR3 pos);
private:
	vector<CFbxMesh*> meshes; // 可変の配列
	vector<vector<int>> map; // マップデータの二次元配列
};
