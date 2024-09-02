//=============================================================================
//		���b�V���̓ǂݍ��݂ƕ`��̃v���O����
//�@                                                  ver 3.3        2024.3.234
//
//		���b�V������
//
//      (���b�V���R���g���[���N���X�Ńe�N�X�`���̑����Ǘ����s��)
//
//
//																	FbxMesh.h
//=============================================================================
#pragma once

#include <list>
#include "Direct3D.h"
#include "Shader.h"

////�x����\��
//#pragma warning(disable : 4005)
//#pragma warning(disable : 4244)
//#pragma warning(disable : 4018)
//
//
#define MAX_BONES 255			// �ő�{�[����
#define ANIMATION_MAX 50		// �ő�A�j���[�V������

// �e�N�X�`�����X�g                                  // -- 2021.2.4
// �EFbxMeshCtrl�Ńe�N�X�`���̊Ǘ��Ɏg���郊�X�g
struct TextureList
{
	TCHAR m_FName[MAX_PATH];                 // �e�N�X�`����
	ID3D11ShaderResourceView* m_pTexture;    // �e�N�X�`��
	int   m_nCnt;                            // ���̃e�N�X�`�����g�p���Ă��郁�b�V���̐�

	TextureList()
	{
		m_FName[0] = _T('\0');
		m_pTexture = nullptr;
		m_nCnt = 0;
	}
};

// �X�^�e�B�b�N���b�V�����_�̍\����
struct StaticVertex
{
	VECTOR3 Pos;     // �ʒu
	VECTOR3 Normal;  // �@���@�V�F�[�f�B���O�i�A�e�v�Z�j�ɂ͖@���͕K�{
	VECTOR2 vTex;    // �e�N�X�`���[���W
};
struct StaticVertexNormal
{
	VECTOR3 Pos;      // �ʒu
	VECTOR3 Normal;   // �@���@�V�F�[�f�B���O�i�A�e�v�Z�j�ɂ͖@���͕K�{
	VECTOR2 vTex;     // �e�N�X�`���[���W
	VECTOR3 Tangent;  // �ڃx�N�g��  �m�[�}���}�b�s���O�ɂ͐ڃx�N�g�����K�v
	VECTOR3 Binormal; // �]�@��      �m�[�}���}�b�s���O�ɂ͏]�@�����K�v
};

// �X�L�����b�V�����_�̍\����
struct SkinVertex
{
	VECTOR3 Pos;             //�ʒu
	VECTOR3 Normal;          //�@���@�V�F�[�f�B���O�i�A�e�v�Z�j�ɂ͖@���͕K�{
	VECTOR2 vTex;            //�e�N�X�`���[���W
	UINT    ClusterNum[4];   // �e����^����N���X�^�[(�{�[��)�C���f�b�N�X
	VECTOR4 Weits;           // �E�F�C�g
};
struct SkinVertexNormal
{
	VECTOR3 Pos;             //�ʒu
	VECTOR3 Normal;          //�@���@�V�F�[�f�B���O�i�A�e�v�Z�j�ɂ͖@���͕K�{
	VECTOR2 vTex;            //�e�N�X�`���[���W
	UINT    ClusterNum[4];   // �e����^����N���X�^�[(�{�[��)�C���f�b�N�X
	VECTOR4 Weits;           // �E�F�C�g
	VECTOR3 Tangent;         //�ڃx�N�g��
	VECTOR3 Binormal;        //�]�@��
};

// �{�[���\����
struct BONE {
	MATRIX4X4  bindPose;		// �����|�[�Y(�t�s����)
	int        framePoseNum;    // �t���[���|�[�Y��
	MATRIX4X4* framePose;	    // �t���[���|�[�Y

	BONE()
	{
		framePose = nullptr;
		framePoseNum = 0;
		framePose = nullptr;
	}
	~BONE()
	{
		SAFE_DELETE_ARRAY(framePose);
	}
};

