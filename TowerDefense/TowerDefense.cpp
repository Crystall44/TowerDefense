#define _CRT_CECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "head/Structures.h"

int main()
{
	Tower mainTower;
	Map gameMap;
	Shop shop;
	srand(time(NULL));
	setlocale(LC_ALL, "RUS");
	Game game;
	TowerDef towers[8];
	for (int i = 0; i < 8; i++) gameMap.towerBrake(i);
	std::cout << "Добро пожаловать в Tower Defence!\n";
	bool gameOver = false;
	while (!gameOver) {
		system("cls");
		int choice;
		std::cout << "Ваши действия:\n1.Начать новую волну врагов\n2.Открыть магазин\n3.Выход из игры\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			while (game.enemyMoney > 0) {
				game.buyEnemy();
			}
			if (!game.Wave(mainTower, towers, gameMap)) {
				gameOver = true;
			}
			break;
		case 2:
			shop.openShop();
			shop.Info(mainTower, game.money, towers, gameMap);
			break;
		case 3:
			gameOver = true;
			std::cout << "Выход из игры. Спасибо за игру!\n";
			break;
		default:
			std::cout << "Неверный ввод!\n";
			break;
		}
		if (!mainTower.isAlive()) {
			std::cout << "Главная башня разрушена! Вы проиграли.\n";
			gameOver = true;
		}
	}
	return 0;
}

