#include <DxLib.h>
#include <cassert>
#include "Game.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "InputState.h"
#include "DrawFunctions.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1) 
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	
	InputState input;

	SceneManager sceneManager;
	sceneManager.ChangeScene(new TitleScene(sceneManager));

	while (ProcessMessage() != -1) 
	{
		LONGLONG  time = GetNowHiPerformanceCount();

		ClearDrawScreen();

		input.Update();

		sceneManager.Update(input);

		sceneManager.Draw();

		ScreenFlip();

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667){}
	}

	DxLib_End();

	return 0;
}