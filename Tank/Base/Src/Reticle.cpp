#include "Reticle.h"

Reticle::Reticle()
{
	image = new CSpriteImage("Data/Image/scope.png");
	sprite = new CSprite();
}

Reticle::~Reticle()
{
}

void Reticle::Draw()
{
	int x = GameDevice()->m_pD3D->m_dwWindowWidth / 2;
	int y = GameDevice()->m_pD3D->m_dwWindowHeight / 2;
	                    /*画面位置*/  /*画像の左上*/ /*画像の右下*/
	sprite->Draw(image, x-16, y-16,      0, 0,           32, 32       );
}
