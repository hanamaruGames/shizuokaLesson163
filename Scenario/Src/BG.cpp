#include "BG.h"

BG::BG()
{
	image = new CSpriteImage(GameDevice()->m_pShader, 
								"Assets/BG/BG_road.jpg");
}

BG::~BG()
{
}

void BG::Update()
{
}

void BG::Draw()
{
	CSprite* spr = new CSprite();
	int x = image->m_dwImageWidth;
	int y = image->m_dwImageHeight;
	spr->Draw(image, 0, 0, 0, 0, x, y);
}
