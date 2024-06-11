#include "PlayScene.h"
#include "Player.h"
#include "Ground.h"
#include "Door.h"
#include "Dancer.h"
#include <fstream>
#include "CsvReader.h"
#include <assert.h>

PlayScene::PlayScene()
{
	// �e�L�X�g�t�@�C���̓ǂݕ�
	// �t�@�C�����X�g���[��
#if 0
	std::ifstream ifs("Data/map00.csv"); // �t�@�C�����J��
	if (ifs) { // �t�@�C�����J���Ă���
		std::string str;
		std::getline(ifs, str); // �����łP�s�ǂ�
		std::getline(ifs, str); // �����łP�s�ǂ�
		// ������2�s�ڂ̃f�[�^�������Ă���
		// "PLAYER,0,0,0"�������Ă���B[,]�Ő؂��ĂS�̃f�[�^�ɕ�����
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == ',') {
				// �������؂���ɂ́Astr.substr(/*�������ڂ���*/, /*������*/);
				// �Q�߂̈������w�肵�Ȃ��ƁA�Ō�܂łƂ������ƂɂȂ�
				std::string before = str.substr(0, i); // ,�̑O"PLAYER"������͂�
				std::string after = str.substr(i+1); // ,�̌��"0,0,0"������͂�
			}
		}
	}
	ifs.close(); // �t�@�C�������
#endif
	CsvReader* csv = new CsvReader("Data/map00.csv");
	assert(csv->GetLines() > 0);
	for (int i = 1; i < csv->GetLines(); i++) { // �P�s���ǂ�
		std::string str = csv->GetString(i, 0); // PLAYER������
		Object3D* obj;
		if (str == "PLAYER") {
			obj = Instantiate<Player>();
		}
		else if (str == "GROUND") {
			obj = Instantiate<Ground>();
		}
		else if (str == "DOOR") {
			obj = Instantiate<Door>();
		}
		else if (str == "DANCER") {
			obj = Instantiate<Dancer>();
		}
		else {
			assert(false);
		}
		float x = csv->GetFloat(i, 1);
		float y = csv->GetFloat(i, 2);
		float z = csv->GetFloat(i, 3);
		obj->SetPosition(x, y, z);
	}
	//Instantiate<Ground>();
	//Door * door = Instantiate<Door>();
	//door->SetPosition(40, 0, -40);
	//// const�́A���������֎~�Ƃ����Ӗ�
	//const VECTOR3 dancerPos[] = {
	//	VECTOR3(0,0,20),
	//	VECTOR3(20,0,0),
	//	VECTOR3(-20,0,-20),
	//	VECTOR3(-25,0,-25),
	//	VECTOR3(30,0,-25),
	//};
	//for (VECTOR3 p : dancerPos) {
	//	Dancer* d = Instantiate<Dancer>();
	//	d->SetPosition(p);
	//}
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void PlayScene::Draw()
{
}
