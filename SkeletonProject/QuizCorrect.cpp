#include "QuizCorrect.h"
#include "DxLib.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Scene/KeyConfigScene.h"
#include "Game.h"
#include "Quiz2.h"

namespace 
{
	//�ǂ̂��炢�̎��Ԃŉ�ʂ�\�����邩(�傫�����قǒx��)
	constexpr int kFadeInterval = 30;
}

QuizCorrect::QuizCorrect(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&QuizCorrect::FadeInUpdate),
	letterH_(-1),
	interval_(0),
	interval2_(0)
{
	fadeColor_ = 0x000000;		//�t�F�[�h�̐F(�f�t�H��)
	fadeTimer_ = kFadeInterval;
	fadeValue_ = 255;
	correctH_  = CreateFontToHandle(NULL, 50, 10);
	letterH_   = CreateFontToHandle(NULL, 20, 10);
	nextLetterH_ = CreateFontToHandle(NULL, 25, 10);
}

void QuizCorrect::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &QuizCorrect::NormalUpdate;
	}
}

void QuizCorrect::NormalUpdate(const InputState& input)
{
	//���ւ̃{�^���������ꂽ�玟�̃V�[���֍s��
	if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &QuizCorrect::FadeOutUpdate;
	}
}

void QuizCorrect::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval));
	if (++fadeTimer_ == kFadeInterval)
	{
		manager_.ChangeScene(new Quiz2(manager_));
	}
}

void QuizCorrect::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void QuizCorrect::Draw()
{
	constexpr int pw_width = 400;	//�|�[�Y�g�̕�
	constexpr int pw_height = 700;	//�|�[�Y�g�̍���
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;	//�|�[�Y�g�̍�
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;	//�|�[�Y�g��

	//�E�B���h�E�Z���t�@��(����)
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�ʏ�`��ɖ߂�

	DrawStringToHandle(pw_start_x + 140, pw_start_y + 140, L"����",	     GetColor(255,   0,   0), correctH_);
	DrawStringToHandle(pw_start_x + 150, pw_start_y + 300, L"������...", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 185, pw_start_y + 350, L"A",         GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x +  50, pw_start_y + 420, L"�p�X���[�h��ݒ肷��Ƃ��́A", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x +  50, pw_start_y + 450, L"�p�啶�� + �p������ + ���� + ", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x +  50, pw_start_y + 480, L"�L����10���ȏオ���S���Ƃ���", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x +  50, pw_start_y + 510, L"��Ă���B", GetColor(255, 255, 255), letterH_);

	//�E�B���h�E�g��
	DrawRoundRect(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 10, 10, 0xffffff, false);
	DrawRoundRect(pw_start_x + 50, pw_start_y + 570, pw_start_x + pw_width - 50, pw_start_y + pw_height - 50, 10, 10, GetColor(255, 255, 255), true);
	DrawRoundRect(pw_start_x + 55, pw_start_y + 575, pw_start_x + pw_width - 55, pw_start_y + pw_height - 55, 10, 10, GetColor(220, 0, 0), true);
	DrawStringToHandle(pw_start_x + 135, pw_start_y + 595, L"���̖���", GetColor(255, 255, 255), nextLetterH_);

	/*interval_++;
	if (interval_ >= 60)
	{
		DrawStringToHandle(pw_start_x + 135, pw_start_y + 595, L"���̖���", GetColor(255, 255, 255), nextLetterH_);
		interval2_++;
		if (interval2_ >= 60)
		{
			interval_ = 0;
			interval2_ = 0;
		}
	}*/

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
