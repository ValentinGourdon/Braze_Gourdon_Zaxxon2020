#pragma once
#include "Weapon.h"

#define ENEMIES_COUNT 5
#define CANON_COUNT 2
#define MAX_WEAPON_FIRE 3

class Game
{
public:
	Game();
	~Game() { };
	void run();

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();

	void InitSprites();
	void ResetSprites();

	void updateStatistics(sf::Time elapsedTime);
	void HandleTexts();
	void HandleCollisionWeaponEnemy();
	void HandleCollisionWeaponEnemyMaster();
	void HandleCollisionWeaponEnemyCanon();
	void HandleCollisionWeaponEnemyBoss();
	void HandleCollisionWeaponEnemyCanonWeapon();
	void HandleCollisionEnemyPlayer();
	void HandleCollisionEnemyMasterPlayer();
	void HandleCollisionEnemyCanonPlayer();
	void HandleCollisionEnemyMasterWeaponPlayer();
	void HandleCollisionEnemyWeaponPlayer();
	void HandleCollisionEnemyCanonWeaponPlayer();
	void HandleCollisionEnemyBossPlayer();
	void HandleCollisionEnemyBossWeaponPlayer();
	void HandleEnemyMasterWeaponFiring();
	void HandleEnemyWeaponFiring();
	void HandleEnemyCanonWeaponFiring();
	void HandleEnemyBossWeaponFiring();
	void HandleEnemyMasterMove();
	void HandleEnemyMoves();
	void HandleEnemyBossArrival();
	void HandleEnemyBossMoves();
	void HandleEnemyCanonArrival();
	void HandleEnemyPastPlayer();
	void HandleEnemyMasterWeaponMoves();
	void HanldeEnemyWeaponMoves();
	void HandleEnemyCanonWeaponMove();
	void HandleEnemyBossWeaponMove();
	void HanldeWeaponMoves();
	void HandleGameOver();
	void DisplayGameOver();
	void DisplayWin();
	void HandlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	float SelfGuidedShoot(float y, float yPlayer);

private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	sf::Texture	mTexture;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;
	sf::Text	mText;
	sf::Text	_LivesText;
	sf::Text	_ScoreText;
	int _playerLives = 3;
	int _score = 0;
	int _countPlayerWeaponFired = 0;
	int _nbEnemyKilled = 0;
	int _canonKilled = 0;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;

	bool _IsGameOver = false;
	bool _IsGameWon = false;
	bool _IsEnemyWeaponFired = false;
	bool _IsEnemyMasterWeaponFired = false;
	bool _IsEnemyCanonWeaponFired = false;
	bool _IsEnemyBossWeaponFired = false;
	bool _IsMasterSummonned = false;
	bool _IsBossArrived = false;
	bool _IsCanonOnScreen = false;
	//bool _WeaponBoss = true;

	sf::Texture	_TextureEnemy;
	sf::Sprite	_EnemySquad1[ENEMIES_COUNT];
	sf::Sprite	_EnemySquad2[ENEMIES_COUNT];
	sf::Texture	_TextureCanon;
	sf::Sprite	_EnemyCanon;
	sf::Texture	_TextureWeapon;
	sf::Texture	_TextureWeaponEnemy;
	sf::Texture	_TextureWeaponEnemyMaster;
	sf::Sprite	_Weapon;
	sf::Texture	_TextureEnemyMaster;
	sf::Sprite	_EnemyMaster;
	sf::Texture _TextureCanonWeapon;
	sf::Sprite	_EnemyBoss;
	sf::Texture	_TextureEnemyBoss;
	sf::Texture	_TextureWeaponEnemyBoss;
	sf::Sprite	_Background;
	sf::Texture	_TextureBackground;
};

