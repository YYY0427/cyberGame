#pragma once
#include "Scene/Scene.h"
class Quiz3 : public Scene
{
public:
	Quiz3(SceneManager& manager);
	~Quiz3() {}

	void Update(const InputState& input);
	void Draw();
private:
	unsigned int fadeColor_;
	using UpdateFunc_t = void (Quiz3::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

	int		fadeTimer_;
	int		fadeValue_;

	int		timeBoxPosX_;				//���̍���X���W
	float	timeBoxPosX2_;				//���̉E��X���W
	int		startTime_;					//�X�^�[�g���Ԃ̎擾
	int		nowTime_;					//���݂̎��Ԃ̎擾

	int		timeLetterH_;				//Time�̕\���p�n���h��
	int		problemLetterH_;			//��蕶�̕\���p�n���h��
	int		optionLetterH_;				//�I�����̕\���p�n���h��
	int		problemNumH_;				//��萔�\���p�n���h��

	int		correctUrlH_;
	int		incorrectUrlH_;

	int		currentInputIndex_;			//���ݑI�𒆂̃C���f�b�N�X

	int		option0BoxColor_;			//�I����0�̔��̐F
	int		option1BoxColor_;			//�I����1�̔��̐F
	int		option2BoxColor_;			//�I����2�̔��̐F
	int		option3BoxColor_;			//�I����3�̔��̐F

	bool	whichAnswer_;

	void	FadeInUpdate(const InputState& input);
	void	FadeOutUpdate(const InputState& input);
	void	NormalUpdate(const InputState& input);
};

