// -----------------------------------------------------------------------
// 
// �G�t�F�N�g�̃V�F�[�_�[
// 
//                                                              2021.1.11
//                                                         Effect3D.hlsli
// -----------------------------------------------------------------------

// �O���[�o��
Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

// �R���X�^���g�o�b�t�@
cbuffer global
{
	matrix g_WVP;       // ���[���h����ˉe�܂ł̕ϊ��s��
	matrix g_W;         // ���[���h�ϊ��s��    // -- 2017.8.25
	matrix g_V;         // �r���[�ϊ��s��
	matrix g_P;         // �ˉe�ϊ��s��
	float2 g_UVOffset;  // �e�N�X�`�����W�̃I�t�Z�b�g
	float2 g_UVScale;   // �e�N�X�`�����W�̊g�k    // -- 2019.7.16
	float  g_Alpha;     // �����x
	float  g_Size;      // �p�[�e�B�N���̃T�C�Y  // -- 2018.8.23
	float2 g_Dummy;
};

// �\����
struct GS_INPUT
{
	float4 Pos : SV_POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

// ----------------------------------------------------------------------------
//
// �o�[�e�b�N�X�V�F�[�_�[ �r���{�[�h
//
// ----------------------------------------------------------------------------
PS_INPUT VS_BILL(float4 PosB : POSITION, float2 UV : TEXCOORD)
{
	PS_INPUT Out = (PS_INPUT)0;
	Out.Pos = mul(PosB, g_WVP);
	Out.UV = UV + g_UVOffset;

	return Out;
}

// ----------------------------------------------------------------------------
//
// �o�[�e�b�N�X�V�F�[�_�[ �r���{�[�h ���b�V��
//
// ----------------------------------------------------------------------------
PS_INPUT VS_BILLMESH(float3 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD, float3 Tangent : TANGENT, float3 Binormal : BINORMAL)
{
	PS_INPUT Out = (PS_INPUT)0;
	float4 PosB = float4(Pos, 1);
	Out.Pos = mul(PosB, g_WVP);
	Out.UV = Tex * g_UVScale + g_UVOffset;

	return Out;
}

// ----------------------------------------------------------------------------
//
// �o�[�e�b�N�X�V�F�[�_�[�@�p�[�e�B�N��
//
// ----------------------------------------------------------------------------
GS_INPUT VS_POINT(float4 Pos : POSITION)
{
	GS_INPUT Out = (GS_INPUT)0;
	Out.Pos = Pos;       // �����Œ��_��ϊ����Ȃ��B�Ȃ��Ȃ烂�f�����W�̂܂܂ŃW�I���g���V�F�[�_�[�ɓn����������B
	Out.Pos.w = 1.0f;    // ���_�S�Ă�w�l��1.0f�Ƃ���
	return Out;
}
// ----------------------------------------------------------------------------
//
// �W�I���g���V�F�[�_�[
//
//   �|���S���̉E����\�ʂƂ���      // -- 2024.3.23
//
// ----------------------------------------------------------------------------
[maxvertexcount(4)]
void GS_POINT(point GS_INPUT In[1], inout TriangleStream <PS_INPUT> TriStream)
{

	//  �g���C�A���O���X�g���b�v�Ō������̂S�p�`�|���S�����쐬����B
	//  �i���_��������������Ƃy�́{���������_���������߁j
	//  �P�ӂ�g_Size��4�p�`�B���_��4�p�`�̒��S�ɗ���B  // - 2018.8.23
	PS_INPUT p = (PS_INPUT)0;

	p.Pos = In[0].Pos + float4(g_Size / 2, g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(-g_Size / 2, g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(0, 0);    // -- 2024.3.23
	//p.UV = float2(1, 0);
	TriStream.Append(p);

	p.Pos = In[0].Pos + float4(-g_Size / 2, g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(g_Size / 2, g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(1, 0);      // -- 2024.3.23
	//p.UV = float2(0, 0);
	TriStream.Append(p);

	p.Pos = In[0].Pos + float4(g_Size / 2, -g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(-g_Size / 2, -g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(0, 1);    // -- 2024.3.23
	//p.UV = float2(1, 1);
	TriStream.Append(p);

	p.Pos = In[0].Pos + float4(-g_Size / 2, -g_Size / 2, 0, 0);    // -- 2024.3.23
	//p.Pos = In[0].Pos + float4(g_Size / 2, -g_Size / 2, 0, 0);
	p.Pos = mul(p.Pos, g_W);
	p.Pos = mul(p.Pos, g_V);
	p.Pos = mul(p.Pos, g_P);
	p.UV = float2(1, 1);    // -- 2024.3.23
	//p.UV = float2(0, 1);
	TriStream.Append(p);

	TriStream.RestartStrip();
}
// ----------------------------------------------------------------------------
//
// �s�N�Z���V�F�[�_�[
//
// ----------------------------------------------------------------------------
float4 PS(PS_INPUT In) : SV_Target
{
	float4 color;
	color = g_Texture.Sample(g_Sampler, In.UV);
	color.a -= (1 - g_Alpha);
	return saturate(color);
}