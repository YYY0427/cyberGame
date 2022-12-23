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

	int		timeBoxPosX_;				//箱の左上X座標
	float	timeBoxPosX2_;				//箱の右下X座標
	int		startTime_;					//スタート時間の取得
	int		nowTime_;					//現在の時間の取得

	int		timeLetterH_;				//Timeの表示用ハンドル
	int		problemLetterH_;			//問題文の表示用ハンドル
	int		optionLetterH_;				//選択肢の表示用ハンドル
	int		problemNumH_;				//問題数表示用ハンドル

	int		correctUrlH_;
	int		incorrectUrlH_;

	int		currentInputIndex_;			//現在選択中のインデックス

	int		option0BoxColor_;			//選択肢0の箱の色
	int		option1BoxColor_;			//選択肢1の箱の色
	int		option2BoxColor_;			//選択肢2の箱の色
	int		option3BoxColor_;			//選択肢3の箱の色

	bool	whichAnswer_;

	void	FadeInUpdate(const InputState& input);
	void	FadeOutUpdate(const InputState& input);
	void	NormalUpdate(const InputState& input);
};