// �A�j���[�V�����\����
struct ANIMATION {
	int    used;
	int    startFrame;
	int    endFrame;
	int    RootBoneMesh;       // -- 2020.12.15 -- 3
	int    RootBone;           // -- 2020.12.15 -- 3
	bool   loop; // ���[�v�t���O�������Ɏ�������
	ANIMATION() {
		used = false;
	}
};

// �V�F�[�_�ɂ��̂܂ܓn����悤�Ƀ{�[����ɐ��񂵂������ςݍs��̔z��
struct BONESHADER
{
	MATRIX4X4  shaderFramePose[MAX_BONES];
};

// ���[�g�{�[���A�j���[�V�����^�C�v
enum ROOTANIMTYPE { eRootAnimNone = 0, eRootAnimXZ = 1, eRootAnim = 2 };          // -- 2020.12.15 -- 3


// ���b�V���z��
struct CFbxMeshArray
{
	StaticVertexNormal*   m_vStaticVerticesNormal; // ���_�f�[�^ �o���v�}�b�v�p
	SkinVertexNormal*     m_vSkinVerticesNormal; // ���_�f�[�^ �o���v�}�b�v�p
	DWORD*                m_nIndices;        // �C���f�b�N�X�f�[�^
	ID3D11Buffer*         m_pVertexBuffer;   // �o�[�e�b�N�X�o�b�t�@
	ID3D11Buffer*         m_pIndexBuffer;    // �C���f�b�N�X�o�b�t�@
	DWORD                 m_dwVerticesNum;   // �o�[�e�b�N�X�o�b�t�@�v�f��
	DWORD                 m_dwIndicesNum;    // �C���f�b�N�X�o�b�t�@�v�f��
	VECTOR4               m_pMaterialDiffuse;      // �}�e���A���f�B�t���[�Y�J���[     // -- 2020.12.15
	VECTOR4               m_pMaterialSpecular;     // �}�e���A���X�y�L�����J���[       // -- 2020.12.15
	ID3D11ShaderResourceView* m_pTexture;          // �e�N�X�`���[�A�h���X
	ID3D11ShaderResourceView* m_pTextureNormal;    // �e�N�X�`���[Normal�A�h���X
	ID3D11ShaderResourceView* m_pTextureHeight;    // �e�N�X�`���[Height�A�h���X
	ID3D11ShaderResourceView* m_pTextureSpecular;  // �e�N�X�`���[Specular�A�h���X
	DWORD                 m_NumBones;        // �{�[����
	BONE                  m_BoneArray[ANIMATION_MAX][MAX_BONES];  // �A�j���[�V�����f�[�^��[�t���[����][�{�[����] 
	BONESHADER*           m_pBoneShader[ANIMATION_MAX];       // �V�F�[�_�[�ɓn���{�[���s��[�t���[����]

	VECTOR3               m_vCenterPos;            // ���b�V�����S�_���W

	CFbxMeshArray()
	{
		m_vStaticVerticesNormal = nullptr;
		m_vSkinVerticesNormal = nullptr;
		m_nIndices = nullptr;
		m_pVertexBuffer = nullptr;
		m_pIndexBuffer = nullptr;
		m_dwVerticesNum = 0;
		m_dwIndicesNum = 0;
		m_pTexture = nullptr;
		m_pTextureNormal = nullptr;
		m_pTextureHeight = nullptr;
		m_pTextureSpecular = nullptr;
		m_NumBones = 0;        // �{�[����
		ZeroMemory(m_BoneArray, sizeof(m_BoneArray));
		ZeroMemory(m_pBoneShader, sizeof(m_pBoneShader));
		m_vCenterPos = VECTOR3(0, 0, 0);
	}
};

//
// Fbx���b�V���N���X
//
class CShader;
class CFbxMeshCtrl;
class CFbxMesh
{
public:
	CDirect3D* m_pD3D;  // Direct3D11
	CShader* m_pShader;
	CFbxMeshCtrl* m_pFbxMeshCtrl;                  // �e�N�X�`���R���g���[���N���X		// -- 2021.2.4

