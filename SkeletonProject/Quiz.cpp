#include "Quiz.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Game.h"
#include "QuizIncorrect.h"
#include "QuizCorrect.h"
#include <DxLib.h>

namespace
{
	//��������
	constexpr int kTimeLimit = 11000;//10000;
	//�I�����̐�
	constexpr int kOptionNum = 4;
	//�ǂ̂��炢�̎��Ԃŉ�ʂ�\�����邩(�傫�����قǒx��)
	constexpr int kFadeInterval = 60;
	//�I�����̎l�p�`�̊p�̊p�x
	constexpr int kBoxAngle = 15;
}

Quiz::Quiz(SceneManager& manager) : 
	Scene(manager),
	updateFunc_(&Quiz::FadeInUpdate),
	nowTime_(0),
	timeLetterH_(-1),
	problemLetterH_(-1),
	currentInputIndex_(0),
	whichAnswer_(false)
{
	startTime_		= GetNowCount();
	timeBoxPosX_	= Game::kScreenWidth / 2 - 170;
	timeBoxPosX2_	= Game::kScreenWidth - 70;
	timeLetterH_	= CreateFontToHandle(NULL, 15, 10);
	problemLetterH_ = CreateFontToHandle(NULL, 22, 20);
	optionLetterH_	= CreateFontToHandle(NULL, 16, 10);
	problemNumH_    = CreateFontToHandle(NULL, 16, 10);

	fadeColor_ = 0x000000;		//�t�F�[�h�̐F(�f�t�H��)
	fadeTimer_  = kFadeInterval;
	fadeValue_ = 255;

	option0BoxColor_ = GetColor(255, 255, 255);
	option1BoxColor_ = GetColor(255, 255, 255);
	option2BoxColor_ = GetColor(255, 255, 255);
	option3BoxColor_ = GetColor(255, 255, 255);
}

void Quiz::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void Quiz::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &Quiz::NormalUpdate;
	}
}
void Quiz::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(kFadeInterval));
	if (++fadeTimer_ == 52)
	{
		//����
		if (whichAnswer_)
		{
			manager_.ChangeScene(new QuizCorrect(manager_));
		}
		//�s����
		else
		{
			manager_.ChangeScene(new QuizIncorrect(manager_));
		}
		return;
	}
}
void Quiz::NormalUpdate(const InputState& input)
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
			whichAnswer_ = true;
			updateFunc_ = &Quiz::FadeOutUpdate;
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
			whichAnswer_ = false;
			updateFunc_ = &Quiz::FadeOutUpdate;
		}
	}
	//�I����2���I�𒆂̏ꍇ
	else if (currentInputIndex_ == 2)
	{
		option0BoxColor_ = GetColor(255, 255, 255);
		option1BoxColor_ = GetColor(255, 255, 255);
		option2BoxColor_ = GetColor(150, 150, 150);
		option3BoxColor_ = GetColor(255, 255, 255);

		if (input.IsTriggered(InputType::next))
		{
			//�s����
			whichAnswer_ = false;
			updateFunc_ = &Quiz::FadeOutUpdate;
		}
	}
	//�I����3���I�𒆂̏ꍇ
	else if (currentInputIndex_ == 3)
	{
		option0BoxColor_ = GetColor(255, 255, 255);
		option1BoxColor_ = GetColor(255, 255, 255);
		option2BoxColor_ = GetColor(255, 255, 255);
		option3BoxColor_ = GetColor(150, 150, 150);

		if (input.IsTriggered(InputType::next))
		{
			//�s����
			whichAnswer_ = false;
			updateFunc_ = &Quiz::FadeOutUpdate;
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
		updateFunc_ = &Quiz::FadeOutUpdate;
	}

	/*if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &Quiz::FadeOutUpdate;
		fadeColor_ = 0xff0000;
	}
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}*/
}

void Quiz::Draw()
{
	//��萔�̕\��
	DrawStringToHandle(Game::kScreenWidth / 2 - 200, 50, L"Q 1 / 5", GetColor(255, 255, 255), problemNumH_);

	//��蕶�̈͂��̕\��
	DrawRoundRect(Game::kScreenWidth - 450 , Game::kScreenHeight / 2 - 300, Game::kScreenWidth - 50, Game::kScreenHeight / 2 - 150,
		10, 10, GetColor(255, 255, 255), false);

	//��蕶�̕\��
	DrawStringToHandle(Game::kScreenWidth / 2 - 180, Game::kScreenHeight / 2 - 245, L"�p�\�R���̃p�X���[�h�����߂����B\n���̒��ōł����S�ȃp�X���[�h�́H", 
		GetColor(255, 255, 255), problemLetterH_);

	//�I�����̔��̕\��
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 - 50, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 20,
		kBoxAngle, kBoxAngle, option0BoxColor_, true);
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 50, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 120,
		kBoxAngle, kBoxAngle, option1BoxColor_, true);
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 150, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 220,
		kBoxAngle, kBoxAngle, option2BoxColor_, true);
	DrawRoundRect(Game::kScreenWidth - 450, Game::kScreenHeight / 2 + 250, Game::kScreenWidth - 50, Game::kScreenHeight / 2 + 320,
		kBoxAngle, kBoxAngle, option3BoxColor_, true);

	//�I����0�̕\��(����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 75, Game::kScreenHeight / 2 - 22, L"A.  Yusaku!115",
		GetColor(0, 0, 0), optionLetterH_);
	//�I����1�̕\��(�s����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 75, Game::kScreenHeight / 2 + 78, L"B.  ryuta29",
		GetColor(0, 0, 0), optionLetterH_);
	//�I����2�̕\��(�s����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 75, Game::kScreenHeight / 2 + 178, L"C.  20040105",
		GetColor(0, 0, 0), optionLetterH_);
	//�I����3�̕\��(�s����)
	DrawStringToHandle(Game::kScreenWidth / 2 - 75, Game::kScreenHeight / 2 + 278, L"D.  Password4",
		GetColor(0, 0, 0), optionLetterH_);

	//�������Ԃ̕\��
	DrawBox(timeBoxPosX_, Game::kScreenHeight / 2 - 100, timeBoxPosX2_, Game::kScreenHeight / 2 - 90,
		GetColor(0, 255, 0), true);
	DrawStringToHandle(Game::kScreenWidth / 2 - 210, Game::kScreenHeight / 2 - 105, L"Time", GetColor(255, 255, 255), timeLetterH_);
	//DrawFormatString(0, 0, GetColor(255, 255, 255), L"%d�b", (kTimeLimit - (nowTime_ - startTime_)) / 1000);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}