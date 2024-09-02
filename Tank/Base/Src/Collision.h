//------------------------------------------------------------------------
//    ���b�V���ڐG����p�̃��C�u����
//	  											ver 3.3         2024.3.23
//	  Collision.h								
//------------------------------------------------------------------------
#pragma once

// �œK���̒��~
//#pragma optimize("", off)

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>

#include "Direct3D.h"
#include "Shader.h"
#include "FbxMesh.h"


// ---------------------------------------------------------
//
// �}�N����`
//
// ---------------------------------------------------------
// �ǂƏ��Ƃ̋�ʂ����邽�߂Ɏg�p�B�ʖ@���̂x�����̒l�ł���
// 0.5f�̂Ƃ��͖@���p�x��30߂ł���̂ŁA�ʂ̊p�x��60���ƂȂ�
// 0.6f�̂Ƃ��͖@���p�x�͖�37߂ł���̂ŁA�ʂ̊p�x�͖�53���ƂȂ�B
#define  GROUND  0.6f

// �d�͂̉����x
// Collision�̒��ł͎g�p���Ă��Ȃ��B�Q�[�����Ŏg�p���Ă���
#define  GRAVITY  -9.8f

// AddAngle�֐��̖߂�l�ł���p�x�i���W�A���j�̌��E�l
// ���_�l�́A2.0�ł��邪�덷���l������>=1.99f�`1.95f���x�Ƃ���
#define  ADDANGLELIMIT  1.99f

// HightCheck�֐��Ŏg�p����ړ��揰�𔻒肷�鍂���̏���l
// ������l�����āA�����l��1.0�ł��邪�A�ړ���W�����v�X�s�[�h�ƌ��z���l������1.0�`10.0���x�Ƃ���
#define  UPHILLLIMIT  1.0f

// CheckFloorMove�֐��Ŏg�p����ړ��揰�ɒǏ]���鍂���̉����l
// �ړ���_�̏������̍������Ⴂ�ƁA�󒆂ɕ����Ԃ悤�ɂȂ�i�������j
#define  LOWFLOORLIMIT  -0.2f

// HightCheck�֐��Ŏg�p����L�����N�^�[�̑����̏�����̍���
// ���̒l�����A�����Ə��̊ԂɌ��Ԃ��ł���
// ���̒l����������������Ə��ƕǂ̋��ڂ��甲��������\���������Ȃ�
#define  UNDERFOOTLIMIT  0.05f

// ���b�V���ڐG����z��ő�v�f��
#define  MCKTBL_MAX     100

// ���������x�}�b�v�ő啪����
#define  MESHCKTBL_FACE_MAX  8
//#define  MESHCKTBL_FACE_MAX  1

// ���������x�}�b�v�̍ŏ��T�C�Y
#define  MESHCKTBL_LOWLIMIT  3.0f


// ---------------------------------------------------------
//
// AABB�i�����s���E�{�b�N�X�FAxis - Aligned Bounding Box�j�N���X
//
// ---------------------------------------------------------
class CAABB
{
public:
	VECTOR3 m_vMax;
	VECTOR3 m_vMin;
public:
	CAABB() = default;
	CAABB(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3)
	{
		MakeAABB(v1, v2, v3);
	}
	CAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r = 0.0f)
	{
		MakeAABB(v1, v2, r);
	}
	void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3);
	void MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r = 0.0f);
	bool Hitcheck(const CAABB& other);
	bool HitcheckXZ(const CAABB& other);
	VECTOR3 GetVecPos(const int& nIdx);
};

// ---------------------------------------------------------
// �R���W�������b�V����1�|���S��
// ---------------------------------------------------------
struct ColFace
{
	DWORD				dwIdx[3];	// �R�p�`�|���S���C���f�b�N�X
	VECTOR3				vNormal;	// �R�p�`�|���S���@��
	CAABB				AABB;		// �����s�o�E���f�B���O�{�b�N�X
};

// ---------------------------------------------------------
// �R���W�������b�V��
// ---------------------------------------------------------
struct ColMesh
{
	ColFace*			pFace;
	VECTOR3*			pVert;
	int					nNumFace;
	int					nNumVert;

	ColMesh()
	{
		pFace		= nullptr;
		pVert		= nullptr;
		nNumFace	= 0;
		nNumVert	= 0;
	}
};
// ---------------------------------------------------------
// ���������x�}�b�v
// ---------------------------------------------------------
//
// �P�|���S���̃A�h���X�i�[�p
// ���X�g�\���ƂȂ��Ă���
//
struct ChkFace
{
	ColFace*			pFace;
	ChkFace*			pNext;
	ChkFace()
	{
		ZeroMemory(this, sizeof(struct ChkFace));
	}
};
//
// ���ꕪ���x�̃|���S�����X�g�i�[�p
//
struct ChkBlk
{
	ChkFace**			ppChkFace;  // dwNumX*dwNumY*dwNumZ�̃|�C���^�z��
	VECTOR3				vBlksize;
	DWORD				dwNumX, dwNumY, dwNumZ;
	ChkBlk()
	{
		ZeroMemory(this, sizeof(struct ChkBlk));
	}
};
//
// ���������x�}�b�v�i�[�p
//
struct ChkColMesh
{
	ChkBlk				ChkBlkArray[MESHCKTBL_FACE_MAX];
	VECTOR3				vMin;
	VECTOR3				vMax;							// -- 2020.12.3
	ChkColMesh()
	{
		ZeroMemory(this, sizeof(struct ChkColMesh));
	}
};



