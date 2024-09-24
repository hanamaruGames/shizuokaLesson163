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
	vector<CFbxMesh*> meshes; // �ς̔z��
	vector<vector<int>> map; // �}�b�v�f�[�^�̓񎟌��z��
};