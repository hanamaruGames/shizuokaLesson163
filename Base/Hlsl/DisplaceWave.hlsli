// -----------------------------------------------------------------------
// 
// �g�̃f�B�X�v���[�X�����g�}�b�s���O�̃V�F�[�_�[
//                                                              2021.2.4
//  (�g�ɉ����A�n�ⓙ�̏����ɑΉ�����)
//                                                     DisplaceWave.hlsli
// -----------------------------------------------------------------------

// �O���[�o��
Texture2D g_Texture: register(t0);         // �e�N�X�`���[�� ���W�X�^�[t(n)     // -- 2021.2.4
Texture2D g_NormalTexture: register(t1);   // �@���}�b�v
Texture2D g_SpecularTexture: register(t3); // �X�y�L�����[�}�b�v

SamplerState g_samPoint : register(s0);

// �O���[�o��
// ���[���h����ˉe�܂ł̕ϊ��s��E��(b0)
cbuffer global_0 : register(b0)
{
	matrix g_mW;          // ���[���h�s��
	matrix g_mWVP;        // ���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_LightDir;    // ���C�g�̕����x�N�g��
	float4 g_EyePos;      // ���_�ʒu
	float4 g_Diffuse;     // �f�B�t���[�Y�F	
	float4 g_DrawInfo;    // �e����(�g���Ă��Ȃ�)   // -- 2020.12.15
};
// �f�B�X�v���[�X�����g�}�b�s���O�p�̊e��f�[�^��n��  // -- 2020.1.24
cbuffer global_1: register(b1)
{
	float3 g_EyePosInv;     // ���f�����猩�����_�ʒu
	float  g_fMinDistance;  // �|���S�������̍ŏ�����
	float  g_fMaxDistance;  // �|���S�������̍ő召����
	int    g_iMaxDevide;    // �����ő吔
	float2 g_Height;        // �g�̍���
	float4 g_WaveMove;      // �g�̈ړ���
	float4 g_Specular;      // ���ʔ���
};

struct VS_OUTPUT
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal: NORMAL;
};

struct HS_CONSTANT_OUTPUT
{
	float factor[3]    : SV_TessFactor;
	float inner_factor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float3 Pos  : POSITION;
	float2 Tex  : TEXCOORD0;
};

struct DS_OUTPUT
{
	float4 Pos  : SV_POSITION;
	float2 Tex  : TEXCOORD0;
	float4 PosWorld : TEXCOORD1;        // -- 2019.4.14
};
//
// �X�y�L�����̌v�Z     // -- 2021.2.4
//
float4 CalcSpecular(float4 Color, float4 texSpecular, float4 PosWorld, float3 Normal, float NMbright)
{
	if (texSpecular.w == 1.0f) // �X�y�L�����}�b�v�����邩
	{
		// �n�[�t�x�N�g�����g�����X�y�L�����̌v�Z
		float3 ViewDir = -normalize(g_EyePos.xyz - PosWorld.xyz);  // PosWorld����g_EyePos�֌����������x�N�g���i���K���j  // �x�N�g�����t�ɂ���   // -- 2021.2.4
		float3 HalfVec = normalize(g_LightDir.xyz + ViewDir);     // �n�[�t�x�N�g���i�����x�N�g���ƌ����x�N�N�g���̒��ԃx�N�g���j�i���K���j

		float Gloss = 4;   // ����x�W���B�w���l���傫���قǃX�y�L���������������Ȃ�B�����ł�4�Ƃ��Ă���B
		float Spec = 2.5f * pow(saturate(dot(HalfVec, Normal)), Gloss);   // �X�y�L�����̌v�Z�B��ŃX�y�L�����}�b�v�l���|���邽��2.5�{�ɂ��Ă���
		Color.rgb = saturate(Color.rgb + Spec * texSpecular.rgb * NMbright);   // �X�y�L�����ɃX�y�L�����}�b�v�l���|���ăJ���[�ɉ�����B

	}
	return Color;
}


