//=============================================================================
//		メッシュの読み込みと描画のプログラム
//　                                                  ver 3.3        2024.3.234
//
//		メッシュ処理
//
//      (メッシュコントロールクラスでテクスチャの総合管理を行う)
//
//
//																	FbxMesh.h
//=============================================================================
#pragma once

#include <list>
#include "Direct3D.h"
#include "Shader.h"
#include <vector>

////警告非表示
//#pragma warning(disable : 4005)
//#pragma warning(disable : 4244)
//#pragma warning(disable : 4018)
//
//
#define MAX_BONES 255			// 最大ボーン数
#define ANIMATION_MAX 50		// 最大アニメーション数

// テクスチャリスト                                  // -- 2021.2.4
// ・FbxMeshCtrlでテクスチャの管理に使われるリスト
struct TextureList
{
	TCHAR m_FName[MAX_PATH];                 // テクスチャ名
	ID3D11ShaderResourceView* m_pTexture;    // テクスチャ
	int   m_nCnt;                            // このテクスチャを使用しているメッシュの数

	TextureList()
	{
		m_FName[0] = _T('\0');
		m_pTexture = nullptr;
		m_nCnt = 0;
	}
};

// スタティックメッシュ頂点の構造体
struct StaticVertex
{
	VECTOR3 Pos;     // 位置
	VECTOR3 Normal;  // 法線　シェーディング（陰影計算）には法線は必須
	VECTOR2 vTex;    // テクスチャー座標
};
struct StaticVertexNormal
{
	VECTOR3 Pos;      // 位置
	VECTOR3 Normal;   // 法線　シェーディング（陰影計算）には法線は必須
	VECTOR2 vTex;     // テクスチャー座標
	VECTOR3 Tangent;  // 接ベクトル  ノーマルマッピングには接ベクトルが必要
	VECTOR3 Binormal; // 従法線      ノーマルマッピングには従法線が必要
};

// スキンメッシュ頂点の構造体
struct SkinVertex
{
	VECTOR3 Pos;             //位置
	VECTOR3 Normal;          //法線　シェーディング（陰影計算）には法線は必須
	VECTOR2 vTex;            //テクスチャー座標
	UINT    ClusterNum[4];   // 影響を与えるクラスター(ボーン)インデックス
	VECTOR4 Weits;           // ウェイト
};
struct SkinVertexNormal
{
	VECTOR3 Pos;             //位置
	VECTOR3 Normal;          //法線　シェーディング（陰影計算）には法線は必須
	VECTOR2 vTex;            //テクスチャー座標
	UINT    ClusterNum[4];   // 影響を与えるクラスター(ボーン)インデックス
	VECTOR4 Weits;           // ウェイト
	VECTOR3 Tangent;         //接ベクトル
	VECTOR3 Binormal;        //従法線
};

// ボーン構造体
struct BONE {
	MATRIX4X4  bindPose;		// 初期ポーズ(逆行列状態)
	int        framePoseNum;    // フレームポーズ数
	MATRIX4X4* framePose;	    // フレームポーズ

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

// アニメーション構造体
struct ANIMATION {
	int    used;
	int    startFrame;
	int    endFrame;
	int    RootBoneMesh;       // -- 2020.12.15 -- 3
	int    RootBone;           // -- 2020.12.15 -- 3
	bool   loop; // ループフラグをここに持たせる
	ANIMATION() : used(false), 
		startFrame(0), endFrame(0),
		RootBoneMesh(0), RootBone(0),
		loop(false)
		{}
};

// シェーダにそのまま渡せるようにボーン基準に整列した合成済み行列の配列
struct BONESHADER
{
	MATRIX4X4  shaderFramePose[MAX_BONES];
};

// ルートボーンアニメーションタイプ
enum ROOTANIMTYPE { eRootAnimNone = 0, eRootAnimXZ = 1, eRootAnim = 2 };          // -- 2020.12.15 -- 3


// メッシュ配列
struct CFbxMeshArray
{
	StaticVertexNormal*   m_vStaticVerticesNormal; // 頂点データ バンプマップ用
	SkinVertexNormal*     m_vSkinVerticesNormal; // 頂点データ バンプマップ用
	DWORD*                m_nIndices;        // インデックスデータ
	ID3D11Buffer*         m_pVertexBuffer;   // バーテックスバッファ
	ID3D11Buffer*         m_pIndexBuffer;    // インデックスバッファ
	DWORD                 m_dwVerticesNum;   // バーテックスバッファ要素数
	DWORD                 m_dwIndicesNum;    // インデックスバッファ要素数
	VECTOR4               m_pMaterialDiffuse;      // マテリアルディフューズカラー     // -- 2020.12.15
	VECTOR4               m_pMaterialSpecular;     // マテリアルスペキュラカラー       // -- 2020.12.15
	ID3D11ShaderResourceView* m_pTexture;          // テクスチャーアドレス
	ID3D11ShaderResourceView* m_pTextureNormal;    // テクスチャーNormalアドレス
	ID3D11ShaderResourceView* m_pTextureHeight;    // テクスチャーHeightアドレス
	ID3D11ShaderResourceView* m_pTextureSpecular;  // テクスチャーSpecularアドレス
	DWORD                 m_NumBones;        // ボーン数
	BONE                  m_BoneArray[ANIMATION_MAX][MAX_BONES];  // アニメーションデータが[フレーム数][ボーン数] 
	BONESHADER*           m_pBoneShader[ANIMATION_MAX];       // シェーダーに渡すボーン行列[フレーム数]

