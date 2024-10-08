#include "Particle.h"

namespace {
	struct Info {
		VECTOR3 pos;
		VECTOR2 uv;
	};
};

Particle::Particle()
{
	useAddBlend = false;
	texture = nullptr;
}

Particle::~Particle()
{
//	SAFE_DELETE(vertexBuffer);
//	SAFE_DELETE(texture);
}

void Particle::LoadTexture(std::string filename, const VECTOR2& _uv)
{
	SAFE_DELETE(texture);
	SAFE_DELETE(vertexBuffer);
	GameDevice()->m_pD3D->CreateShaderResourceViewFromFile(filename.c_str(), &texture);
	uvSize = _uv;

	static const Info verts[] = {
		{ VECTOR3(-0.5f, 0.5f, 0), VECTOR2(uvSize.x, 0) },
		{ VECTOR3(-0.5f, -0.5f, 0), VECTOR2(uvSize.x, uvSize.y) },
		{ VECTOR3(0.5f, 0.5f, 0), VECTOR2(0, 0) },
		{ VECTOR3(0.5f, -0.5f, 0), VECTOR2(0, uvSize.y) },
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(verts);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA init;
	init.pSysMem = verts;
	GameDevice()->m_pD3D->m_pDevice->CreateBuffer(&bd, &init, &vertexBuffer);
}

void Particle::Draw()
{
	if (texture == nullptr)
		return;

	CDirect3D* d3d = GameDevice()->m_pD3D;
	CShader* shd = GameDevice()->m_pShader;

	d3d->m_pDeviceContext->VSSetShader(shd->m_pEffect3D_VS_BILL, NULL, 0);
	d3d->m_pDeviceContext->PSSetShader(shd->m_pEffect3D_PS, NULL, 0);

	//�o�[�e�b�N�X�o�b�t�@���Z�b�g
	UINT stride = sizeof(Info);
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = vertexBuffer;   // �o�[�e�b�N�X�o�b�t�@
	d3d->m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	MATRIX4X4 world = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	world = world * GetLookatMatrix(transform.position, GameDevice()->m_vEyePt);

	if (useAddBlend)
	{
		UINT mask = 0xffffffff;
		d3d->m_pDeviceContext->OMSetBlendState(d3d->m_pBlendStateAdd, NULL, mask);
	}

	for (auto c : chips) {
		//�r���{�[�h�́A���_���������[���h�g�����X�t�H�[�������߂�
		// �`�撆�S�ʒu�̈ړ�������
		MATRIX4X4 off = XMMatrixTranslation(-c.offset.x, c.offset.y, -c.offset.z);

		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE pData;
		CONSTANT_BUFFER_EFFECT cb;
		ZeroMemory(&cb, sizeof(cb));

		if (SUCCEEDED(d3d->m_pDeviceContext->Map(shd->m_pConstantBufferEffect, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			MATRIX4X4 scale = XMMatrixScaling(c.scale, c.scale, c.scale);
			//���[���h�A�J�����A�ˉe�s��A�e�N�X�`���[�I�t�Z�b�g��n��
			cb.mWVP = XMMatrixTranspose(scale * off * world * GameDevice()->m_mView * GameDevice()->m_mProj);

			cb.vUVOffset.x = c.uv.x;
			cb.vUVOffset.y = c.uv.y;
			cb.vUVScale.x = 1;
			cb.vUVScale.y = 1;
			cb.fAlpha = c.alpha;

			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			d3d->m_pDeviceContext->Unmap(shd->m_pConstantBufferEffect, 0);
		}
		//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�[�Ŏg����
		d3d->m_pDeviceContext->VSSetConstantBuffers(0, 1, &shd->m_pConstantBufferEffect);
		d3d->m_pDeviceContext->PSSetConstantBuffers(0, 1, &shd->m_pConstantBufferEffect);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		d3d->m_pDeviceContext->IASetInputLayout(shd->m_pEffect3D_VertexLayout_BILL);
		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		d3d->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		d3d->m_pDeviceContext->PSSetSamplers(0, 1, &d3d->m_pSampleLinear);
		d3d->m_pDeviceContext->PSSetShaderResources(0, 1, &texture);
		//�v���~�e�B�u�������_�����O
		d3d->m_pDeviceContext->Draw(4, 0);
	}

	if (useAddBlend)   // ���Z�����F�w��
	{
		UINT mask = 0xffffffff;
		d3d->m_pDeviceContext->OMSetBlendState(d3d->m_pBlendStateTrapen, NULL, mask);
	}
}