// ----------------------------------------------------------------------------
// 
// �o�[�e�b�N�X�V�F�[�_�[
// 
// ----------------------------------------------------------------------------
VS_OUTPUT VS( float3 pos :POSITION ,float2 uv :TEXCOORD0,float3 normal :NORMAL)
{
	VS_OUTPUT Out;

	Out.Pos = pos;

	Out.Tex = uv;
	Out.Tex += g_WaveMove.xy;   // �g��XY�����̕ω��ʂ��e�N�X�`�����W�ɉ�����
	Out.Normal = normal;        // -- 2021.2.4

	return Out;
}
// ----------------------------------------------------------------------------
//
// �n���V�F�[�_�[�R���X�^���g
//
// ----------------------------------------------------------------------------
HS_CONSTANT_OUTPUT HSConstant( InputPatch<VS_OUTPUT, 3> ip, uint pid : SV_PrimitiveID )
{
	HS_CONSTANT_OUTPUT Out;
	float divide = 0;

	float distance = length(g_EyePosInv-ip[0].Pos);  //	  ���̃|���S��(3���_��0�Ԗ�)���王�_�ʒu�܂ł̋������͂���

	//float distance = length(g_EyePosInv);	// World���王�_�ʒu�܂ł̋������͂���

	if(distance<g_fMinDistance) distance = g_fMinDistance;  // �ŏ������ƍő勗���̊ԂɂȂ�悤�ɃN�����v
	if(distance>g_fMaxDistance) distance = g_fMaxDistance;

	// x�́A�������ő勗���Ɠ����ꍇ��1.0�ɁA�ŏ������Ɠ����ꍇ��0.0�ɁA���̒��Ԃ̏ꍇ��0.0�`1.0�ɂȂ�B
	float x = (distance-g_fMinDistance) / (g_fMaxDistance-g_fMinDistance);
	// ��L�̌v�Z�ŁAx�͉��߂��t�ƂȂ��Ă��邽�߁A1�̕␔(1-x)���Ƃ�B
	divide = (1-x) * g_iMaxDevide+1;

	//divide = 16;      // �������̐؂�ڂŃ|���S���̋��ڂ��C�ɂȂ�Ƃ��͌Œ蕪���Ƃ���

	Out.factor[0] = divide;   // ��������factor�ɐݒ肷��
	Out.factor[1] = divide;
	Out.factor[2] = divide;

	Out.inner_factor = divide;

	return Out;
}
// ----------------------------------------------------------------------------
//
// �n���V�F�[�_�[
//
// ----------------------------------------------------------------------------
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HSConstant")]
HS_OUTPUT HS( InputPatch<VS_OUTPUT, 3> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID )
{
	HS_OUTPUT Out;
	Out.Pos = ip[cpid].Pos;
	Out.Tex = ip[cpid].Tex;

	return Out;
}
// ----------------------------------------------------------------------------
//
// �h���C���V�F�[�_�[
//
// ----------------------------------------------------------------------------
[domain("tri")]
DS_OUTPUT DS( HS_CONSTANT_OUTPUT In, float3 DomLoc : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch )
{
	DS_OUTPUT Out;

	// �R�̃R���g���[���|�C���g�̃e�N�X�`�����W�ɎO�p�`�d�S�ʒu�ix,y,z)���|���āA���ۂ̃e�N�X�`�����W�����߂Ă���B
	// �i���̂Ƃ��A0�Ԗڂ̃e�N�X�`�����W�̏d�S�ʒu��DomLoc.x�ɁA1�Ԗڂ�DomLoc.y�ɁA2�Ԗڂ�DomLoc.z�ɑΉ�����j
	Out.Tex = patch[0].Tex*DomLoc.x + patch[1].Tex*DomLoc.y + patch[2].Tex*DomLoc.z;

	// Height�e�N�X�`���[���g�p���āA��قǋ��߂��e�N�X�`�����W�ʒu�̒��_�̍��������߂�
	// (�m�[�}���e�N�X�`����Hight�e�N�X�`���[�Ƃ��Ďg�p����)
	float4 texHeight = g_NormalTexture.SampleLevel( g_samPoint, Out.Tex, 0 );

	// �R�̃R���g���[���|�C���g�̍��W�ɎO�p�`�d�S�ʒu�ix,y,z)���|���āA���_���W�����߂Ă���B
	float3 pos = patch[0].Pos*DomLoc.x + patch[1].Pos*DomLoc.y + patch[2].Pos*DomLoc.z;

	// ���_�̍�����������(y������)
	pos.y += (texHeight.x + texHeight.y + texHeight.z) / 3 * g_Height.y;
	float4 pos4 = float4(pos, 1);
	Out.Pos = mul(pos4, g_mWVP);     // ���[���h�E�r���[�E�v���W�F�N�V�����ϊ�������

	Out.PosWorld = mul(pos4, g_mW);  // ���[���h�ϊ�������

	return Out;
}
// ----------------------------------------------------------------------------
//
// �s�N�Z���V�F�[�_
//
// ----------------------------------------------------------------------------
float4 PS( DS_OUTPUT In ) : SV_Target 
{
	float4 Color;      // �ŏI�o�͒l
	uint width, height;
	g_Texture.GetDimensions(width, height);  // �f�B�t���[�Y�e�N�X�`���̃T�C�Y�𓾂�     // -- 2021.2.4

	float4 texSpecular = g_SpecularTexture.Sample(g_samPoint, In.Tex);
	float3 Normal = -normalize(g_NormalTexture.SampleLevel(g_samPoint,In.Tex,0).xyz * 2 - 1);   // �m�[�}���}�b�v�e�N�X�`������-�@���𓾂�
	float  NMbright = saturate(dot(Normal, g_LightDir.xyz));   // �m�[�}���}�b�v�̌��v�Z

	if (width == 0)  // �f�B�t���[�Y�e�N�X�`�����Ȃ��Ƃ�     // -- 2021.2.4
	{
		Color = g_Diffuse;
	}
	else {
		Color = g_Texture.Sample(g_samPoint, In.Tex);
	}

	if (texSpecular.w == 1.0f) // �X�y�L�����}�b�v�����邩     // -- 2021.2.4
	{
		// �X�y�L�����̌v�Z
		Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, Normal, NMbright).rgb;
	}
	else {
		float3 ViewDir = normalize(g_EyePosInv - In.Pos.xyz);
		float3 Reflect = normalize(2 * NMbright * Normal - g_LightDir.xyz);    // ���ʔ��˂̌��v�Z
		float4 Spec = pow(saturate(dot(Reflect, ViewDir)), 8); // �X�y�L�����̌v�Z

		Color.rgb = Color.rgb * NMbright + g_Specular.rgb * Spec.rgb * 1.3f; // �X�y�L����������
	}

	return saturate(Color);  // �ŏI�o�͒l��0�`�P�͈̔͂ɐ�������

}

