#include "KeyConfigScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include <DxLib.h>

KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input) : 
	Scene(manager),
	inputState_(input),
	currentInputIndex_(0)
{
}

void KeyConfigScene::Update(const InputState& input)
{
	//const�͂���
	auto& configInput = const_cast<InputState&>(input);
	if (!isEditing)
	{
		if (input.IsTriggered(InputType::keyconf))
		{
			manager_.PopScene();
			return;
		}
		if (input.IsTriggered(InputType::change))
		{
			//const�͂���
			//auto& configInput = const_cast<InputState&>(input);

			configInput.RewriteInputInfo(InputType::prev, InputCategory::keybd, KEY_INPUT_ESCAPE);
			configInput.RewriteInputInfo(InputType::prev, InputCategory::pad, PAD_INPUT_A);
			configInput.RewriteInputInfo(InputType::prev, InputCategory::mouse, MOUSE_INPUT_RIGHT);

			//����L�[������������ꂽ��
			static int count = 0;
			++count;
			char logstr[64] = {};
			sprintf_s(logstr, sizeof(logstr), "%d��L�[�������������܂���\n", count);
			//�o�̓��O�ɕ\��
			OutputDebugStringA(logstr);
		}

		//�㉺�ŉ�鏈��
		const int nameCount = input.inputNameTable_.size();
		if (input.IsTriggered(InputType::up))
		{
			currentInputIndex_ = ((currentInputIndex_ - 1) + nameCount) % nameCount;
		}
		else if (input.IsTriggered(InputType::down))
		{
			currentInputIndex_ = (currentInputIndex_ + 1) % nameCount;
		}
	}
	if (input.IsTriggered(InputType::next))
	{
		isEditing =! isEditing;
		return;
	}

	if (isEditing)
	{
		char keystate[256];
		GetHitKeyStateAll(keystate);
		auto padState = GetJoypadInputState(DX_INPUT_PAD1);
		auto mouseState = GetMouseInput();

		int idx = 0;
		InputType currentType = InputType::max;
		for (const auto& name : inputState_.inputNameTable_)
		{
			if (currentInputIndex_ == idx)
			{
				currentType = name.first;
				break;
			}
			idx++;
		}

		//�L�[�̓���ւ�
		for (int i = 0; i < 256; ++i)
		{
			if (keystate[i])
			{
				configInput.RewriteInputInfo(currentType, InputCategory::keybd, i);
				break;
			}
		}
		if (padState != 0)
		{
			configInput.RewriteInputInfo(currentType, InputCategory::pad, padState);
		}
		if (mouseState != 0)
		{
			configInput.RewriteInputInfo(currentType, InputCategory::mouse, mouseState);
		}
	}
}

void KeyConfigScene::Draw()
{
	constexpr int pw_width = 450;	//�L�[�R���t�B�O�g�̕�
	constexpr int pw_height = 350;	//�L�[�R���t�B�O�g�̍���
	constexpr int pw_start_x = (640 - pw_width) / 2 + 50;	//�L�[�R���t�B�O�g�̍�
	constexpr int pw_start_y = (480 - pw_height) / 2 + 50;	//�L�[�R���t�B�O�g��

	//�L�[�R���t�B�O�E�B���h�E�w�i
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x008800, true);

	//�L�[�R���t�B�O�����b�Z�[�W
	DrawString(pw_start_x + 10, pw_start_y + 10, L"keyconfig...", 0xffffaa);

	//�e�L�[����ׂĕ\��
	auto y = pw_start_y + 30;
	int idx = 0;
	for (const auto& name : inputState_.inputNameTable_)
	{
		int offset = 0;
		unsigned int color = 0xffffff;
		if (currentInputIndex_ == idx)
		{
			offset = 10;
			if (isEditing)
			{
				color = 0xff0000;
			}
		}
		//�e�L�[�̕\��
		int x = pw_start_x + 20 + offset;
		DrawString(x, y, name.second.c_str(), color);

		auto type = name.first;
		auto it = inputState_.inputMapTable_.find(type);
		x += 64;
		DrawString(x, y, L":", color);
		for (const auto& elem : it->second)
		{
			x += 10;
			if (elem.cat == InputCategory::keybd)
			{
				DrawFormatString(x, y, color, L"key = %d", elem.id);
			}
			else if (elem.cat == InputCategory::pad)
			{
				DrawFormatString(x, y, color, L"pad = %d", elem.id);
			}
			else if (elem.cat == InputCategory::mouse)
			{
				DrawFormatString(x, y, color, L"mouse = %d", elem.id);
			}
			x += 100;
		}
		y += 18;
		++idx;
	}
	y += 20;

	//�e�L�[�̕\��
	DrawString(pw_start_x + 100, y, L"�m�肵�܂�", 0xffffff);

	//�L�[�R���t�B�O�E�B���h�E�g��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}
