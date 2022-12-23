#pragma once
#include "Scene/Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// �N�C�Y3�̕s�������
/// </summary>

class Quiz3Incorrect : public Scene
{
public:
	Quiz3Incorrect(SceneManager& manager);
	~Quiz3Incorrect(){};
	virtual void Update(const InputState& input)override;
	void Draw();
private:
	void FadeInUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);

	unsigned int fadeColor_;

	int fadeTimer_;
	int fadeValue_;
	int incorrectH_;
	int letterH_;
	int nextLetterH_;
	int optionLetterH_;
	int	currentInputIndex_;			//���ݑI�𒆂̃C���f�b�N�X

	int	option0BoxColor_;			//�I����0�̔��̐F
	int	option1BoxColor_;			//�I����1�̔��̐F
	bool	whichAnswer_;


	using UpdateFunc_t = void (Quiz3Incorrect::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

};