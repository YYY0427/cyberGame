#include "Quiz2Incorrect.h"
#include "DxLib.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Scene/KeyConfigScene.h"
#include "Quiz3.h"
#include "Game.h"

namespace
{
	//どのくらいの時間で画面を表示するか(大きい数ほど遅い)
	constexpr int kFadeInterval = 30;
}

Quiz2Incorrect::Quiz2Incorrect(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&Quiz2Incorrect::FadeInUpdate)
{
	fadeColor_ = 0x000000;		//フェードの色(デフォ黒)
	fadeTimer_ = kFadeInterval;
	fadeValue_ = 255;
	incorrectH_ = CreateFontToHandle(NULL, 50, 10);
	letterH_ = CreateFontToHandle(NULL, 20, 10);
	nextLetterH_ = CreateFontToHandle(NULL, 25, 10);
}

Quiz2Incorrect::~Quiz2Incorrect()
{
}

void Quiz2Incorrect::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &Quiz2Incorrect::NormalUpdate;
	}
}

void Quiz2Incorrect::NormalUpdate(const InputState& input)
{
	//次へのボタンが押されたら次のシーンへ行く
	if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &Quiz2Incorrect::FadeOutUpdate;
	}
}

void Quiz2Incorrect::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval));
	if (++fadeTimer_ == kFadeInterval)
	{
		manager_.ChangeScene(new Quiz3(manager_));
	}
}

void Quiz2Incorrect::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void Quiz2Incorrect::Draw()
{
	constexpr int pw_width = 400;	//ポーズ枠の幅
	constexpr int pw_height = 700;	//ポーズ枠の高さ
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;	//ポーズ枠の左
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;	//ポーズ枠上

	//ポーズウィンドウセロファン(黒い)
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//通常描画に戻す

	//ポーズ中メッセージ
	DrawStringToHandle(pw_start_x + 120, pw_start_y + 140, L"不正解", GetColor(128, 192, 255), incorrectH_);
	DrawStringToHandle(pw_start_x + 150, pw_start_y + 300, L"答えは...", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 185, pw_start_y + 350, L"B", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 420, L"政府や企業を装って偽サイトに", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 450, L"誘導し、IDとパスワードを盗ま ", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 480, L"れるので、SMSからのリンクは", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 510, L"開かないようにしましょう。", GetColor(255, 255, 255), letterH_);

	//ポーズウィンドウ枠線
	DrawRoundRect(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 10, 10, 0xffffff, false);
	DrawRoundRect(pw_start_x + 50, pw_start_y + 570, pw_start_x + pw_width - 50, pw_start_y + pw_height - 50, 10, 10, GetColor(255, 255, 255), true);
	DrawRoundRect(pw_start_x + 55, pw_start_y + 575, pw_start_x + pw_width - 55, pw_start_y + pw_height - 55, 10, 10, GetColor(0, 0, 255), true);
	DrawStringToHandle(pw_start_x + 135, pw_start_y + 595, L"次の問題へ", GetColor(255, 255, 255), nextLetterH_);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
