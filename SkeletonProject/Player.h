#pragma once

class Enemy;

class Player
{
public:
	void Dameged();

private:
	Enemy* enemy_ = nullptr;
};

