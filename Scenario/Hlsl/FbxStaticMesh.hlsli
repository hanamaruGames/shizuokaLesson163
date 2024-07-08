// -----------------------------------------------------------------------
// 
// �e�����X�^�e�B�b�N���b�V���̃V�F�[�_�[
// 
//                                                              2021.6.11
//                                                    FbxStaticMesh.hlsli
// -----------------------------------------------------------------------

// �O���[�o���ϐ�	
Texture2D g_Texture: register(t0);   //�e�N�X�`���[�� ���W�X�^�[t(n)
Texture2D g_NormalTexture : register(t1);   // �@���}�b�v
Texture2D g_SpecularTexture : register(t3);   // �X�y�L�����[�}�b�v

SamplerState g_samLinear : register(s0);//�T���v���[�̓��W�X�^�[s(n)


//  (�R���X�^���g�o�b�t�@)
cbuffer global : register(b0)               // -- 2020.12.15
{
	matrix g_mW;          // ���[���h�s��
	matrix g_mWVP;        // ���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_LightDir;    // ���C�g�̕����x�N�g��
	float4 g_EyePos;      // ���_�ʒu
	float4 g_Diffuse;     // �f�B�t���[�Y�F	
	float4 g_DrawInfo;    // �e����(�g���Ă��Ȃ�)   // -- 2020.12.15
};

// �}�e���A���J���[�̃R���X�^���g�o�b�t�@
cbuffer global : register(b3)               // -- 2020.12.15
{
	float4 g_MatDiffuse = float4(0.5, 0.5, 0.5, 1);   // �f�B�t���[�Y�F	
	float4 g_MatSpecular = float4(0, 0, 0, 0);        // �X�y�L�����F
};

// �o�[�e�b�N�X�V�F�[�_�[�̏o�͍\����
// (�s�N�Z���V�F�[�_�[�̓��͂ƂȂ�)
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
	float4 lightTangentSpace : TEXCOORD1; // �ڋ�Ԃɕϊ����ꂽ���C�g�x�N�g��

	float4 PosWorld:TEXCOORD2;
};

// �ڋ�ԍs��̋t�s����Z�o
float4x4 InverseTangentMatrix(float3 tangent, float3 binormal, float3 normal)
{
	tangent = normalize(mul(tangent, (float3x3)g_mW));     // �ڐ������[���h�ϊ�����    // -- 2021.6.11
	binormal = normalize(mul(binormal, (float3x3)g_mW));   // �]�@�������[���h�ϊ�����
	normal = normalize(mul(normal, (float3x3)g_mW));       // �@�������[���h�ϊ�����

	float4x4 mat = { float4(tangent, 0.0f),
		float4(binormal, 0.0f),
		float4(normal, 0.0f),
		{ 0.0f, 0.0f, 0.0f, 1.0f } };

	return transpose(mat); // �]�u�s��ɂ���
}

//
// �X�y�L�����̌v�Z
//
float4 CalcSpecular(float4 Color, float4 texSpecular, float4 PosWorld, float3 Normal, float NMbright)
{
	if (texSpecular.w == 1.0f) // �X�y�L�����}�b�v�����邩
	{
		// �n�[�t�x�N�g�����g�����X�y�L�����̌v�Z
		float3 ViewDir = normalize( g_EyePos.xyz - PosWorld.xyz);  // PosWorld����g_EyePos�֌����������x�N�g���i���K���j
		float3 HalfVec = normalize( g_LightDir.xyz + ViewDir);     // �n�[�t�x�N�g���i�����x�N�g���ƌ����x�N�N�g���̒��ԃx�N�g���j�i���K���j

		float Gloss = 4;   // ����x�W���B�w���l���傫���قǃX�y�L���������������Ȃ�B�����ł�4�Ƃ��Ă���B
		float Spec = 2.5f * pow( saturate(dot(HalfVec, Normal)), Gloss );   // �X�y�L�����̌v�Z�B��ŃX�y�L�����}�b�v�l���|���邽��2.5�{�ɂ��Ă���
		Color.rgb = saturate(Color.rgb + Spec * texSpecular.rgb * NMbright);   // �X�y�L�����ɃX�y�L�����}�b�v�l���|���ăJ���[�ɉ�����B

	}
	return Color;
}

