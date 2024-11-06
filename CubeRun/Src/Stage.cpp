#include "Stage.h"
#include "CsvReader.h"
#include "BlockFileName.h"
#include "Player.h"
#include "Coin.h"

Stage::Stage(int stageNumber)
{
	for (const std::string& f : files) {
		CFbxMesh* m = new CFbxMesh();
		std::string folder = "data/models/"; // ��UC++�̕�����ɂ���
		m->Load((folder + f + ".mesh").c_str()); // +�ŕ�������Ȃ��邱�Ƃ��ł���
		meshes.push_back(m);
	}
	boxCollider = new MeshCollider();
	boxCollider->MakeFromFile("data/models/boxCol.mesh");

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
				if (chip >= 0) { // ���̎��͌�
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
				if (d == boxID::CHAR01) {
					Player* p = new Player();
					p->SetPosition(x, map.size(), -(int)m2.size());
					d = -1;
				}
				else if (d == boxID::COIN) {
					Coin* c = new Coin();
					c->SetPosition(x, map.size(), -(int)m2.size());
					d = -1;
				}
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

bool Stage::HitSphere(const SphereCollider& coll, VECTOR3* out)
{
	VECTOR3 pushVec = VECTOR3(0, 0, 0);
	VECTOR3 pushVecCorner = VECTOR3(0, 0, 0);
	bool pushed = false;
	for (int y = 0; y < map.size(); y++) {
		for (int z = 0; z < map[y].size(); z++) {
			for (int x = 0; x < map[y][z].size(); x++) {
				if (map[y][z][x] >= 0) {
					VECTOR3 dist = coll.center - VECTOR3(x, y, -z);
					if (dist.LengthSquare() >= (coll.radius + 1.5f) * (coll.radius + 1.5f))
						continue;
					MATRIX4X4 mat = XMMatrixTranslation(x, y, -z);
					std::list<MeshCollider::CollInfo> meshes = boxCollider->CheckCollisionSphereList(mat, coll.center, coll.radius);
					for (const MeshCollider::CollInfo& m : meshes) {
						VECTOR3 move = coll.center - m.hitPosition;
						VECTOR3 v = XMVector3Cross(move, m.normal);
						if (v.Length() <= 0.0001f) {
							float len = move.Length(); // ���������_���璆�S�ւ̋���
							if (len > 0.0f) {
								move = move * ((coll.radius - len) / len);
							}
							VECTOR3 push = m.normal * Dot(move, m.normal); // �����Ԃ������x�N�g��
							// ����pushVec�ƍ�������
							VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // �����ς݃x�N�g���̌���
							float dot = Dot(push, pushVecNorm);	// ���̐����̒���
//							if (dot < pushVec.Length()) {
								pushVec += push - pushVecNorm * dot; // ���̐��������炵�Ă���
//							}
//							else {
//								pushVec = push;
//							}
						}
						else {
							float len = move.Length(); // ���������_���璆�S�ւ̋���
							move = move * ((coll.radius - len) / len);
							VECTOR3 push = m.normal * Dot(move, m.normal); // �����Ԃ������x�N�g��
							// ����pushVec�ƍ�������
							VECTOR3 pushVecNorm = XMVector3Normalize(pushVecCorner); // �����ς݃x�N�g���̌���
							float dot = Dot(push, pushVecNorm);	// ���̐����̒���
//							if (dot < pushVecCorner.Length()) {
								pushVecCorner += push - pushVecNorm * dot; // ���̐��������炵�Ă���
//							}
//							else {
//								pushVecCorner = push;
//							}
						}
						pushed = true;
					}
				}
			}
		}
	}
	if (pushed && out != nullptr) {
		if (pushVec.LengthSquare() > 0.0f) {
			*out = pushVec;
		}
		else {
			*out = pushVecCorner;
		}
	}
	return pushed;
}