	int                   m_nMeshType;     // 0:����  1:�X�^�e�B�b�N���b�V��  2:�X�L�����b�V��
	VECTOR4* m_pMaterialDiffuseArray;    // �}�e���A���f�B�t���[�Y�J���[     // -- 2020.12.15
	VECTOR4* m_pMaterialSpecularArray;   // �}�e���A���X�y�L�����J���[       // -- 2020.12.15
	ID3D11ShaderResourceView** m_pTextureArray;       //�e�N�X�`���[�z��
	ID3D11ShaderResourceView** m_pTextureNormalArray; //�e�N�X�`���[Normal�z��
	ID3D11ShaderResourceView** m_pTextureHeightArray; //�e�N�X�`���[Height�z��
	ID3D11ShaderResourceView** m_pTextureSpecularArray; // �e�N�X�`���[Specular�z��
	DWORD                 m_dwTextureNum;       //�e�N�X�`���[�z��v�f��

	void SetAnimation(int id, float frame);
	void SetMixAnimation(int id, float frame, float rate, int id2, float frame2);
	CFbxMeshArray* m_pMeshArray;         // ���b�V���I�u�W�F�N�g�z��
	DWORD                 m_dwMeshNum;          // ���b�V���I�u�W�F�N�g�̗v�f��
	//	int                   m_allAnimationCount;  // �S�ẴA�j���[�V������

	ANIMATION			m_Animation[ANIMATION_MAX];       // �A�j���[�V����
	BONE                  m_RootBoneArray[ANIMATION_MAX];   // ���[�g�{�[���A�j���[�V�����f�[�^��[�t���[����]��   // -- 2020.12.15 -- 3
	ROOTANIMTYPE          m_RootAnimType[ANIMATION_MAX];    // ���[�g�{�[���A�j���^�C�v   eRootAnimNone:���[�g�A�j���Ȃ�  eRootAnimXZ:���[�g�A�j��XZ  eRootAnim:���[�g�A�j��  // -- 2020.12.15 -- 3

	float                 m_fHeightMax;         // �f�B�X�v���C�X�����g�}�b�s���O�̍���
	int                   m_iMaxDevide;         // �f�B�X�v���C�X�����g�}�b�s���O�̕�����
	VECTOR4               m_vDiffuse;           // �f�B�t���[�Y�J���[

	DWORD                 m_dwLoadTime;         // ���[�h����

	DWORD*                m_dwRenderIdxArray;       // �����_�����O���̓Y���z��
	float*                m_fRenderDistanceArray;   // �����_�����O�������߂邽�߂̋���

