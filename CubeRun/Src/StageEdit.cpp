#include "StageEdit.h"
#include <fstream>
#include "CsvReader.h"
#include "BlockFileName.h"

StageEdit::StageEdit()
{
	stageNumber = 2;
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // ��UC++�̕�����ɂ���
		m->Load((folder+f+".mesh").c_str()); // +�ŕ�������Ȃ��邱�Ƃ��ł���
		meshes.push_back(m);
	}

	// �ŏ���map�f�[�^�����
	for (int y = 0; y < 1; y++) {
		vector<vector<int>> m2;
		for (int z = 0; z < 3; z++) {
			vector<int> m;
			for (int x = 0; x < 4; x++) {
				m.push_back(0);
			}
			m2.push_back(m);
		}
		map.push_back(m2);
	}
	map[0][1][1] = 1;
	map[0][2][3] = 1;
	cursorX = 0;
	cursorZ = 0;

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
	CDirectInput* di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_TRG, DIK_D)) {
		cursorX = min(cursorX + 1, map[cursorY][cursorZ].size() - 1);
	}
	if (di->CheckKey(KD_TRG, DIK_A)) {
		cursorX = max(cursorX - 1, 0);
	}
	if (di->CheckKey(KD_TRG, DIK_S)) {
		cursorZ++;
		if (cursorZ >= map[cursorY].size()) {
			cursorZ = map[cursorY].size() - 1;
		}
	}
	if (di->CheckKey(KD_TRG, DIK_W)) {
		cursorZ--;
		if (cursorZ < 0) {
			cursorZ = 0;
		}
	}
	if (di->CheckKey(KD_TRG, DIK_1)) {
		cursorY = min(cursorY + 1, map.size() - 1);
	}
	if (di->CheckKey(KD_TRG, DIK_2)) {
		cursorY = max(cursorY - 1, 0);
	}
	if (di->CheckKey(KD_TRG, DIK_SPACE)) {
		map[cursorY][cursorZ][cursorX]++;
		if (map[cursorY][cursorZ][cursorX] >= meshes.size()) {
			map[cursorY][cursorZ][cursorX] = -1;
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
	ImGui::InputInt("XSIZE", &xsize);
	ImGui::InputInt("YSIZE", &ysize);
	ImGui::InputInt("ZSIZE", &zsize);
	if (ImGui::Button("CREATE")) {
		Create(xsize, ysize, zsize);
	}
	ImGui::End();

//	ImGui::Begin("CURSOR");
//	ImGui::InputInt("X", &cursorX);
//	ImGui::InputInt("Z", &cursorZ);
//	ImGui::End();
}

void StageEdit::Draw()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(cursorX+5 , cursorY + 20, -cursorZ - 10), // �J�����ʒu
		VECTOR3(cursorX, cursorY, -cursorZ), // �����_
		VECTOR3(0, 1, 1)); // ��x�N�g��

	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			for (int x = 0; x < map[y][z].size(); x++) {
				int chip = map[y][z][x];
				if (chip >= 0) { // ���̎��͌�
					meshes[chip]->Render(XMMatrixTranslation(x, y, -z));
				}
			}
		}
	}
	DrawBox(VECTOR3(cursorX, cursorY, -cursorZ), 0xff0000ff/*ABGR*/);
}

void StageEdit::Save(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);
	// �t�@�C�����J��
	std::ofstream ofs(name); // �����Ƀt�@�C����
	// �f�[�^������
	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			int xs = map[y][z].size();
			for (int x = 0; x < xs; x++) {
				ofs << map[y][z][x];
				if (x < xs - 1) {
					ofs << ",";
				}
			}
			ofs << std::endl;
		}
		ofs << std::endl;
	}
	// �t�@�C�������
	ofs.close();
}

void StageEdit::Load(int n)
{
	char name[64];
	sprintf_s<64>(name, "data/Stage%02d.csv", n);

	CsvReader* csv = new CsvReader(name);
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
	delete csv;
}

void StageEdit::Create(int xsize, int ysize, int zsize)
{
	map.clear();
	for (int y = 0; y < ysize; y++) {
		vector<vector<int>> m2;
		for (int z = 0; z < zsize; z++) {
			vector<int> m;
			for (int x = 0; x < xsize; x++) {
				if (y==0)
					m.push_back(0);
				else
					m.push_back(-1);
			}
			m2.push_back(m);
		}
		map.push_back(m2);
	}
	//�O�������ׂ�-1�ɂ���
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
