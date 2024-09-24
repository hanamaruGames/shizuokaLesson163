#pragma once
// StageEdit.h
#include "Object3D.h"

using namespace std;

class StageEdit : public Object3D {
public:
	StageEdit();
	~StageEdit();
	void Update() override;
	void Draw() override;
private:
	vector<CFbxMesh*> meshes; // 可変の配列
	vector<vector<int>> map; // マップデータの二次元配列
};