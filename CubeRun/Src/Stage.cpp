#include "Stage.h"
#include "CsvReader.h"
#include "BlockFileName.h"

Stage::Stage(int stageNumber)
{
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // ��UC++�̕�����ɂ���
		m->Load((folder + f + ".mesh").c_str()); // +�ŕ�������Ȃ��邱�Ƃ��ł���
		meshes.push_back(m);
	}

	Load(stageNumber);

}

Stage::~Stage()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int z = 0; z < map.size(); z++) {
		for (int x = 0; x < map[z].size(); x++) {
			int chip = map[z][x];
			if (chip >= 0) { // ���̎��͌�
				meshes[chip]->Render(XMMatrixTranslation(x, 0, -z));
			}
		}
	}
}

void Stage::Load(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);

	CsvReader* csv = new CsvReader(name);
	map.clear();
	for (int z = 0; z < csv->GetLines(); z++) {
		std::vector<int> m;
		for (int x = 0; x < csv->GetColumns(z); x++) {
			int d = csv->GetInt(z, x);
			m.push_back(d);
		}
		map.push_back(m);
	}
	delete csv;
}

bool Stage::IsLandBlock(VECTOR3 pos)
{
	return false;
}
