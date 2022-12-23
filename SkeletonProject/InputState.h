#pragma once
#include <map>
#include <vector>
#include <string>

enum class InputType
{
	next,	 //���֍s���{�^��
	prev,	 //�O�ɖ߂�{�^��
	pause,	 //�|�[�Y�{�^��
	keyconf, //�L�[�R���t�B�O
	change,	 //�ύX
	up,		 //��
	down,	 //��
	max		 //�ő���̓C���f�b�N�X
};

/// <summary>
/// ���͑��u�J�e�S��
/// </summary>
enum class InputCategory
{
	keybd,	//�L�[�{�[�h
	pad,	//�Q�[���p�b�h
	mouse	//�}�E�X
};

/// <summary>
/// ���͏��
/// </summary>
struct InputInfo
{
	InputCategory cat;	//���͑��u�J�e�S��
	int id;				//����ID(KEY_INPUT_�`,PAD_INPUT_�`,MOUSE_INPUT_�`)
};

class KeyConfigScene;

/// <summary>
/// ���͏�Ԃ��Ǘ�����
/// </summary>
class InputState
{
	friend KeyConfigScene;	//KeyConfigScene�ɂ����A���ׂĂ�������
public:
	InputState();

	/// <summary>
	/// �������u�Ԃ�true�ɂȂ�
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns></returns>
	bool IsTriggered(InputType type) const;

	/// <summary>
	/// ������Ă�����true�ɂȂ�
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>������Ă���true,������ĂȂ��Ȃ�false</returns>
	bool IsPressed(InputType type) const;

	/// <summary>
	/// ���͏����X�V����
	/// (����)���t���[��Update���Ă΂Ȃ��Ɠ��͏�Ԃ͍X�V����܂���
	/// </summary>
	void Update();

	/// <summary>
	/// ���͏����X�V����
	/// </summary>
	/// <param name="type">�Q�[���ɂ�������͎��</param>
	/// <param name="cat">���̓J�e�S��(keybd��)</param>
	/// <param name="id">���ۂ̓���</param>
	void RewriteInputInfo(InputType type, InputCategory cat, int id);
private:
	//���ۂ̓��͂ƃQ�[���{�^���̑Ή��e�[�u��
	std::map<InputType, std::vector<InputInfo>> inputMapTable_;	

	//���̓^�C�v�Ƃ��̖��O�̑Ή��e�[�u��
	std::map<InputType, std::wstring> inputNameTable_;

	std::vector<bool> currentInput_;	//���݂̓��͏��(�����Ă邩�����ĂȂ���)
	std::vector<bool> lastInput_;		//���O�̓��͏��(���O�����Ă邩�����ĂȂ���)
};

