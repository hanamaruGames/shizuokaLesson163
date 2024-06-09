#pragma once

namespace MainControl {
	/// <summary>
	/// �t���[�����[�g�́A�`��X�V�̃��b�Z�[�W���g��
	/// </summary>
	void UseRefreshMessage();

	/// <summary>
	/// �t���[�����[�g��ݒ肷��
	/// �P�t���[���̎��Ԃ�b�Őݒ肷��̂ŁA�U�O�������̏ꍇ�́A
	/// 1.0/60.0���w�肷��
	/// </summary>
	/// <param name="time">�X�V���ԁi�b�j</param>
	void UseFrameTimer(float time);

	/// <summary>
	/// �E�B���h�E�̖��O��ύX����
	/// ���O�́A�E�B���h�E�̍���ɕ\������܂�
	/// </summary>
	/// <param name="name">�E�B���h�E�̖��O</param>
	void SetWindowName(const char* name);

	/// <summary>
	/// ���ݐݒ肳��Ă��郊�t���b�V���^�C�}�[���擾����
	/// 0.0�ȉ��̏ꍇ�́ARefreshMessage���g���Ă��������B
	/// </summary>
	/// <returns>�t���[���^�C�}�[</returns>
	float RefreshTimer();
};