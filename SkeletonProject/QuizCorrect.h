#pragma once
#include "Scene/Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// ÉNÉCÉY1ÇÃê≥ââÊñ 
/// </summary>

class QuizCorrect : public Scene
{
public:
	QuizCorrect(SceneManager& manager);
	~QuizCorrect(){};
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

	using UpdateFunc_t = void (QuizCorrect::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};