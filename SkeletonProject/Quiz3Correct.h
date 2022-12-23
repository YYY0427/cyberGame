#pragma once
#include "Scene/Scene.h"
class InputState;
class SceneManager;

/// <summary>
/// �N�C�Y3�̐������
/// </summary>

class Quiz3Correct : public Scene
{
public:
	Quiz3Correct(SceneManager& manager);
	~Quiz3Correct() {};
	virtual void Update(const InputState& input) override;
	void Draw();
private:

	void FadeInUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);

	unsigned int fadeColor_;

	int fadeTimer_;
	int fadeValue_;

	int correctH_;
	int letterH_;
	int nextLetterH_;

	int interval_;
	int interval2_;

	int optionLetterH_;
	int		currentInputIndex_;			//���ݑI�𒆂̃C���f�b�N�X

	int		option0BoxColor_;			//�I����0�̔��̐F
	int		option1BoxColor_;			//�I����1�̔��̐F
	bool	whichAnswer_;

	using UpdateFunc_t = void (Quiz3Correct::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};
