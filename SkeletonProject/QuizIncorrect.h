#pragma once
#include "Scene/Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// ÉNÉCÉY1ÇÃê≥ââÊñ 
/// </summary>

class QuizIncorrect : public Scene
{
public:
	QuizIncorrect(SceneManager& manager);
	~QuizIncorrect();
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

	using UpdateFunc_t = void (QuizIncorrect::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};