//
// �o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float3 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD, float3 Tangent : TANGENT, float3 Binormal : BINORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 Pos4 = float4(Pos, 1);
	float4x4 inverseTangentMat;

	Out.Pos = mul(Pos4, g_mWVP);  // ���_�����[���h�E�r���[�E�v���W�F�N�V�����ϊ�����

	// �ڐ��̌v�Z
	// �ڋ�ԍs��̋t�s����Z�o
	inverseTangentMat = InverseTangentMatrix(normalize(Tangent), normalize(Binormal), normalize(Normal));

	// ���C�g�x�N�g����ڋ�ԏ�ɕϊ�
	Out.lightTangentSpace = mul(float4(g_LightDir.xyz, 1.0), inverseTangentMat);

	// �@�������[���h�ϊ�����
	Out.Normal = normalize(mul(Normal, (float3x3)g_mW));

	// �e�N�X�`�����W�͂��̂܂܏o��
	Out.Tex = Tex;

	Out.PosWorld = mul(Pos4, g_mW);  // ���_�����[���h�ϊ�����

	return Out;
}

//
// �s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT In) : SV_Target
{
	float4 Color;      // �ŏI�o�͒l
	uint width, height;
	g_Texture.GetDimensions(width, height);  // �f�B�t���[�Y�e�N�X�`���̃T�C�Y�𓾂�  // -- 2020.12.15

	// �e�N�X�`���J���[
	float4 texColor;
	float4 texSpecular;

	if (width == 0)  // �f�B�t���[�Y�e�N�X�`�����Ȃ��Ƃ�  // -- 2020.12.15
	{
		texColor = g_MatDiffuse;      // -- 2020.12.15
		texSpecular = g_MatSpecular;      // -- 2020.12.15
	}
	else {
	    texColor = g_Texture.Sample(g_samLinear, In.Tex);
	    texSpecular = g_SpecularTexture.Sample(g_samLinear, In.Tex);
	}
	float4 texNormal = g_NormalTexture.Sample(g_samLinear, In.Tex);

	// �����̕������ݒ肳��Ă��邩�ǂ����̃`�F�b�N
	if (g_LightDir.x == 0 && g_LightDir.y == 0 && g_LightDir.z == 0)
	{
		// ���̕�����0,0,0�̏ꍇ�͌��̌v�Z�����Ȃ��ŕ`��
		Color.rgb = texColor.rgb * g_Diffuse.rgb;
		Color.a = texColor.a - (1 - g_Diffuse.a); 
	}
	else {
		// ���̕������ݒ肳��Ă���ꍇ
		if (texNormal.w == 1.0f) {   // �@���}�b�v�����邩�ǂ����`�F�b�N

			// �@���}�b�v������ꍇ�͖@���}�b�s���O�t���̃��C�e�B���O
			// �@���}�b�s���O�̖��邳�v�Z
			float4 normalVector = normalize( 2.0 * texNormal - 1.0f);  // �@���}�b�v���x�N�g���֕ϊ�(�@���}�b�v�̐F��0�`1.0�B�����-1.0�`1.0�̃x�N�g���͈͂ɂ���)
			float NMbright = max(0, dot( normalVector, normalize(In.lightTangentSpace)));// ���C�g�̌����Ɩ@���}�b�v�̖@���ƂŖ��x�Z�o�B�}�C�i�X��0�ɕ␳�B

			// �����̕����v�Z
			float halflamb = max(0, dot(normalize(In.Normal), g_LightDir.xyz));
			halflamb = halflamb * 0.6f + 0.7f;   // ������Ɩ���߂�

			// �@���ƌ������������������ŏI�I�ȐF�̌v�Z
			Color.rgb = texColor.rgb * NMbright * halflamb * g_Diffuse.rgb;

			// �X�y�L�����̌v�Z
			Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, In.Normal, NMbright).rgb;
			// �����F�̌v�Z
			Color.a = texColor.a - (1 - g_Diffuse.a); 
		}
		else {

			// �@���}�b�v���Z�b�g����Ă��Ȃ��ꍇ�́A�n�[�t�����o�[�g�ŕ`��
			float halflamb = max(0, dot(normalize(In.Normal), g_LightDir.xyz));
			//halflamb = halflamb * 0.4f + 0.7f;
			halflamb = halflamb * 0.7f + 0.6f;      // -- 2021.6.11

			// �������������������ŏI�I�ȐF�̌v�Z
			Color.rgb = texColor.rgb * halflamb * g_Diffuse.rgb;

			// �X�y�L�����̌v�Z
			Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, In.Normal, 1).rgb;
			// �����F�̌v�Z
			Color.a = texColor.a - (1 - g_Diffuse.a);
		}
	}
	return saturate(Color);  // �ŏI�o�͒l��0�`�P�͈̔͂ɐ�������
}
