#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	~TitleScene();

	void Update(const InputState& input);
	void Draw();
private:
	static constexpr int fade_interval = 60;

	int titleLetterH_ = -1;			//�^�C�g�������̑傫��
	int letterH_ = -1;				//�����̑傫��
	int gradH_ = -1;
	int fadeTimer_ = fade_interval;	//�t�F�[�h�^�C�}�[
	int fadeValue_ = 255;			//����`�Ƃ̃u�����h�
	int interval_ = 0;
	int interval2_ = 0;

	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdate(const InputState& input);
	//�ʏ��Ԃ�Update�֐�
	void NormalUpdate(const InputState& input);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdate(const InputState& input);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);

};