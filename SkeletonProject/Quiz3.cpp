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
	//��������
	constexpr int kTimeLimit = 11000;//10000;
	//�I�����̐�
	constexpr int kOptionNum = 2;
	//�ǂ̂��炢�̎��Ԃŉ�ʂ�\�����邩(�傫�����قǒx��)
	constexpr int kFadeInterval = 60;
	//�I�����̎l�p�`�̊p�̊p�x
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

	fadeColor_ = 0x000000;		//�t�F�[�h�̐F(�f�t�H��)
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
		//����
		if (whichAnswer_)
		{
			manager_.ChangeScene(new Quiz3Correct(manager_));
		}
		//�s����
		else
		{
			manager_.ChangeScene(new Quiz3Incorrect(manager_));
		}
		return;
	}
}
void Quiz3::NormalUpdate(const InputState& input)
{
	//�Q�[���̃X�^�[�g���Ԃ��擾
	nowTime_ = GetNowCount();

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
		option2BoxColor_ = GetColor(255, 255, 255);
		option3BoxColor_ = GetColor(255, 255, 255);

		if (input.IsTriggered(InputType::next))
		{
			//����
			whichAnswer_ = false;
			updateFunc_ = &Quiz3::FadeOutUpdate;
		}
	}
	//�I����1���I�𒆂̏ꍇ
	else if (currentInputIndex_ == 1)
	{
		option0BoxColor_ = GetColor(255, 255, 255);
		option1BoxColor_ = GetColor(150, 150, 150);
		option2BoxColor_ = GetColor(255, 255, 255);
		option3BoxColor_ = GetColor(255, 255, 255);

		if (input.IsTriggered(InputType::next))
		{
			//�s����
			whichAnswer_ = true;
			updateFunc_ = &Quiz3::FadeOutUpdate;
		}
	}

	//���̉E���_�������_���傫���ꍇ
	if (timeBoxPosX_ < timeBoxPosX2_)
	{
		//���Ԑ�����\�����̕\���̌���
		timeBoxPosX2_ -= 0.582; //0.582
	}
	//�w�肵�����Ԉȏ�o�����ꍇ
	if ((nowTime_ - startTime_) > kTimeLimit)
	{
		//�s����
		whichAnswer_ = false;
		updateFunc_ = &Quiz3::FadeOutUpdate;
	}
}

void Quiz3::Draw()
{
	//��萔�̕\��
	DrawStringToHandle(Game::kScreenWidth / 2 - 200, 50, L"Q 3 / 5", GetColor(255, 255, 255), problemNumH_);

	//�摜�̕\��
	DrawGraph(120, 200, correctUrlH_, false);
	DrawGraph(120, 300, incorrectUrlH_, false);

	DrawStringToHandle(90, 200, L"A", 0xffffff, problemLetterH_);
	DrawStringToHandle(90, 300, L"B", 0xffffff, problemLetterH_);

	//��蕶�̈͂��̕\��
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 - 300, Game::kScreenWidth - 20, Game::kScreenHeight / 2,
		10, 10, GetColor(255, 255, 255), false);

	//��蕶�̕\��
	DrawStringToHandle(Game::kScreenWidth / 2 - 180, Game::kScreenHeight / 2 - 270, L"2�̂����A�U�T�C�g�͂ǂ��炩?",
		GetColor(255, 255, 255), problemLetterH_);

	//�I�����̔��̕\��
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 100, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 170,
		kBoxAngle, kBoxAngle, option0BoxColor_, true);
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 200, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 270,
		kBoxAngle, kBoxAngle, option1BoxColor_, true);

	//�I����0�̕\��(����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 10, Game::kScreenHeight / 2 + 130, L"A",
		GetColor(0, 0, 0), optionLetterH_);
	//�I����1�̕\��(�s����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 10, Game::kScreenHeight / 2 + 230, L"B",
		GetColor(0, 0, 0), optionLetterH_);

	//�������Ԃ̕\��
	DrawBox(timeBoxPosX_, Game::kScreenHeight / 2 + 20, timeBoxPosX2_, Game::kScreenHeight / 2 + 30,
		GetColor(0, 255, 0), true);
	DrawStringToHandle(Game::kScreenWidth / 2 - 215, Game::kScreenHeight / 2 + 15, L"Time", GetColor(255, 255, 255), timeLetterH_);
	//DrawFormatString(0, 0, GetColor(255, 255, 255), L"%d�b", (kTimeLimit - (nowTime_ - startTime_)) / 1000);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}