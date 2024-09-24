#include "Animator.h"
#include <fstream>
#include "gameObject.h"

Animator::Animator()
{
	main.id = -1;
	main.frame = 0.0f;
	changeTime = 0.0f;
	time = 0.0f;

	playSpeed = 1.0f;
	finished = true;
}

Animator::~Animator()
{
}

void Animator::Update()
{
	if (base==nullptr)
		return;
	finished = false;
	main.frame += playSpeed;
	const ANIMATION &info = base->m_Animation[main.id];
	if (main.frame >= info.endFrame) {

		if (info.loop) {
			main.frame -= info.endFrame - info.startFrame;
		}
		else {
			main.frame = info.endFrame;
			finished = true;
		}
	}
	if (changeTime > 0.0f) {
		time += SceneManager::DeltaTime();
		float rate = time / changeTime;
		if (rate >= 1.0f) {
			rate = 1.0f;
			changeTime = 0.0;
			time = 0.0f;
		}
		base->SetMixAnimation(main.id, main.frame, rate, sub.id, sub.frame);
	}
	else {
		base->SetAnimation(main.id, main.frame);
	}
}

void Animator::SetModel(CFbxMesh* mesh)
{
	base = mesh;
}

void Animator::Play(int id, bool force)
{
//	playSpeed = 1.0f;
	if (base == nullptr)
		return;

	const ANIMATION &info = base->m_Animation[id];
	if (!info.used) {
		MessageBox(nullptr, "Animator::Play()", _T("������ �A�j���[�V������Load����Ă��܂��� ������"), MB_OK);
	}
	if (main.id == id && !Finished()) {
		if (force) {
			main.frame = info.startFrame;
		}
	}
	else {
		main.id = id;
		main.frame = info.startFrame;
	}
}

void Animator::MergePlay(int id, float time)
{
	//	playSpeed = 1.0f;
	if (base == nullptr)
		return;

	const ANIMATION& info = base->m_Animation[id];
	if (!info.used) {
		MessageBox(nullptr, "Animator::Play()", _T("������ �A�j���[�V������Load����Ă��܂��� ������"), MB_OK);
	}
	if (main.id != id) {
		changeTime = time;
		sub.id = main.id;
		sub.frame = main.frame;
		main.id = id;
		main.frame = info.startFrame;
	}
}

void Animator::SetPlaySpeed(float speed)
{
	playSpeed = speed;
}

int Animator::PlayingID()
{
	return main.id;
}

float Animator::CurrentFrame()
{
	return main.frame;
}

bool Animator::Finished()
{
	const ANIMATION &info = base->m_Animation[main.id];
	return !info.loop&& main.frame >= info.endFrame;
}
