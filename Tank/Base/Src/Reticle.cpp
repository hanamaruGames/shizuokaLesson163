#include "Reticle.h"

Reticle::Reticle()
{
	image = new CSpriteImage("Data/Image/scope.png");
	sprite = new CSprite();
	transform.position = VECTOR3(0, 0, 0);
}

Reticle::~Reticle()
{
}

void Reticle::Draw()
{
	ImGui::Begin("Reticle");
	ImGui::InputFloat3("POS", &transform.position.x);
	ImGui::End();

	int x = transform.position.x + GameDevice()->m_pD3D->m_dwWindowWidth / 2;
	int y = transform.position.y + GameDevice()->m_pD3D->m_dwWindowHeight / 2;
	                    /*画面位置*/  /*画像の左上*/ /*画像の右下*/
	sprite->Draw(image, x-16, y-16,      0, 0,           32, 32       );
}

VECTOR3 Reticle::GetWorldPos()
{
	MATRIX4X4 mView = GameDevice()->m_mView;
	MATRIX4X4 mInv = XMMatrixInverse(nullptr, mView);
	// ↑ Viewの逆行列
	// 今のレティクルを座標にする
	float max = GameDevice()->m_pD3D->m_dwWindowWidth / 2.0f;
	VECTOR3 cur = VECTOR3(transform.position.x / max * 20.0f,
		-transform.position.y / max * 20.0f, 33.0f);
	return cur * mInv;
}
