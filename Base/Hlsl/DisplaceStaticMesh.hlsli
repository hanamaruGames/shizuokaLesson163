// -----------------------------------------------------------------------
// 
// �X�^�e�B�b�N���b�V���@�f�B�X�v���[�X�����g�}�b�s���O�̃V�F�[�_�[
// 
//                                                              2021.6.11
//                                               DisplaceStaticMesh.hlsli
// -----------------------------------------------------------------------

// �O���[�o��
Texture2D g_Texture: register(t0);         // �e�N�X�`���[�� ���W�X�^�[t(n)
Texture2D g_NormalTexture: register(t1);   // �@���}�b�v
Texture2D g_HeightTexture: register(t2);   // �n�C�g�}�b�v
Texture2D g_SpecularTexture : register(t3);   // �X�y�L�����[�}�b�v

SamplerState g_samLinear : register(s0);

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
	float2 g_Height;        // �f�B�X�v���[�X�����g�}�b�s���O���̐���グ����
	float4 g_WaveMove;      // �g�̈ړ���(�g�̏������̂�)
	float4 g_Specular;      // ���ʔ���(�g�̏������̂�)
};

// �}�e���A���J���[�̃R���X�^���g�o�b�t�@
cbuffer global : register(b3)               // -- 2020.12.15
{
	float4 g_MatDiffuse = float4(0.5, 0.5, 0.5, 1);   // �f�B�t���[�Y�F	
	float4 g_MatSpecular = float4(0, 0, 0, 0);        // �X�y�L�����F
};

struct VS_OUTPUT
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct HS_CONSTANT_OUTPUT
{
	float factor[3]  : SV_TessFactor;
	float inner_factor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float3 Pos    : POSITION;
	float2 Tex    : TEXCOORD0;
	float3 Normal : NORMAL;    // -- 2018.4.16
};

struct DS_OUTPUT
{
	float4 Pos     : SV_POSITION;
	float2 Tex     : TEXCOORD0;
	float3 Normal  : NORMAL;    // -- 2020.1.15
	float4 PosWorld:TEXCOORD2;
};
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

// ----------------------------------------------------------------------------
//
// �o�[�e�b�N�X�V�F�[�_�[
//
// ----------------------------------------------------------------------------
VS_OUTPUT VS(float3 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD, float3 Tangent : TANGENT, float3 Binormal : BINORMAL)
{
	VS_OUTPUT Out;

	Out.Pos   = Pos;     // ���_��ϊ������Ƀn���V�F�[�_�ɓn��
	Out.Tex   = Tex;
	Out.Normal= Normal;

	return Out;
}