class CFbxMeshCtrl;
class CFbxMesh;
// ---------------------------------------------------------
//
// �R���W�����N���X
//
// ---------------------------------------------------------
class  CCollision
{
public:
	CDirect3D*			m_pD3D;
	CShader*			m_pShader;
	CFbxMeshCtrl*		m_pFbxMeshCtrl;			// -- 2021.2.4

	// ��������p�̕ϐ�
	struct ColFace*		m_pIndex;
	VECTOR3				m_vNormalH;
	float				m_fHeight;
	VECTOR3				m_vVertexH[3];

	// ���b�V���ڐG����z��
	// (vector�N���X�͒x�����ߔz����g�p����)
	ColMesh				m_ColArray[MCKTBL_MAX];
	int					m_nNum;			// �z��̗v�f��

	// ���������x�}�b�v
	ChkColMesh			m_ChkColMesh[MCKTBL_MAX];

	// �R���W�����ړ��p
	bool				m_bMoveFlag;	// �ړ����邩�ǂ��� �ړ��̎� �^
	MATRIX4X4			m_mWorldBase;	// �ړ��}�g���b�N�X�̏����ʒu	// -- 2022.11.14
	MATRIX4X4			m_mWorldOld;	// �ړ��}�g���b�N�X�i���݈ʒu�̈�O�j
	MATRIX4X4			m_mWorld;		// �ړ��}�g���b�N�X�̌��݈ʒu
	MATRIX4X4			m_mWorldInv;	// �ړ��}�g���b�N�X�̌��݈ʒu�̋t�}�g���b�N�X

public:
	CCollision();		   // -- 2024.3.13
	CCollision(CFbxMeshCtrl* pFbxMeshCtrl);   // -- 2021.2.4

	~CCollision();

	bool	AddFbxLoad( const TCHAR*);										// -- 2022.11.14
	bool	AddFbxLoad( const TCHAR*, const VECTOR3& vOffset);
	bool	AddFbxLoad( const TCHAR*, const MATRIX4X4& mOffset);			// -- 2022.11.14
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh);							// -- 2022.11.14
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh, const VECTOR3& vOffset);
	bool	AddFbxLoad(const CFbxMesh* pFbxMesh, const MATRIX4X4& mOffset);	// -- 2022.11.14
	void	MakeChkColMesh(const int& nNum, const VECTOR3& vMin, const VECTOR3& vMax);
	void	GetChkArrayIdx(const int& nNum, const int& nNo, CAABB AABB, int nIdx[], int& nIMax);
	void	SetChkArray(const int& nNum, const int& nNo, const int& nIdx, ColFace* pFace);
	void	ClearAll(void);
	void	DeleteAll(void);

	int     isCollisionLay(const MATRIX4X4& mWorld, const MATRIX4X4& mWorldOld, VECTOR3& vHit, VECTOR3& vNormal);
	int     isCollisionLay(const VECTOR3& vNowIn, const VECTOR3& vOldIn, VECTOR3& vHit, VECTOR3& vNormal);
	int     isCollisionSphere(const VECTOR3& vNowIn, const VECTOR3& vOldIn, const float& fRadius, VECTOR3& vHit, VECTOR3& vNormal);
	int     isCollisionMove(MATRIX4X4& mWorld, const MATRIX4X4& mWorldOld, float fRadius = 0.2f);
	int     isCollisionMove(MATRIX4X4& mWorld, const MATRIX4X4& mWorldOld, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);    // -- 2022.11.14
	int     isCollisionMoveGravity(MATRIX4X4& mWorld, const MATRIX4X4& mWorldOld, float fRadius = 0.2f);
	int     isCollisionMoveGravity(MATRIX4X4& mWorld, const MATRIX4X4& mWorldOld, VECTOR3& vHit, VECTOR3& vNormal, float fRadius = 0.2f);    // -- 2022.11.14

	void    GetMeshLimit(const int& nNum, const int& nNo, const VECTOR3& vNow, const VECTOR3& vOld, const float& fRadius,
							int& nStatrX, int& nEndX, int& nStatrY, int& nEndY, int& nStatrZ, int& nEndZ);

	int     CheckWallMove(MATRIX4X4& mWorld, const MATRIX4X4& mWorldOld, VECTOR3& vHit, VECTOR3& vNormal, float fRadius);    // -- 2022.11.14
	void    InitHeightCheck();
	bool    CheckHeight(MATRIX4X4& mWorld, const MATRIX4X4&  mWorldOld, const float fObjheight);
	int     CheckFloorMove(MATRIX4X4& mWorld, const MATRIX4X4&  mWorldOld);
	int     CheckCollisionMove(MATRIX4X4& mWorld, const MATRIX4X4& mWorldOld, VECTOR3& vHit, VECTOR3& vNormal, float fRadius);    // -- 2022.11.14
	void    GetDistNormal(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, float& fNowDist, float& fOldDist, float& fLayDist);
	int     CheckLay(const VECTOR3[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, const float& fNowDist, const float& fOldDist, const float& fLayDist, VECTOR3& vHit);
	int     CheckNear(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vFaceNorm, const float& fNowDist, float fRadius, VECTOR3& vHit);

	void	InitWorldMatrix(const MATRIX4X4&);	// -- 2022.11.14
	void	SetWorldMatrix(const MATRIX4X4&);

	void    GetChkAABB(VECTOR3& vMin, VECTOR3& vMax);				// -- 2020.12.3

};
