#pragma once
#include <string>
#include <vector>

/// <summary>
/// CSV��ǂރN���X
/// </summary>
class CsvReader {
public:
	CsvReader(std::string filename);
	~CsvReader();

	/// <summary>
	/// CSV�t�@�C���̍s����Ԃ�
	/// </summary>
	/// <returns>�s��</returns>
	unsigned int GetLines();

	/// <summary>
	/// CSV�̌�����Ԃ�
	/// �s�ɂ���Č������ς��\��������̂ŁA�����ōs���擾����
	/// �s�ԍ��́AExcel�̕\���s�ł͂Ȃ��A0����w�肷��
	/// </summary>
	/// <param name="line">�s�ԍ��iExcel�̕\���s��-1)</param>
	/// <returns>���̍s�̌���</returns>
	unsigned int GetColumns(unsigned int line);

	/// <summary>
	/// �w��ʒu�̕�������擾����
	/// </summary>
	/// <param name="line">�s</param>
	/// <param name="column">��</param>
	/// <returns>������</returns>
	std::string GetString(unsigned int line, unsigned int column);

	/// <summary>
	/// �w��ʒu�̓��e��int�Ŏ擾����
	/// </summary>
	/// <param name="line">�s</param>
	/// <param name="column">��</param>
	/// <returns>�����l</returns>
	int GetInt(unsigned int line, unsigned int column);

	/// <summary>
	/// �w��ʒu�̓��e��float�Ŏ擾����
	/// </summary>
	/// <param name="line">�s</param>
	/// <param name="column">��</param>
	/// <returns>�����l</returns>
	float GetFloat(unsigned int line, unsigned int column);
private:
	struct LINEREC {
		std::vector<std::string> record;
	};
	std::vector<LINEREC> all;
};
