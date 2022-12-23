#include "Quiz3Incorrect.h"
#include "DxLib.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Scene/KeyConfigScene.h"
#include "Quiz3.h"
#include "Quiz.h"
#include "Game.h"

namespace
{
	//�ǂ̂��炢�̎��Ԃŉ�ʂ�\�����邩(�傫�����قǒx��)
	constexpr int kFadeInterval = 30;
	//�I�����̎l�p�`�̊p�̊p�x
	constexpr int kBoxAngle = 15;
	//�I�����̐�
	constexpr int kOptionNum = 2;
}

Quiz3Incorrect::Quiz3Incorrect(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&Quiz3Incorrect::FadeInUpdate),
	letterH_(-1)
{
	optionLetterH_ = CreateFontToHandle(NULL, 18, 10);
	option0BoxColor_ = GetColor(255, 255, 255);
	option1BoxColor_ = GetColor(255, 255, 255);
	fadeColor_ = 0x000000;		//�t�F�[�h�̐F(�f�t�H��)
	fadeTimer_ = kFadeInterval;
	fadeValue_ = 255;
	incorrectH_ = CreateFontToHandle(NULL, 50, 10);
	letterH_ = CreateFontToHandle(NULL, 20, 10);
	nextLetterH_ = CreateFontToHandle(NULL, 25, 10);
}

void Quiz3Incorrect::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &Quiz3Incorrect::NormalUpdate;
	}
}

void Quiz3Incorrect::NormalUpdate(const InputState& input)
{
	//�I�������㉺�ŉ񂷏���
	const int nameCount = kOptionNum;
	if (input.IsTriggered(InputType::up))
	{
		currentInputIndex_ = ((currentInputIndex_ - 1) + nameCount) % nameCount;
	}
	else if (input.IsTriggered(InputType::down))
	{
		currentInputIndex_ = (currentInputIndex_ + 1) % nameCount;
	}

	//�I����0���I�𒆂̏ꍇ
	if (currentInputIndex_ == 0)
	{
		option0BoxColor_ = GetColor(150, 150, 150);
		option1BoxColor_ = GetColor(255, 255, 255);

		if (input.IsTriggered(InputType::next))
		{
			//����
			whichAnswer_ = true;
			updateFunc_ = &Quiz3Incorrect::FadeOutUpdate;
		}
	}
	//�I����1���I�𒆂̏ꍇ
	else if (currentInputIndex_ == 1)
	{
		option0BoxColor_ = GetColor(255, 255, 255);
		option1BoxColor_ = GetColor(150, 150, 150);

		if (input.IsTriggered(InputType::next))
		{
			//�s����
			whichAnswer_ = false;
			updateFunc_ = &Quiz3Incorrect::FadeOutUpdate;
		}
	}
}

void Quiz3Incorrect::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval));
	if (++fadeTimer_ == kFadeInterval)
	{
		if (whichAnswer_)
		{
			manager_.ChangeScene(new Quiz(manager_));
		}
		else
		{
			DxLib_End();
		}
		return;
	}
}

void Quiz3Incorrect::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void Quiz3Incorrect::Draw()
{
	constexpr int pw_width = 400;	//�|�[�Y�g�̕�
	constexpr int pw_height = 700;	//�|�[�Y�g�̍���
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;	//�|�[�Y�g�̍�
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;	//�|�[�Y�g��

	DrawStringToHandle(pw_start_x + 140, pw_start_y + 100, L"�s����", GetColor(128, 192, 255), incorrectH_);
	DrawStringToHandle(pw_start_x + 150, pw_start_y + 200, L"������...", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 185, pw_start_y + 250, L"B", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 320, L"�ꌩ�������T�C�g�Ɍ����Ă�", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 350, L"URL��������������A���}�[�N", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 380, L"�����Ă��Ȃ������肷��̂�", GetColor(255, 255, 255), letterH_);
	DrawStringToHandle(pw_start_x + 50, pw_start_y + 410, L"�C��t���܂��傤�B", GetColor(255, 255, 255), letterH_);

	////�E�B���h�E�g��
	//DrawRoundRect(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 10, 10, 0xffffff, false);
	//DrawRoundRect(pw_start_x + 50, pw_start_y + 570, pw_start_x + pw_width - 50, pw_start_y + pw_height - 50, 10, 10, GetColor(255, 255, 255), true);
	//DrawRoundRect(pw_start_x + 55, pw_start_y + 575, pw_start_x + pw_width - 55, pw_start_y + pw_height - 55, 10, 10, GetColor(220, 0, 0), true);
	//DrawStringToHandle(pw_start_x + 135, pw_start_y + 595, L"���̖���", GetColor(255, 255, 255), nextLetterH_);

	//�I�����̔��̕\��
	DrawRoundRect(Game::kScreenWidth - 350, Game::kScreenHeight / 2 + 150, Game::kScreenWidth - 100, Game::kScreenHeight / 2 + 220,
		kBoxAngle, kBoxAngle, option0BoxColor_, true);
	DrawRoundRect(Game::kScreenWidth - 350, Game::kScreenHeight / 2 + 250, Game::kScreenWidth - 100, Game::kScreenHeight / 2 + 320,
		kBoxAngle, kBoxAngle, option1BoxColor_, true);

	//�I����0�̕\��(����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 50, Game::kScreenHeight / 2 + 160, L"������x�V��",
		GetColor(0, 0, 0), optionLetterH_);
	//�I����1�̕\��(�s����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 50, Game::kScreenHeight / 2 + 260, L"�Q�[�����I���",
		GetColor(0, 0, 0), optionLetterH_);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
