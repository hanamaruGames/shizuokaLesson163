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
	// テキストファイルの読み方
	// ファイル＝ストリーム
#if 0
	std::ifstream ifs("Data/map00.csv"); // ファイルを開く
	if (ifs) { // ファイルが開けている
		std::string str;
		std::getline(ifs, str); // ここで１行読む
		std::getline(ifs, str); // ここで１行読む
		// ここに2行目のデータが入っている
		// "PLAYER,0,0,0"が入っている。[,]で切って４つのデータに分ける
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == ',') {
				// 文字列を切り取るには、str.substr(/*何文字目から*/, /*何文字*/);
				// ２つめの引数を指定しないと、最後までということになる
				std::string before = str.substr(0, i); // ,の前"PLAYER"が入るはず
				std::string after = str.substr(i+1); // ,の後ろ"0,0,0"が入るはず
			}
		}
	}
	ifs.close(); // ファイルを閉じる
#endif
	CsvReader* csv = new CsvReader("Data/map00.csv");
	assert(csv->GetLines() > 0);
	for (int i = 1; i < csv->GetLines(); i++) { // １行ずつ読む
		std::string str = csv->GetString(i, 0); // PLAYERが入る
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
	//// constは、書き換え禁止という意味
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
