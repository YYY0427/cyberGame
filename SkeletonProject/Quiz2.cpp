#include "Quiz2.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Game.h"
#include "Quiz2Incorrect.h"
#include "Quiz2Correct.h"
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

Quiz2::Quiz2(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&Quiz2::FadeInUpdate),
	nowTime_(0),
	timeLetterH_(-1),
	problemLetterH_(-1),
	currentInputIndex_(0),
	urlH_(-1),
	whichAnswer_(false)
{
	startTime_ = GetNowCount();
	timeBoxPosX_ = Game::kScreenWidth / 2 - 170;
	timeBoxPosX2_ = Game::kScreenWidth - 70;
	timeLetterH_ = CreateFontToHandle(NULL, 15, 10);
	problemLetterH_ = CreateFontToHandle(NULL, 20, 15);
	optionLetterH_ = CreateFontToHandle(NULL, 16, 10);
	problemNumH_ = CreateFontToHandle(NULL, 16, 10);

	fadeColor_ = 0x000000;		//�t�F�[�h�̐F(�f�t�H��)
	fadeTimer_ = kFadeInterval;
	fadeValue_ = 255;

	urlH_ = my::MyLoadGraph(L"Data/img/URL.jpg");

	option0BoxColor_ = GetColor(255, 255, 255);
	option1BoxColor_ = GetColor(255, 255, 255);
	option2BoxColor_ = GetColor(255, 255, 255);
	option3BoxColor_ = GetColor(255, 255, 255);
}

void Quiz2::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void Quiz2::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &Quiz2::NormalUpdate;
	}
}
void Quiz2::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval));
	if (++fadeTimer_ == 52)
	{
		//����
		if (whichAnswer_)
		{
			manager_.ChangeScene(new Quiz2Correct(manager_));
		}
		//�s����
		else
		{
			manager_.ChangeScene(new Quiz2Incorrect(manager_));
		}
		return;
	}
}
void Quiz2::NormalUpdate(const InputState& input)
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
			//�s����
			whichAnswer_ = false;
			updateFunc_ = &Quiz2::FadeOutUpdate;
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
			//����
			whichAnswer_ = true;
			updateFunc_ = &Quiz2::FadeOutUpdate;
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
		updateFunc_ = &Quiz2::FadeOutUpdate;
	}
}

void Quiz2::Draw()
{
	//��萔�̕\��
	DrawStringToHandle(Game::kScreenWidth / 2 - 200, 50, L"Q 2 / 5", GetColor(255, 255, 255), problemNumH_);

	//�摜�̕\��
	DrawGraph(50, 180, urlH_, false);

	//��蕶�̈͂��̕\��
	DrawRoundRect(Game::kScreenWidth - 470, Game::kScreenHeight / 2 - 300, Game::kScreenWidth - 30, Game::kScreenHeight / 2 + 50,
		10, 10, GetColor(255, 255, 255), false);

	//��蕶�̕\��
	DrawStringToHandle(Game::kScreenWidth / 2 - 200, Game::kScreenHeight / 2 - 280, L"���̂悤��SMS���͂����B\n�������Ώۖ@�͂ǂ��炩?",
		GetColor(255, 255, 255), problemLetterH_);

	//�I�����̔��̕\��
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 130, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 200,
		kBoxAngle, kBoxAngle, option0BoxColor_, true);
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 230, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 300,
		kBoxAngle, kBoxAngle, option1BoxColor_, true);

	//�I����0�̕\��(�s����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 110, Game::kScreenHeight / 2 + 160, L"A.  URL�̃����N���J��",
		GetColor(0, 0, 0), optionLetterH_);
	//�I����1�̕\��(����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 110, Game::kScreenHeight / 2 + 260, L"B.  �����T�C�g����m�F����",
		GetColor(0, 0, 0), optionLetterH_);

	//�������Ԃ̕\��
	DrawBox(timeBoxPosX_, Game::kScreenHeight / 2 + 70, timeBoxPosX2_, Game::kScreenHeight / 2 + 80,
		GetColor(0, 255, 0), true);
	DrawStringToHandle(Game::kScreenWidth / 2 - 210, Game::kScreenHeight / 2 + 65, L"Time", GetColor(255, 255, 255), timeLetterH_);
	//DrawFormatString(0, 0, GetColor(255, 255, 255), L"%d�b", (kTimeLimit - (nowTime_ - startTime_)) / 1000);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}