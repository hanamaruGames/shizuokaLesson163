#include "StageEdit.h"

int map[3][4] = {
	{ 0, 0, 0, 0},
	{ 0, 1, 0, 0},
	{ 0, 0, 0, 1},
};

std::vector<std::string> files = {
	"wallEarth01",
	"boxQuestion",
};

StageEdit::StageEdit()
{
	for (std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // ��UC++�̕�����ɂ���
		m->Load((folder+f+".mesh").c_str()); // +�ŕ�������Ȃ��邱�Ƃ��ł���
		meshes.push_back(m);
	}

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 20, -10), // �J�����ʒu
		VECTOR3(0, 0, 0), // �����_
		VECTOR3(0, 1, 1)); // ��x�N�g��
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
}

void StageEdit::Draw()
{
	for (int z = 0; z < 3; z++) {
		for (int x = 0; x < 4; x++) {
			int chip = map[z][x];
			meshes[chip]->Render(XMMatrixTranslation(x, 0, -z));
		}
	}
}
