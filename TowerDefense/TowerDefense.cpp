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
	int wave = 0;
	int booster = 0;
	while (!gameOver) {
		system("cls");
		int choice;
		int mon;
		std::cout << "Ваши действия:\n1.Начать новую волну врагов\n2.Открыть магазин\n3.Выход из игры\n";
		std::cin >> choice;
		switch (choice) {
		case 1://Волна врагов
			wave++;
			if (wave % 5 == 0) booster++;
			mon = game.getEnemyMoney() / 20;
			while (game.getEnemyMoney() > 1) {
				game.buyEnemy(wave);
			}
			for (int i = 0; i < mon; i++) {
				game.upenemys();
			}
			if (!game.Wave(mainTower, towers, gameMap)) {
				gameOver = true;
			}
			break;
		case 2://Магазин
			shop.openShop();
			mon = game.getMoney();
			shop.Info(mainTower, mon, towers, gameMap, booster);
			game.setMoney(mon);
			break;
		case 3://Выход из игры
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