// ----------------------------------------------------------------------------
//
// �n���V�F�[�_�[�́A�p�b�`�萔�֐��ƃR���g���[���|�C���g�V�F�[�_�̂Q�̃t�F�[�Y����Ȃ��Ă���
//
// �p�b�`�Ƃ�	�v���~�e�B�u�E�g�|���W�[ IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST)
// �ȂǂŎw�肳�����̂ŁA���̗�ł̓R���g���[���|�C���g���R�������p�b�`�Ƃ������ƂɂȂ�B
// �Ȃ��A�R���g���[���|�C���g�́A�ő�R�Q�܂Ŏw��ł���B
//
// ----------------------------------------------------------------------------
// �n���V�F�[�_�[�̃p�b�`�萔�t�F�[�Y(�p�b�`�萔�֐�)
// 
// ���̊֐��́A�w��p�b�`���ƂɂP��N������܂��B�i�����ł́A�R���g���[���|�C���g�R���ɋN�������j
// ���̎� ip �̓Y���̓R���g���[���|�C���g�i���_�j�̂O�Ԃ���Q�ԂƂȂ�܂�
// �������e�́A���_���璸�_�܂ł̋����ɉ����ĕ����������肵�܂�
// �R���_���ꂼ��̕�������߂�l�ŕԂ��܂�
// 
//  ����  InputPatch<VS_OUTPUT, 3> ip   ���_�i�o�[�e�b�N�X�j�V�F�[�_�[����w��p�b�`�i�����ł͂R��j���̒l���󂯎��
//        uint pid                      �p�b�`�̂h�c
// 
// ----------------------------------------------------------------------------
HS_CONSTANT_OUTPUT HSConstant( InputPatch<VS_OUTPUT, 3> ip, uint pid : SV_PrimitiveID )
{
	HS_CONSTANT_OUTPUT Out;
	float divide = 0;

	float distance = length(g_EyePosInv - ip[0].Pos);   // ���_�O�ԂƎ��_�Ƃ̋����𑪂�
	if(distance < g_fMinDistance) distance = g_fMinDistance;  // �ŏ������ƍő勗���̊ԂɂȂ�悤�ɃN�����v
	if(distance > g_fMaxDistance) distance = g_fMaxDistance;

	// x�́A�������ő勗���Ɠ����ꍇ��1.0�ɁA�ŏ������Ɠ����ꍇ��0.0�ɁA���̒��Ԃ̏ꍇ��0.0�`1.0�ɂȂ�B
	float x = (distance - g_fMinDistance) / (g_fMaxDistance - g_fMinDistance);
	// ��L�̌v�Z�ŁAx�͉��߂��t�ƂȂ��Ă��邽�߁A1�̕␔(1-x)���Ƃ�B
	divide = (1-x) * g_iMaxDevide + 1;   // ���������v�Z����

	// ��������factor�ɐݒ肷��
	Out.factor[0] = divide;
	Out.factor[1] = divide;
	Out.factor[2] = divide;

	Out.inner_factor = divide;

	return Out;
}
// ----------------------------------------------------------------------------
// �n���V�F�[�_�[�̃R���g���[���|�C���g�t�F�[�Y(�R���g���[���|�C���g�V�F�[�_)
// �i�����炪�n���V�F�[�_�[�̃V�F�[�_�[���ƂȂ�j
// 
// ���̊֐��́A�R���g���[���|�C���g�i���_�j���ƂɂP��N������܂��B
// 
//  ����
//  domain("tri")                    �h���C���^�C�v�̎w��iqiad:�l�p�h���C�� tri:�O�p�h���C�� isoline:���h���C���j
//  partitioning("integer")          �������͐��������������_�������w��
//  outputtopology("triangle_ccw")    �ŏI�I�ȕ�����̃|���S���ɂ��āitriangle_cw:���v��� triangle_ccw:�����v��� line:���|���S��)
//  outputcontrolpoints(3)           �n���V�F�[�_�[�̃R���g���[�� �|�C���g�t�F�[�Y���R�[�������񐔁i�����ł͂R��j
//  patchconstantfunc("HSConstant")  �Ή�����n���V�F�[�_�[�̃p�b�`�萔�֐���
// 
//  ����  InputPatch<VS_OUTPUT, 3> ip   ���_�i�o�[�e�b�N�X�j�V�F�[�_�[����w��p�b�`�i�����ł͂R��j���̒l���󂯎��
//        uint cpid                     �R���g���[���|�C���g�̔ԍ��i�Y���j
//        uint pid                      �p�b�`�̂h�c
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
	Out.Pos    = ip[cpid].Pos;
	Out.Tex    = ip[cpid].Tex;
	Out.Normal = ip[cpid].Normal;

	return Out;
}
// ----------------------------------------------------------------------------
// 
// �h���C���V�F�[�_�[
// 
//  �h���C���V�F�[�_�[�́A�e�Z���[�^�[�̕����ɂ���Đ������ꂽ���_���ɋN�����܂�
//  �h���C���V�F�[�_�[�́A������̎��ۂ̒��_���W���v�Z���܂��B 
//  �O�p�`�d�S�ʒu�iDomLoc)�͕������ 0 �` 1 �͈͓̔��̕⊮�W���ł��B
//  �R���g���[���|�C���g�̍��W���番����̒��_���W���v�Z���܂��B 
//  Hight�e�N�X�`���[���g�p���āA���_�̍������v�Z���܂��B
// 
//  ����
//  domain("tri")                           �h���C���^�C�v�̎w��iqiad:�l�p�h���C�� tri:�O�p�h���C�� isoline:���h���C���j
// 
//  ����  HS_CONSTANT_OUTPUT In             �n���V�F�[�_�[�萔�֐��̏o�͒l���󂯎��
//        float3 DomLoc                     ���Y���_�̈ʒu���i�O�p�`�̏d�S�ʒu�j�B�������ꂽ���_�̃p�b�`��̈ʒu�����߂邽�߂Ɏg�p����
//        OutputPatch<HS_OUTPUT, 3> patch   �S�ẴR���g���[���|�C���g�փA�N�Z�X�\
// 
// ----------------------------------------------------------------------------
[domain("tri")]
DS_OUTPUT DS( HS_CONSTANT_OUTPUT In, float3 DomLoc : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch )
{
	DS_OUTPUT Out;

	// �R�̃R���g���[���|�C���g�̃e�N�X�`�����W�ɎO�p�`�d�S�ʒu�ix,y,z)���|���āA���ۂ̃e�N�X�`�����W�����߂Ă���B
	// �i���̂Ƃ��A0�Ԗڂ̃e�N�X�`�����W�̏d�S�ʒu��DomLoc.x�ɁA1�Ԗڂ�DomLoc.y�ɁA2�Ԗڂ�DomLoc.z�ɑΉ�����j
	Out.Tex = patch[0].Tex*DomLoc.x + patch[1].Tex*DomLoc.y + patch[2].Tex*DomLoc.z;

	// Height�e�N�X�`���[���g�p���āA��قǋ��߂��e�N�X�`�����W�ʒu�̒��_�l�����߂�
	float4 texHeight = g_HeightTexture.SampleLevel(g_samLinear,Out.Tex,0 );

	// �R�̃R���g���[���|�C���g�̖@�����W�ɎO�p�`�d�S�ʒu�ix,y,z)���|���āA�@�����W�����߂�     // -- 2020.1.15
	// �i���̂Ƃ��A0�Ԗڂ̖@�����W�̏d�S�ʒu��DomLoc.x�ɁA1�Ԗڂ�DomLoc.y�ɁA2�Ԗڂ�DomLoc.z�ɑΉ�����j
	Out.Normal = normalize(patch[0].Normal*DomLoc.x + patch[1].Normal*DomLoc.y + patch[2].Normal*DomLoc.z);

	// �R�̃R���g���[���|�C���g�̍��W�ɎO�p�`�d�S�ʒu�ix,y,z)���|���āA���_���W�����߂Ă���B
	// �i���̂Ƃ��A0�Ԗڂ̒��_�̏d�S�ʒu��DomLoc.x�ɁA1�Ԗڂ�DomLoc.y�ɁA2�Ԗڂ�DomLoc.z�ɑΉ�����j
	float3 pos = patch[0].Pos*DomLoc.x + patch[1].Pos*DomLoc.y + patch[2].Pos*DomLoc.z;
	if (DomLoc.x != 0 && DomLoc.y != 0 && DomLoc.z != 0)   // �d�S�ʒu�����̒��_�ʒu�̂Ƃ��́A�������Ȃ��B�|���S���̐؂�ڂ��􂯂Ȃ�����
	{
		pos = pos + (Out.Normal * ((texHeight.x + texHeight.y + texHeight.z) / 3 * g_Height.y));  // ���_�̍�����������(�@��������)
	}
	float4 pos4 = float4(pos, 1);

	Out.Pos = mul(pos4,g_mWVP);  // ���[���h�E�r���[�E�v���W�F�N�V�����ϊ�������
	Out.Normal = normalize(mul(Out.Normal, (float3x3)g_mW));   // ���[���h�ϊ�������
	Out.PosWorld = mul(pos4, g_mW);  // ���[���h�ϊ�������

	return Out;
}
// ----------------------------------------------------------------------------
//
// �s�N�Z���V�F�[�_�[
//
// ----------------------------------------------------------------------------
float4 PS( DS_OUTPUT In ) : SV_Target
{
	float4 Color;      // �ŏI�o�͒l
	uint width, height;
	g_Texture.GetDimensions(width, height);  // �f�B�t���[�Y�e�N�X�`���̃T�C�Y�𓾂�  // -- 2020.12.15

	// �e�N�X�`���J���[
	float4 texCol;
	float4 texSpecular;

	if (width == 0)  // �f�B�t���[�Y�e�N�X�`�����Ȃ��Ƃ�  // -- 2020.12.15
	{
		texCol = g_MatDiffuse;      // -- 2020.12.15
		texSpecular = g_MatSpecular;      // -- 2020.12.15
	}else{
		texCol = g_Texture.Sample(g_samLinear, In.Tex);  // �f�B�t���[�Y�e�N�X�`��
		texSpecular = g_SpecularTexture.SampleLevel(g_samLinear, In.Tex, 0);  // �X�y�L�����e�N�X�`��
	}
	float4 texNM  = g_NormalTexture.SampleLevel(g_samLinear, In.Tex, 0);  // �m�[�}���e�N�X�`��

	// �����̕������ݒ肳��Ă��邩�ǂ����̃`�F�b�N
	if (g_LightDir.x == 0 && g_LightDir.y == 0 && g_LightDir.z == 0)
	{
		// ���̕�����0,0,0�̏ꍇ�͌��̌v�Z�����Ȃ��ŕ`��
		Color.rgb = texCol.rgb * g_Diffuse.rgb;
		Color.a = texCol.a - (1 - g_Diffuse.a);
	}
	else {
		// ���̕������ݒ肳��Ă���ꍇ
		if (texNM.w == 1.0f)   // �@���}�b�v�����邩�ǂ����`�F�b�N
		{
			// �@���}�b�v������ꍇ�͖@���}�b�s���O�t���̃��C�e�B���O
			// �@���}�b�s���O�̖��邳�v�Z
			float3 Normal = -(texNM.xyz * 2 - 1);  // �m�[�}���}�b�v�e�N�X�`������-�@���𓾂�
			float  NMbright = max(0, dot(Normal, normalize(g_LightDir.xyz))) * 0.9f + 0.4f;  // �m�[�}���}�b�v�̌��v�Z

			// �����̕����v�Z
			float halflamb = max(0, dot(normalize(In.Normal), normalize(g_LightDir.xyz)));  // �����o�[�g�̗]����
			halflamb = halflamb * 0.6f + 0.6f;   // �n�[�t�����o�[�g�B������Ɩ���߂�

			// �@���ƌ������������������ŏI�I�ȐF�̌v�Z
			Color.rgb = texCol.rgb * NMbright * halflamb * g_Diffuse.rgb;  // �e�N�X�`���F�Ƀm�[�}���}�b�v�ƃn�[�t�����o�[�g�A�g�U���ːF���d�˂�
			// �X�y�L�����̌v�Z
			Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, In.Normal, NMbright).rgb;

			// �����F�̌v�Z
			Color.a = texCol.a - (1 - g_Diffuse.a);   // �����x�̓e�N�X�`�������x�Ɋg�U���ːF�����x����������
		}
		else {
			// �@���}�b�v���Z�b�g����Ă��Ȃ��ꍇ�́A�n�[�t�����o�[�g�ŕ`��
			float halflamb = max(0, dot(normalize(In.Normal), g_LightDir.xyz));  // �}�C�i�X��0�ɕ␳
			//halflamb = halflamb * 0.5f + 0.8f;
			halflamb = halflamb * 0.7f + 0.6f;                     // -- 2021.6.11

			// �������������������ŏI�I�ȐF�̌v�Z
			Color.rgb = texCol.rgb * halflamb * g_Diffuse.rgb;
			// �X�y�L�����̌v�Z
			Color.rgb = CalcSpecular(Color, texSpecular, In.PosWorld, In.Normal, 1).rgb;

			// �����F�̌v�Z
			Color.a = texCol.a - (1 - g_Diffuse.a);
		}
	}
	return saturate(Color);  // �ŏI�o�͒l��0�`�P�͈̔͂ɐ�������
}