	VECTOR3               m_vMin;                   // ���f���̑傫���𑪂邽�߂̍ŏ��l  // -- 2021.2.4
	VECTOR3               m_vMax;                   // ���f���̑傫���𑪂邽�߂̍ő�l  // -- 2021.2.4
private:
		struct ANIMATION_INFO {
			int animID;
			float frame;
			float rate;
			int animIDSub;
			float frameSub;
		};
		ANIMATION_INFO animInfo;
		void    Render(ANIMATION_INFO& animStatus, const MATRIX4X4& mWorld);
		void    Render(ANIMATION_INFO& animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
		void    RenderSkin(ANIMATION_INFO& animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
		void    RenderDisplaceSkin(ANIMATION_INFO& animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
		void    RenderDisplace(ANIMATION_INFO& animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);

		void MakeBoneMatrix(MATRIX4X4* mat, const CFbxMeshArray& _mesh);
public:
	// ���\�b�h
	CFbxMesh();
	CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl);                              // -- 2021.2.4
	CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl, const TCHAR* FName);
	~CFbxMesh();

	void    Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    RenderDisplace(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    Render(const MATRIX4X4& mWorld);
	void    RenderDisplace(const MATRIX4X4& mWorld);

//
	void    RenderStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    RenderDisplaceStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);

	void    Draw(const int& animNum, const float& frame, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    DrawDisplace(const int& animNum, const int& frame, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    SetShaderTexture(const TCHAR ProfileName[], int SlotNo, ID3D11ShaderResourceView** ppTexture);
	void    SetShaderMatColor(const TCHAR ProfileName[], int SlotNo, int i);      // -- 2020.12.15

	void    DestroyD3D();
	bool    Load(const TCHAR* FName);
	bool    LoadAnimation(int id, const TCHAR* FName, bool loopFlag, const ROOTANIMTYPE& RAType = eRootAnimNone);

	VECTOR3 GetStaticCenterPos(const StaticVertex* vertex, const DWORD& Num);
	bool    ChangeStaticVertexLayout(const StaticVertex* vertices, const DWORD* indices, const DWORD& IndicesNum, StaticVertexNormal* verticesNormal);
	HRESULT SetStaticVIBuffer(const DWORD& mi, const StaticVertexNormal* vertices, const DWORD* indices);

	VECTOR3 GetSkinCenterPos(const SkinVertex* vertex, const DWORD& Num);
	bool    ChangeSkinVertexLayout(const SkinVertex* vertices, const DWORD* indices, const DWORD& IndicesNum, SkinVertexNormal* verticesNormal);
	HRESULT SetSkinVIBuffer(const DWORD& mi, const SkinVertexNormal* vertices, const DWORD* indices);

	void    SetRenderIdxArray(const MATRIX4X4& mWorld, const VECTOR3& vEye);
	bool    CalcTangentSub(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3,
							const VECTOR2& uv1, const VECTOR2& uv2, const VECTOR2& uv3, VECTOR3 &tangent, VECTOR3 &Binormal);
	HRESULT SetTexture(const DWORD& ti, const TCHAR* TexName);
	bool    SetMaterial(const DWORD& ti, const TCHAR* TexName);               // -- 2020.12.15

	int GetEndFrame(const int& animNum);  // endFrame-startFrame���擾 

	MATRIX4X4 GetFrameMatrices(const DWORD& nBone);							       // -- 2024.3.14
	MATRIX4X4 GetFrameMatrices(const DWORD& nBone, const DWORD& nMesh);		       // -- 2024.3.14
	MATRIX4X4 GetFrameMatrices(const MATRIX4X4& mWorld, const DWORD& nBone);       // -- 2024.3.14
	MATRIX4X4 GetFrameMatrices(const MATRIX4X4& mWorld, const DWORD& nBone, const DWORD& nMesh);      // -- 2024.3.14

	int GetRootAnimType() { return m_RootAnimType[animInfo.animID]; }      // -- 2024.3.14
	MATRIX4X4 GetRootAnimMatrices( const int& UpFrame = 0);      // -- 2024.3.14
	MATRIX4X4 GetRootAnimUpMatrices(const int& UpFrame = 0, const int& StartFrameUp = 1);      // -- 2024.3.14
	void MakeRootAnimAndShaderMatrix(const int& animNum, ROOTANIMTYPE RAType);        // ���[�g�{�[���A�j���[�V�����ƃV�F�[�_�[�}�g���b�N�X�̍쐬       // -- 2020.12.15 -- 3

	void ConvIndicesData(int mi);                        // -- 2024.3.23
};

//
// Fbx���b�V���R���g���[���N���X            // -- 2021.2.4
// 
// ���b�V���̑����I�ȊǗ����s���N���X
// ��Ƀe�N�X�`���̊Ǘ����s���B
// �����̃��b�V���Ԃœ���̃e�N�X�`�����g���Ă���ꍇ�A�d������ǂݍ��݂�r������
//
class CFbxMeshCtrl
{
public:
	CDirect3D*     m_pD3D;  // Direct3D11
	CShader*       m_pShader;

	std::list<TextureList> m_TexList;		// �e�N�X�`���[�̃��X�g

public:
	ID3D11ShaderResourceView* SetTextureList(const TCHAR FName[]);
	void DeleteTextureList(ID3D11ShaderResourceView*);

	CFbxMeshCtrl(CShader* pShader);
	~CFbxMeshCtrl();
};
