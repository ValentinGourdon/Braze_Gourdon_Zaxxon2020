#pragma once

enum EntityType
{
	background,
	player,
	weapon,
	enemy,
	enemyWeapon,
	enemyMaster,
	enemyMasterWeapon,
	enemyCanon,
	enemyCanonWeapon,
	enemyBoss,
	enemyBossWeapon
};

class Entity
{
public:
	Entity() { };
	~Entity() { };

public:
	sf::Sprite m_sprite;
	sf::Vector2u m_size;
	sf::Vector2f m_position;
	EntityType m_type;
	bool m_enabled = true;

	// Enemy only
	bool m_bTopToBottom = true;
	int m_times = 0;
	int m_squad = 1;
	// Master only
	int m_livesMaster = 3;
	// Boss only
	bool m_TopToLeft = false;
	bool m_BottomToRight = false;
	bool m_LeftToBottom = false;
	bool m_RightToTop = true;
	int m_livesBoss = 5;
};

