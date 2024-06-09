#include "PlayScene.h"
#include "Player.h"
#include "Ground.h"
#include "Door.h"
#include "Dancer.h"

PlayScene::PlayScene()
{
	Instantiate<Player>();
	Instantiate<Ground>();
	Door * door = Instantiate<Door>();
	door->SetPosition(40, 0, -40);
	// constÇÕÅAèëÇ´ä∑Ç¶ã÷é~Ç∆Ç¢Ç§à”ñ°
	const VECTOR3 dancerPos[] = {
		VECTOR3(0,0,20),
		VECTOR3(20,0,0),
		VECTOR3(-20,0,-20),
		VECTOR3(-25,0,-25),
		VECTOR3(30,0,-25),
	};
	for (VECTOR3 p : dancerPos) {
		Dancer* d = Instantiate<Dancer>();
		d->SetPosition(p);
	}
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
