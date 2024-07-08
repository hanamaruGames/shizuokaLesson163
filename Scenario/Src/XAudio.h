// =========================================================================================
//
//  �w�`���������Q�@
//																   ver 3.3        2024.3.23
// =========================================================================================

#pragma once

#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <tchar.h>

#define AUDIO_LOOP         1          // ���[�v�Đ�
#define AUDIO_SOURCE_MAX  10          // �����ɍĐ����铯��I�[�f�B�I�\�[�X�̍ő吔

// 
//  XAudio �}�X�^�[�N���X	
//  �A�v���Ɉ�K�v
// 
class CXAudio
{
private:
	// �����o�ϐ�
	HWND                    m_hWnd;
	IXAudio2*               m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;

public:
	// ���\�b�h
	HRESULT     InitAudio(HWND);
	CXAudio();
	~CXAudio();
	IXAudio2* XAudio2() { return m_pXAudio2; }
};

// 
//  XAudio �\�[�X�{�C�X�N���X	
//  ��̃T�E���h�i�v�`�u�j�Ɉ�K�v
// 
class CXAudioSource
{
private:
	CXAudio*				m_pXAudio;			// XAudio �}�X�^�[�N���X�A�h���X	
	bool					m_bWav;				// �\�[�X���v�������t�@�C�����@true:XAudio(WAV)�@false:MCI(MP3��MID) 
	TCHAR                   m_szAliasName[256]; // Mci�̕ʖ�������

	DWORD                   m_dwSourceIndex;					//	�I�[�f�B�I�\�[�X�C���f�b�N�X
	DWORD                   m_dwSourceNum;						//	�I�[�f�B�I�\�[�X�̌�	
	IXAudio2SourceVoice*    m_pSourceVoice[AUDIO_SOURCE_MAX];  // �I�[�f�B�I�\�[�X
	BYTE*                   m_pWavBuffer[AUDIO_SOURCE_MAX];    // �g�`�f�[�^�i�t�H�[�}�b�g�����܂܂Ȃ��A�����ɔg�`�f�[�^�̂݁j
	DWORD                   m_dwWavSize[AUDIO_SOURCE_MAX];     // �g�`�f�[�^�̃T�C�Y
public:
	HRESULT      Load(const TCHAR* szFileName, DWORD dwNum=1);
	HRESULT      LoadAudio(const TCHAR* szFileName, DWORD dwNum);
	HRESULT      LoadAudioSub(const TCHAR* szFileName, DWORD dwIndex);
	HRESULT      LoadMci(const TCHAR* szFileName);
	void         Play(int loop=0);
	void         PlayAudio(int loop=0);
	void         PlayMci(int loop=0);
	void         Stop();
	void         StopAudio();
	void         StopMci();
	void         Volume(float fVol);
	void         VolumeAudio(float fVol);
	void         VolumeMci(int nVol);
	
	CXAudioSource();
	CXAudioSource(const TCHAR* szFileName, DWORD dwNum = 1);
	CXAudioSource(CXAudio*	pXAudio);
	CXAudioSource(CXAudio*	pXAudio, const TCHAR* szFileName, DWORD dwNum=1);
	~CXAudioSource();

};