#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "head/Structures.h"

void Restart(Round* round, Shop* shop, Tower* tower) {
	(*round).num = 1;
	(*round).money = 10;
	(*round).boss = false;
	(*shop).cash = 80;
	for (int i = 0; i < 8; i++) (*shop).towers[i] = 0;
	(*tower).lvl = 0;
	(*tower).hp = 3000;
	for (int i = 0; i < 256; i++) (*round).spawn[i] = -1;
}

void RoundEnd(Round* round, Shop* shop, Tower* tower) {
	(*shop).cash += (*round).num * 20;
	(*round).num += 1;
	if ((*round).num % 10 == 0) (*round).boss = true;
	(*round).money = (*round).num * 10;
	(*tower).hp += 300;
	for (int i = 0; i < 256; i++) (*round).spawn[i] = -1;
}



void EnemyBuy(Enemy enemy, Round* round, int i, int j) {
	(*round).money = (*round).money - enemy.cost;
	(*round).spawn[i] = j;
}

void EnemySpawn(Round* round, Enemy enemy[], int i, Map* map) {
	Enemy spawned;
	spawned = enemy[(*round).spawn[i]];
	(*round).spawn[i] = -1;
	(*map).place[0][1] = spawned.pct;
}

void BossSpawn(Round round, BossEnemy boss[], int i, Map* map) {
	BossEnemy spawned;
	spawned = boss[i];
	//Появление босса на карте
	(*map).place[0][1] = spawned.pct;
}

void time(Map* map) {
	Sleep(1000);
	for (int i =38; i > 0; i--) (*map).place[i][1] = (*map).place[i - 1][1];
	(*map).place[0][1] = ' ';
}

void Changes(Map* map) {
	system("cls");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 40; j++) printf("%c", (*map).place[j][i]);
		printf("\n");
	}
	for (int j = 0; j < 40; j++) printf("%c", (*map).towerPlace[j]);
}

void TowerBuy(TowerDef* tower, Shop* shop, int i, Map* map) {
	(*shop).cash = (*shop).cash - 40;
	(*shop).towers[i] = 1;
	(*tower).dmg = 20;
	(*tower).lvl = 1;
	(*tower).range = 0.5;
	(*tower).speed = 1;
	(*map).towerPlace[5 * i + 2] = '1';
}

void TowerUpgrade(TowerDef *towers[], Shop* shop, int i, int j) {
	(*shop).cash = (*shop).cash - (*towers[i]).lvl * 10;
	(*towers[i]).lvl += 1;
	if ((*towers[i]).lvl == 30) (*shop).towers[i] = -1;
	if (j == 1) (*towers[i]).dmg += 20;
	else if (j == 2) (*towers[i]).range += 0.25;
	else (*towers[i]).speed += 1;
}


int main()
{
	Enemy enemy[] = {{5, 100, 10, 1, "Zombe", -1, 0, 'Z'}, {2, 30, 20, 1, "Skeleton", -1, 0, 'X'}, {8, 75, 25, 2, "AngryZombe", -1, 0, '$'}};
	BossEnemy boss[] = { {} };
	TowerDef towers[] = { {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
	Round round;
	Shop shop;
	Tower mainTower;
	Map map;
	int i;
	for (i = 0; i < 40; i++) map.place[i][0] = '-';
	for (i = 0; i < 40; i++) map.place[i][2] = '-';
	map.place[39][1] = '|';
	for (i = 0; i < 39; i++) map.place[i][1] = ' ';
	for (i = 0; i < 40; i++) {
		if ((i + 3) % 5 == 0) map.towerPlace[i] = '0';
		else map.towerPlace[i] = ' ';
	}
	Restart(&round, &shop, &mainTower);
	Changes(&map);
	short int k = 0;
	if (round.boss == true) {
		round.money = round.money - boss[(round.num / 10)].cost;
		BossSpawn(round, boss, round.num/10, &map);
		Changes(&map);
		time(&map);
		Changes(&map);
		time(&map);
		Changes(&map);
		time(&map);
		Changes(&map);
	}
	i = 0;
	while (round.money > 1 && k < 256) {
		//Рандомайзер случайного числа
		EnemyBuy(enemy[i], &round, k, i);
		i++;
		k++;
	}
	for (int i = 0; i < 256; i++) {
		if (round.spawn[i] == -1) break;
		EnemySpawn(&round, enemy, i, &map);
		Changes(&map);
		time(&map);
		Changes(&map);
		time(&map);
		Changes(&map);
	}
	TowerBuy(&towers[3], &shop, 3, &map);
	Changes(&map);
	do{ 
		time(&map); 
		Changes(&map);
	} while (map.place[38][1] == ' ');
}

