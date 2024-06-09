// -----------------------------------------------------------------------
// 
// �V���v���ȃV�F�[�_�[
// 
//                                                              2021.1.11
//                                                           Simple.hlsli
// -----------------------------------------------------------------------

// �O���[�o���ϐ�	
Texture2D g_Texture: register(t0);       // �e�N�X�`���[�� ���W�X�^�[t(n)
SamplerState g_samLinear : register(s0);  // �T���v���[�̓��W�X�^�[s(n)

// �R���X�^���g�o�b�t�@
cbuffer global
{
	matrix g_mW;          // ���[���h�s��
	matrix g_mWVP;        // ���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_LightDir;    // ���C�g�̕����x�N�g��
	float4 g_EyePos;      // ���_�ʒu
	float4 g_Diffuse;     // �f�B�t���[�Y�F	
	float4 g_DrawInfo;    // �e����(�g���Ă��Ȃ�)   // -- 2020.12.15
};

// �o�[�e�b�N�X�V�F�[�_�[�̏o�͍\����
// (�s�N�Z���V�F�[�_�[�̓��͂ƂȂ�)
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD;
};

//
// �o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float3 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 Pos4 = float4(Pos, 1);
	Out.Pos = mul(Pos4, g_mWVP);           // ���_�����[���h�E�r���[�E�v���W�F�N�V�����ϊ�����
	Normal = mul(Normal, (float3x3)g_mW);  // �@�������[���h�ϊ�����
	Normal = normalize(Normal);            // �@���𐳋K������

	if (g_LightDir.x == 0 && g_LightDir.y == 0 && g_LightDir.z == 0)   // �����̃x�N�g�����w�肵�Ȃ��Ƃ�
	{
		Out.Color = g_Diffuse;
	}else{
		Out.Color.rgb = g_Diffuse.rgb * (dot(Normal, g_LightDir.xyz) * 0.5f + 0.7f);  //���̎��̓����o�[�g�̗]����
		Out.Color.a   = g_Diffuse.a;   // �����x�̓f�B�t���[�Y�F�����̂܂܎g�p����
	}
	Out.Tex = Tex;     // �e�N�X�`�����W�͂��̂܂܏o��

	return Out;
}

//
// �s�N�Z���V�F�[�_�[
//
// �E�e�N�X�`���̗L�薳���𔻒f������@�Ɉȉ���2�̕��@������
//   �@�@g_DrawInfo.x�����Ĕ��f����B1:�e�N�X�`���L�� �Ȃ�
//   �A�@�e�N�X�`���̃T�C�Y��GetDimensions()�֐��Œ��ׁA�T�C�Y���O�Ȃ�e�N�X�`������
//   �����ł́A�A�̕��@���Ƃ��Ă���
//
float4 PS(VS_OUTPUT In) : SV_Target
{
	uint width, height;
	g_Texture.GetDimensions(width, height);  // �e�N�X�`���̃T�C�Y�𓾂�
	float4 Color;

	if (width != 0)               // -- 2020.12.15
	{   // �e�N�X�`��������Ƃ�
		float4 texColor = g_Texture.Sample(g_samLinear, In.Tex);  // Sample�֐��ɃT���v���[�ƃe�N�X�`���[���W��n��
		Color.rgb = texColor.rgb * In.Color.rgb;                   // �e�N�X�`���F�Ɍ����F����������
		Color.a = saturate(texColor.a - (1 - In.Color.a));         // �����x�̓f�B�t���[�Y�F����������
	}
	else {
		// �e�N�X�`�����Ȃ��Ƃ�
		Color = In.Color;
	}
	return Color;
}
