#pragma once
#include "Scene/Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// �N�C�Y1�̐������
/// </summary>

class Quiz2Incorrect : public Scene
{
public:
	Quiz2Incorrect(SceneManager& manager);
	~Quiz2Incorrect();
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

	using UpdateFunc_t = void (Quiz2Incorrect::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};
