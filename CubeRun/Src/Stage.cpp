#include "Stage.h"
#include "CsvReader.h"
#include "BlockFileName.h"

Stage::Stage(int stageNumber)
{
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // ˆê’UC++‚Ì•¶š—ñ‚É‚·‚é
		m->Load((folder + f + ".mesh").c_str()); // +‚Å•¶š—ñ‚ğ‚Â‚È‚°‚é‚±‚Æ‚ª‚Å‚«‚é
		meshes.push_back(m);
	}

	Load(stageNumber);
}

Stage::~Stage()
{
//	if (csv != nullptr) {
//		delete csv;
//		csv = nullptr;
//	}
	SAFE_DELETE(csv);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int z = 0; z < map.size(); z++) {
		for (int x = 0; x < map[z].size(); x++) {
			int chip = map[z][x];
			if (chip >= 0) { // •‰‚Ì‚ÍŒŠ
				meshes[chip]->Render(XMMatrixTranslation(x, 0, -z));
			}
		}
	}
}

void Stage::Load(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);

	SAFE_DELETE(csv);
	csv = new CsvReader(name);
	map.clear();
	for (int z = 0; z < csv->GetLines(); z++) {
		std::vector<int> m;
		for (int x = 0; x < csv->GetColumns(z); x++) {
			int d = csv->GetInt(z, x);
			m.push_back(d);
		}
		map.push_back(m);
	}
}

bool Stage::IsLandBlock(VECTOR3 pos)
{
	int chipZ = 0/*pos.z ‚©‚ç‹‚ß‚é*/;
	int chipX = (int)(pos.x+0.5f);

	ImGui::Begin("CHIP");
	ImGui::InputFloat("pos.z", &pos.z);
	ImGui::InputInt("chipZ", &chipZ);
	ImGui::End();

	if (pos.x + 0.5f < 0.0f) {
		return false;
	}
	if (csv == nullptr) {
		return false;
	}
	if (chipX >= csv->GetColumns(chipZ)) {
		return false;
	}
	if (csv->GetInt(chipZ, chipX) >= 0) {
		return true;
	}
	return false;
}
