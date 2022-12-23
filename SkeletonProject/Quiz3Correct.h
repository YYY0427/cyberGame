#pragma once
#include "Scene/Scene.h"
class InputState;
class SceneManager;

/// <summary>
/// クイズ3の正解画面
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
	int		currentInputIndex_;			//現在選択中のインデックス

	int		option0BoxColor_;			//選択肢0の箱の色
	int		option1BoxColor_;			//選択肢1の箱の色
	bool	whichAnswer_;

	using UpdateFunc_t = void (Quiz3Correct::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};
