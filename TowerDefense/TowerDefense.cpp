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
	TowerDef* towers[8];
	for (int i = 0; i < 8; i++) {
		towers[i] = new TowerDef(0, 0, 0, false);
	}
	for (int i = 0; i < 8; i++) gameMap.towerBrake(i);
	std::cout << "Добро пожаловать в Tower Defence!\n";
	bool gameOver = false;
	int wave = 0;
	int booster = 0;
	EnemyManager enemyManager;
	enemyManager.addEnemy(std::make_unique<Enemy>("Zombe", 4, 100, 10, 'Z', std::make_unique<NormalMove>()));
	enemyManager.addEnemy(std::make_unique<Enemy>("Skeleton", 2, 30, 20, 'S', std::make_unique<NormalMove>()));
	enemyManager.addEnemy(std::make_unique<Enemy>("Angry Zombe", 6, 75, 20, 'A', std::make_unique<AdaptiveMove>()));
	enemyManager.addEnemy(std::make_unique<Enemy>("Fly", 1, 5, 10, '*', std::make_unique<NormalMove>()));
	enemyManager.addEnemy(std::make_unique<BossEnemy>());
	enemyManager.addEnemy(std::make_unique<FastEnemy>());
	while (!gameOver) {
		system("cls");
		int choice;
		int mon;
		std::cout << "Ваши действия:\n1.Начать новую волну врагов\n2.Открыть магазин\n3.Информация о врагах\n4.Выход из игры\n";
		std::cin >> choice;
		switch (choice) {
		case 1://Волна врагов
			wave++;
			if (wave % 5 == 0) booster++;
			mon = game.getEnemyMoney() / 20;
			while (game.getEnemyMoney() > 0) {
				game.buyEnemy(wave, enemyManager);
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
		case 3://Сортировка и поиск
			std::cout << "Ваши действия:\n1.Открыть полный список врагов\n2.Поиск\n3.Выход\n";
			int poisk;
			std::cin >> poisk;
			switch (poisk) {
			case 1:
				std::cout << "1.Без сортировки\n2.Сортировка по здоровью\n3.Сортировка по стоимости(редкости)\n4.Сортировка по урону\n";
				int poisk2;
				std::cin >> poisk2;
				if (poisk2 == 2) {
					enemyManager.sortByHp();
				}
				else if (poisk2 == 3) {
					enemyManager.sortByCost();
				}
				else if (poisk2 == 4) {
					enemyManager.sortByDmg();
				}
				std::cout << "1.Краткая ифнормация о врагах\n2.Полная инфмормация и описание каждого врага\nЛюбая другая клавиша - выход.\n";
				std::cin >> poisk2;
				if (poisk2 == 1) {
					enemyManager.showAll(false);
				}
				else if (poisk2 == 2) {
					enemyManager.showAll(true);
				}
				system("pause");
				break;
			case 2:
				std::cout << "1.Поиск по имени\n2.Поиск по картинке\n3.Выход\n";
				int poisk3;
				std::cin >> poisk3;
				if (poisk3 == 1) {
					std::string name;
					std::cout << "Введите имя врага(латиница): ";
					std::cin >> name;
					std::cout << "При успешном поиске:\n1.Краткая ифнормация о врагe\n2.Полная инфмормация и описание\n";
					int poisk4;
					do { 
						std::cin >> poisk4;
					} while (poisk4 != 1 && poisk4 != 2);
					if (poisk4 == 1) {
						enemyManager.findEnemyByName(name, false);
					}
					else {
						enemyManager.findEnemyByName(name, true);
					}
				}
				else if (poisk3 == 2) {
					char pct;
					std::cout << "Введите картинку врага(латиница): ";
					std::cin >> pct;
					std::cout << "При успешном поиске:\n1.Краткая ифнормация о врагe\n2.Полная инфмормация и описание\n";
					int poisk4;
					do {
						std::cin >> poisk4;
					} while (poisk4 != 1 && poisk4 != 2);
					if (poisk4 == 1) {
						enemyManager.findEnemyByPct(pct, false);
					}
					else {
						enemyManager.findEnemyByPct(pct, true);
					}
				}
				system("pause");
				break;
			case 3:
				break;
			default:
				std::cout << "Неверный ввод!\n";
				break;
			}
			break;
		case 4://Выход из игры
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

