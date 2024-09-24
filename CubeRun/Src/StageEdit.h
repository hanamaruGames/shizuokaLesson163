#pragma once
// StageEdit.h
#include "Object3D.h"

class StageEdit : public Object3D {
public:
	StageEdit();
	~StageEdit();
	void Update() override;
	void Draw() override;
private:
	std::vector<CFbxMesh*> meshes; // ‰Â•Ï‚Ì”z—ñ
};