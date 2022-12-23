#pragma once
#include "Scene/Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// クイズ3の不正解画面
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
	int	currentInputIndex_;			//現在選択中のインデックス

	int	option0BoxColor_;			//選択肢0の箱の色
	int	option1BoxColor_;			//選択肢1の箱の色
	bool	whichAnswer_;


	using UpdateFunc_t = void (Quiz3Incorrect::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

};