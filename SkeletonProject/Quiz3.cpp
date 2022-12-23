#include "Quiz3.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Game.h"
#include "Quiz3Incorrect.h"
#include "Quiz3Correct.h"
#include "DrawFunctions.h"
#include <DxLib.h>

namespace
{
	//制限時間
	constexpr int kTimeLimit = 11000;//10000;
	//選択肢の数
	constexpr int kOptionNum = 2;
	//どのくらいの時間で画面を表示するか(大きい数ほど遅い)
	constexpr int kFadeInterval = 60;
	//選択肢の四角形の角の角度
	constexpr int kBoxAngle = 15;
}

Quiz3::Quiz3(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&Quiz3::FadeInUpdate),
	nowTime_(0),
	timeLetterH_(-1),
	problemLetterH_(-1),
	currentInputIndex_(0),
	correctUrlH_(-1),
	incorrectUrlH_(-1),
	whichAnswer_(false)
{
	startTime_ = GetNowCount();
	timeBoxPosX_ = Game::kScreenWidth / 2 - 170;
	timeBoxPosX2_ = Game::kScreenWidth - 70;
	timeLetterH_ = CreateFontToHandle(NULL, 15, 10);
	problemLetterH_ = CreateFontToHandle(NULL, 22, 20);
	optionLetterH_ = CreateFontToHandle(NULL, 20, 15);
	problemNumH_   = CreateFontToHandle(NULL, 16, 10);

	correctUrlH_   = my::MyLoadGraph(L"Data/img/correctURL.jpg");
	incorrectUrlH_ = my::MyLoadGraph(L"Data/img/incorrectURL.jpg");

	fadeColor_ = 0x000000;		//フェードの色(デフォ黒)
	fadeTimer_ = kFadeInterval;
	fadeValue_ = 255;

	option0BoxColor_ = GetColor(255, 255, 255);
	option1BoxColor_ = GetColor(255, 255, 255);
	option2BoxColor_ = GetColor(255, 255, 255);
	option3BoxColor_ = GetColor(255, 255, 255);
}

void Quiz3::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void Quiz3::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &Quiz3::NormalUpdate;
	}
}
void Quiz3::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval));
	if (++fadeTimer_ == 52)
	{
		//正解
		if (whichAnswer_)
		{
			manager_.ChangeScene(new Quiz3Correct(manager_));
		}
		//不正解
		else
		{
			manager_.ChangeScene(new Quiz3Incorrect(manager_));
		}
		return;
	}
}
void Quiz3::NormalUpdate(const InputState& input)
{
	//ゲームのスタート時間を取得
	nowTime_ = GetNowCount();

	//選択肢を上下で回す処理
	const int nameCount = kOptionNum;
	if (input.IsTriggered(InputType::up))
	{
		currentInputIndex_ = ((currentInputIndex_ - 1) + nameCount) % nameCount;
	}
	else if (input.IsTriggered(InputType::down))
	{
		currentInputIndex_ = (currentInputIndex_ + 1) % nameCount;
	}

	//選択肢0が選択中の場合
	if (currentInputIndex_ == 0)
	{
		option0BoxColor_ = GetColor(150, 150, 150);
		option1BoxColor_ = GetColor(255, 255, 255);
		option2BoxColor_ = GetColor(255, 255, 255);
		option3BoxColor_ = GetColor(255, 255, 255);

		if (input.IsTriggered(InputType::next))
		{
			//正解
			whichAnswer_ = false;
			updateFunc_ = &Quiz3::FadeOutUpdate;
		}
	}
	//選択肢1が選択中の場合
	else if (currentInputIndex_ == 1)
	{
		option0BoxColor_ = GetColor(255, 255, 255);
		option1BoxColor_ = GetColor(150, 150, 150);
		option2BoxColor_ = GetColor(255, 255, 255);
		option3BoxColor_ = GetColor(255, 255, 255);

		if (input.IsTriggered(InputType::next))
		{
			//不正解
			whichAnswer_ = true;
			updateFunc_ = &Quiz3::FadeOutUpdate;
		}
	}

	//箱の右頂点が左頂点より大きい場合
	if (timeBoxPosX_ < timeBoxPosX2_)
	{
		//時間制限を表す箱の表示の減少
		timeBoxPosX2_ -= 0.582; //0.582
	}
	//指定した時間以上経った場合
	if ((nowTime_ - startTime_) > kTimeLimit)
	{
		//不正解
		whichAnswer_ = false;
		updateFunc_ = &Quiz3::FadeOutUpdate;
	}
}

void Quiz3::Draw()
{
	//問題数の表示
	DrawStringToHandle(Game::kScreenWidth / 2 - 200, 50, L"Q 3 / 5", GetColor(255, 255, 255), problemNumH_);

	//画像の表示
	DrawGraph(120, 200, correctUrlH_, false);
	DrawGraph(120, 300, incorrectUrlH_, false);

	DrawStringToHandle(90, 200, L"A", 0xffffff, problemLetterH_);
	DrawStringToHandle(90, 300, L"B", 0xffffff, problemLetterH_);

	//問題文の囲いの表示
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 - 300, Game::kScreenWidth - 20, Game::kScreenHeight / 2,
		10, 10, GetColor(255, 255, 255), false);

	//問題文の表示
	DrawStringToHandle(Game::kScreenWidth / 2 - 180, Game::kScreenHeight / 2 - 270, L"2つのうち、偽サイトはどちらか?",
		GetColor(255, 255, 255), problemLetterH_);

	//選択肢の箱の表示
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 100, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 170,
		kBoxAngle, kBoxAngle, option0BoxColor_, true);
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 200, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 270,
		kBoxAngle, kBoxAngle, option1BoxColor_, true);

	//選択肢0の表示(正解)
	DrawStringToHandle(Game::kScreenWidth / 2 - 10, Game::kScreenHeight / 2 + 130, L"A",
		GetColor(0, 0, 0), optionLetterH_);
	//選択肢1の表示(不正解)
	DrawStringToHandle(Game::kScreenWidth / 2 - 10, Game::kScreenHeight / 2 + 230, L"B",
		GetColor(0, 0, 0), optionLetterH_);

	//制限時間の表示
	DrawBox(timeBoxPosX_, Game::kScreenHeight / 2 + 20, timeBoxPosX2_, Game::kScreenHeight / 2 + 30,
		GetColor(0, 255, 0), true);
	DrawStringToHandle(Game::kScreenWidth / 2 - 215, Game::kScreenHeight / 2 + 15, L"Time", GetColor(255, 255, 255), timeLetterH_);
	//DrawFormatString(0, 0, GetColor(255, 255, 255), L"%d秒", (kTimeLimit - (nowTime_ - startTime_)) / 1000);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}