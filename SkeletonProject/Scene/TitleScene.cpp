#include "TitleScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include <DxLib.h>
#include "../DrawFunctions.h"
#include "../Game.h"
#include "../Quiz.h"

void TitleScene::FadeInUpdate(const InputState& input)
{
	//�^�������珙�X�ɕ\������ꍇ�ꍇ
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));

	//�O���f�[�V�������g���ď��X�ɕ\������ꍇ
	/*fadeTimer_;*/
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &TitleScene::NormalUpdate;
		fadeValue_ = 0;
	}
}

void TitleScene::NormalUpdate(const InputState& input)
{
	//���ւ̃{�^���������ꂽ�玟�̃V�[���֍s��
	if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
}

void TitleScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval)
	{
		manager_.ChangeScene(new Quiz(manager_));
		return;
	}
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::FadeInUpdate)
{
	gradH_		  = my::MyLoadGraph(L"Data/img/gradetion.jpg");
	titleLetterH_ = CreateFontToHandle(NULL, 30, 10);
	letterH_	  =	CreateFontToHandle(NULL, 20, 10);
	/*fadeTimer_ = 255;*/
}

TitleScene::~TitleScene()
{
	//DeleteGraph(titleH_);
}

void TitleScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::Draw()
{
	//���ʂ̕`��
	//DrawRotaGraph(320,240,0.3,0.0,titleH_,true);
	DrawStringToHandle(Game::kScreenWidth / 2 - 200, Game::kScreenHeight / 2 - 100, L"�T�C�o�[�Z�L�����e�B�N�C�Y", GetColor(255, 255, 255), titleLetterH_);

	interval_++;
	if (interval_ >= 60)
	{
		DrawStringToHandle(Game::kScreenWidth / 2 - 100, Game::kScreenHeight / 2 + 200, L"Enter�������Ă�������", GetColor(255, 255, 255), letterH_);
		interval2_++;
		if (interval2_ >= 60)
		{
			interval_ = 0;
			interval2_ = 0;
		}
	}

	//�����珑���摜�ƁA���łɕ`�悳��Ă���X�N���[���Ƃ�
	//�u�����h�̎d�����w�肵�Ă���B
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	
	/*SetDrawBlendMode(DX_BLENDMODE_MULA, fadeValue_);
	//��ʑS�̂��e�N�X�`���œh��Ԃ�
	DrawGraph(0, 0, gradH_, true);*/

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}