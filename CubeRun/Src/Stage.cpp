#include "Stage.h"
#include "CsvReader.h"
#include "BlockFileName.h"

Stage::Stage(int stageNumber)
{
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // ˆê’UC++‚Ì•¶Žš—ñ‚É‚·‚é
		m->Load((folder + f + ".mesh").c_str()); // +‚Å•¶Žš—ñ‚ð‚Â‚È‚°‚é‚±‚Æ‚ª‚Å‚«‚é
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
	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			for (int x = 0; x < map[y][z].size(); x++) {
				int chip = map[y][z][x];
				if (chip >= 0) { // •‰‚ÌŽž‚ÍŒŠ
					meshes[chip]->Render(XMMatrixTranslation(x, y, -z));
				}
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

	std::vector<std::vector<int>> m2;
	m2.clear();
	for (int line = 0; line < csv->GetLines(); line++) {
		if (csv->GetString(line, 0) == "") {
			map.push_back(m2);
			m2.clear();
		}
		else {
			std::vector<int> m;
			for (int x = 0; x < csv->GetColumns(line); x++) {
				int d = csv->GetInt(line, x);
				m.push_back(d);
			}
			m2.push_back(m);
		}
	}
	if (m2.size() > 0) {
		map.push_back(m2);
	}
}

bool Stage::IsLandBlock(VECTOR3 pos)
{
	if (csv == nullptr) {
		return false;
	}
	if (-pos.z + 0.5f < 0.0f) {
		return false;
	}
	int chipZ = (int)(-pos.z + 0.5f);
	if (pos.x + 0.5f < 0.0f) {
		return false;
	}
	int chipX = (int)(pos.x+0.5f);

	if (pos.y < 0.0f) {
		return false;
	}
	int chipY = (int)pos.y;
	if (chipY >= map.size()) {
		return false;
	}

	if (chipZ >= map[chipY].size()) {
		return false;
	}
	if (chipX >= map[chipY][chipZ].size()) {
		return false;
	}
	if (map[chipY][chipZ][chipX]>=0) {
		return true;
	}
	return false;
}
