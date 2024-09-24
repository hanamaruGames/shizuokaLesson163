#include "StageEdit.h"
#include <fstream>
#include "CsvReader.h"
#include "BlockFileName.h"

StageEdit::StageEdit()
{
	stageNumber = 2;
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // 一旦C++の文字列にする
		m->Load((folder+f+".mesh").c_str()); // +で文字列をつなげることができる
		meshes.push_back(m);
	}

	// 最初のmapデータを作る
	for (int z = 0; z < 3; z++) {
		vector<int> m;
		for (int x = 0; x < 4; x++) {
			m.push_back(0);
		}
		map.push_back(m);
	}
	map[1][1] = 1;
	map[2][3] = 1;
	cursorX = 0;
	cursorZ = 0;

	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 20, -10), // カメラ位置
		VECTOR3(0, 0, 0), // 注視点
		VECTOR3(0, 1, 1)); // 上ベクトル
}

StageEdit::~StageEdit()
{
}

void StageEdit::Update()
{
	CDirectInput* di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_D)) {
		cursorX = min(cursorX + 1, map[cursorZ].size() - 1);
	}
	if (di->CheckKey(KD_TRG, DIK_A)) {
		cursorX = max(cursorX - 1, 0);
	}
	if (di->CheckKey(KD_TRG, DIK_S)) {
		cursorZ++;
		if (cursorZ >= map.size()) {
			cursorZ = map.size() - 1;
		}
	}
	if (di->CheckKey(KD_TRG, DIK_W)) {
		cursorZ--;
		if (cursorZ < 0) {
			cursorZ = 0;
		}
	}
	if (di->CheckKey(KD_TRG, DIK_SPACE)) {
		map[cursorZ][cursorX]++;
		if (map[cursorZ][cursorX] >= meshes.size()) {
			map[cursorZ][cursorX] = -1;
		}
	}

//	if (di->CheckKey(KD_TRG, DIK_O)) {
//		Save();
//	}
//	if (di->CheckKey(KD_TRG, DIK_I)) {
//		Load();
//	}

	ImGui::Begin("MENU");
	ImGui::InputInt("Stage", &stageNumber);
	if (ImGui::Button("SAVE")) {
		Save(stageNumber);
	}
	if (ImGui::Button("LOAD")) {
		Load(stageNumber);
	}
	ImGui::End();

	ImGui::Begin("CURSOR");
	ImGui::InputInt("X", &cursorX);
	ImGui::InputInt("Z", &cursorZ);
	ImGui::End();
}

void StageEdit::Draw()
{
	for (int z = 0; z < map.size(); z++) {
		for (int x = 0; x < map[z].size(); x++) {
			int chip = map[z][x];
			if (chip >= 0) { // 負の時は穴
				meshes[chip]->Render(XMMatrixTranslation(x, 0, -z));
			}
		}
	}
	DrawBox(VECTOR3(cursorX, 0, -cursorZ), 0xff0000ff/*ABGR*/);
}

void StageEdit::Save(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);
	// ファイルを開く
	std::ofstream ofs(name); // 引数にファイル名
	// データを書く
	for (int z = 0; z < map.size(); z++) {
		int xs = map[z].size();
		for (int x = 0; x < xs; x++) {
			ofs << map[z][x];
			if (x < xs - 1) {
				ofs << ",";
			}
		}
		ofs << std::endl;
	}
	// ファイルを閉じる
	ofs.close();
}

void StageEdit::Load(int n)
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


void StageEdit::DrawBox(VECTOR3 pos, DWORD color)
{
	CSprite spr;
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f, -0.5f), pos + VECTOR3(0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, -0.0f, -0.5f), pos + VECTOR3(0.5f, 1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, 1.0f, -0.5f), pos + VECTOR3(-0.5f, 1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, 1.0f, -0.5f), pos + VECTOR3(-0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f, 0.5f), pos + VECTOR3(0.5f, -0.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, -0.0f, 0.5f), pos + VECTOR3(0.5f, 1.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, 1.0f, 0.5f), pos + VECTOR3(-0.5f, 1.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, 1.0f, 0.5f), pos + VECTOR3(-0.5f, -0.0f, 0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, -0.0f, 0.5f), pos + VECTOR3(-0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, -0.0f, 0.5f), pos + VECTOR3(0.5f, -0.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(-0.5f, 1.0f, 0.5f), pos + VECTOR3(-0.5f, 1.0f, -0.5f), color);
	spr.DrawLine3D(pos + VECTOR3(0.5f, 1.0f, 0.5f), pos + VECTOR3(0.5f, 1.0f, -0.5f), color);
}