	VECTOR3               m_vCenterPos;            // メッシュ中心点座標

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
		m_NumBones = 0;        // ボーン数
		ZeroMemory(m_BoneArray, sizeof(m_BoneArray));
		ZeroMemory(m_pBoneShader, sizeof(m_pBoneShader));
		m_vCenterPos = VECTOR3(0, 0, 0);
	}
};

//
// Fbxメッシュクラス
//
class CShader;
class CFbxMeshCtrl;
class CFbxMesh
{
private:
	CDirect3D* m_pD3D;  // Direct3D11
	CShader* m_pShader;
	CFbxMeshCtrl* m_pFbxMeshCtrl;                  // テクスチャコントロールクラス		// -- 2021.2.4
public:
	int                   m_nMeshType;     // 0:未定  1:スタティックメッシュ  2:スキンメッシュ
private:
	VECTOR4* m_pMaterialDiffuseArray;    // マテリアルディフューズカラー     // -- 2020.12.15
	VECTOR4* m_pMaterialSpecularArray;   // マテリアルスペキュラカラー       // -- 2020.12.15
	ID3D11ShaderResourceView** m_pTextureArray;       //テクスチャー配列
	ID3D11ShaderResourceView** m_pTextureNormalArray; //テクスチャーNormal配列
	ID3D11ShaderResourceView** m_pTextureHeightArray; //テクスチャーHeight配列
	ID3D11ShaderResourceView** m_pTextureSpecularArray; // テクスチャーSpecular配列
	DWORD                 m_dwTextureNum;       //テクスチャー配列要素数

	void SetAnimation(int id, float frame);
	void SetMixAnimation(int id, float frame, float rate, int id2, float frame2);
public:
	CFbxMeshArray* m_pMeshArray;         // メッシュオブジェクト配列
	DWORD                 m_dwMeshNum;          // メッシュオブジェクトの要素数
	//	int                   m_allAnimationCount;  // 全てのアニメーション数

	ANIMATION			m_Animation[ANIMATION_MAX];       // アニメーション
private:
	BONE                  m_RootBoneArray[ANIMATION_MAX];   // ルートボーンアニメーションデータが[フレーム数]分   // -- 2020.12.15 -- 3
	ROOTANIMTYPE          m_RootAnimType[ANIMATION_MAX];    // ルートボーンアニメタイプ   eRootAnimNone:ルートアニメなし  eRootAnimXZ:ルートアニメXZ  eRootAnim:ルートアニメ  // -- 2020.12.15 -- 3

	float                 m_fHeightMax;         // ディスプレイスメントマッピングの高さ
	int                   m_iMaxDevide;         // ディスプレイスメントマッピングの分割数
	VECTOR4               m_vDiffuse;           // ディフューズカラー

	DWORD                 m_dwLoadTime;         // ロード時間

	DWORD*                m_dwRenderIdxArray;       // レンダリング順の添字配列
	float*                m_fRenderDistanceArray;   // レンダリング順を決めるための距離

