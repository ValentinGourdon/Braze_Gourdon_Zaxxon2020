#pragma once
#include "Weapon.h"

#define ENEMIES_COUNT 5
#define CANON_COUNT 1
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
	void HandleCollisionEnemyPlayer();
	void HandleCollisionEnemyMasterPlayer();
	void HandleCollisionEnemyCanonPlayer();
	void HandleCollisionEnemyMasterWeaponPlayer();
	void HandleCollisionEnemyWeaponPlayer();
	void HandleCollisionEnemyCanonWeaponPlayer();
	void HandleEnemyMasterWeaponFiring();
	void HandleEnemyWeaponFiring();
	void HandleEnemyCanonWeaponFiring();
	void HandleEnemyMasterMove();
	void HandleEnemyMoves();
	void HanldeEnemyMasterWeaponMoves();
	void HanldeEnemyWeaponMoves();
	void HandleEnemyCanonWeaponMove();
	void HanldeWeaponMoves();
	void HandleGameOver();
	void DisplayGameOver();
	void DisplayWin();
	void HandlePlayerInput(sf::Keyboard::Key key, bool isPressed);

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
	int _playerLives = 3;
	sf::Text	_ScoreText;
	int _score = 0;
	int _countPlayerWeaponFired = 0;
	int windowHeight = 600;
	int windowWidth = 840;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;

	bool _IsGameOver = false;
	bool _IsGameWon = false;
	bool _IsEnemyWeaponFired = false;
	bool _IsEnemyMasterWeaponFired = false;
	bool _IsCanonWeaponFired = false;
	bool _isMasterSummonned = false;

	sf::Texture	_TextureEnemy;
	sf::Sprite	_Enemy[ENEMIES_COUNT];
	sf::Texture	_TextureCanon;
	sf::Sprite	_Canon[CANON_COUNT];
	sf::Texture	_TextureWeapon;
	sf::Texture	_TextureWeaponEnemy;
	sf::Texture	_TextureWeaponEnemyMaster;
	sf::Sprite	_Weapon;
	sf::Texture	_TextureEnemyMaster;
	sf::Sprite	_EnemyMaster;
	sf::Texture _TextureCanonWeapon;
	sf::Texture _TextureBackground;
};