	VECTOR3               m_vMin;                   // モデルの大きさを測るための最小値  // -- 2021.2.4
	VECTOR3               m_vMax;                   // モデルの大きさを測るための最大値  // -- 2021.2.4
public:
	struct ANIMATION_INFO {
		int animID;
		int frame;
		float rate;
		ANIMATION_INFO() : animID(0), frame(0), rate(1.0f) {}
		ANIMATION_INFO(int id, int frame, float rate) {
			animID = id, this->frame = frame, this->rate = rate;
		}
	};
	// メソッド
	CFbxMesh();
	CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl);                              // -- 2021.2.4
	CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl, const TCHAR* FName);
	~CFbxMesh();
	bool    Load(const TCHAR* FName);
	bool    LoadAnimation(int id, const TCHAR* FName, bool loopFlag = true, const ROOTANIMTYPE& RAType = eRootAnimNone);

	void    Render(const MATRIX4X4& mWorld);
	void    RenderDisplace(const MATRIX4X4& mWorld);

	void    Render(int animID, int frame, const MATRIX4X4& mWorld);
	void    RenderDisplace(int animID, int frame, const MATRIX4X4& mWorld);

	void    Render(const std::vector<ANIMATION_INFO> anim, const MATRIX4X4& mWorld);
	void    RenderDisplace(const std::vector<ANIMATION_INFO> anim, int frame, const MATRIX4X4& mWorld);

	void    Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    RenderDisplace(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);

	void    Render(const std::vector<ANIMATION_INFO> anim, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    RenderDisplace(const std::vector<ANIMATION_INFO> anim, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);

	MATRIX4X4 GetFrameMatrices(int animID, int frame, DWORD nBone);							       // -- 2024.3.14
private:
	void    RenderStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    RenderDisplaceStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);

	void    RenderSkin(const std::vector<ANIMATION_INFO> anim, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    RenderDisplaceSkin(const std::vector<ANIMATION_INFO> anim, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);

	void	MakeBoneMatrix(const std::vector<ANIMATION_INFO> anim, MATRIX4X4* mat, const CFbxMeshArray& _mesh);

	void    Draw(const std::vector<ANIMATION_INFO> anim, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    DrawDisplace(const std::vector<ANIMATION_INFO> anim, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);
	void    SetShaderTexture(const TCHAR ProfileName[], int SlotNo, ID3D11ShaderResourceView** ppTexture);
	void    SetShaderMatColor(const TCHAR ProfileName[], int SlotNo, int i);      // -- 2020.12.15

	void    DestroyD3D();

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

	int GetEndFrame(const int& animID);  // endFrame-startFrameを取得 

	MATRIX4X4 GetFrameMatrices(int animID, int frame, DWORD nBone, DWORD nMesh);		       // -- 2024.3.14
	MATRIX4X4 GetFrameMatrices(int animID, int frame, const MATRIX4X4& mWorld, DWORD nBone);       // -- 2024.3.14
	MATRIX4X4 GetFrameMatrices(int animID, int frame, const MATRIX4X4& mWorld, DWORD nBone, DWORD nMesh);      // -- 2024.3.14

	int GetRootAnimType(int animID) { return m_RootAnimType[animID]; }      // -- 2024.3.14
	MATRIX4X4 GetRootAnimMatrices(int animID,  int frame);      // -- 2024.3.14
	MATRIX4X4 GetRootAnimUpMatrices(int animID, int frame, const int& StartFrameUp = 1);      // -- 2024.3.14
	void MakeRootAnimAndShaderMatrix(const int& animNum, ROOTANIMTYPE RAType);        // ルートボーンアニメーションとシェーダーマトリックスの作成       // -- 2020.12.15 -- 3

	void ConvIndicesData(int mi);                        // -- 2024.3.23
};

//
// Fbxメッシュコントロールクラス            // -- 2021.2.4
// 
// メッシュの総合的な管理を行うクラス
// 主にテクスチャの管理を行う。
// 複数のメッシュ間で同一のテクスチャが使われている場合、重複する読み込みを排除する
//
class CFbxMeshCtrl
{
public:
	CDirect3D*     m_pD3D;  // Direct3D11
	CShader*       m_pShader;

	std::list<TextureList> m_TexList;		// テクスチャーのリスト

public:
	ID3D11ShaderResourceView* SetTextureList(const TCHAR FName[]);
	void DeleteTextureList(ID3D11ShaderResourceView*);

	CFbxMeshCtrl(CShader* pShader);
	~CFbxMeshCtrl();
};
