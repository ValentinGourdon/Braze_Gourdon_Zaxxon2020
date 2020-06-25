#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include <iostream>

const float Game::PlayerSpeed = 250.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const int windowHeight = 600;
const int windowWidth = 840;

Game::Game()
	: mWindow(sf::VideoMode(windowWidth, windowHeight), "Zaxxon 2020", sf::Style::Close)
	, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
{
	mWindow.setFramerateLimit(160);

	// Textures 

	_TextureBackground.loadFromFile("Media/Textures/background.png");
	_TextureWeapon.loadFromFile("Media/Textures/player-weapon.png");
	_TextureWeaponEnemy.loadFromFile("Media/Textures/SI_WeaponYellow_horizontal.png");
	_TextureWeaponEnemyMaster.loadFromFile("Media/Textures/enemy_master_weapon.png");
	mTexture.loadFromFile("Media/Textures/player.png");
	_TextureEnemyMaster.loadFromFile("Media/Textures/enemy_master.png");
	_TextureEnemy.loadFromFile("Media/Textures/enemy.png");
	_TextureCanon.loadFromFile("Media/Textures/canon_2.png");
	_TextureCanonWeapon.loadFromFile("Media/Textures/canon_bullet2.png");
	_TextureEnemyBoss.loadFromFile("Media/Textures/enemy_boss.png");
	_TextureWeaponEnemyBoss.loadFromFile("Media/Textures/enemy_boss_weapon.png");


	// Sounds

	_BufferWeaponShoot.loadFromFile("Media/Sounds/player-shoot.wav");
	_SoundWeaponShoot.setBuffer(_BufferWeaponShoot);
	_BufferEnemyWeaponShoot.loadFromFile("Media/Sounds/enemy-shoot.wav");
	_SoundEnemyWeaponShoot.setBuffer(_BufferEnemyWeaponShoot);
	_BufferEnemyMasterWeaponShoot.loadFromFile("Media/Sounds/enemy-master-shoot.wav");
	_SoundEnemyMasterWeaponShoot.setBuffer(_BufferEnemyMasterWeaponShoot);
	_BufferEnemyCanonWeaponShoot.loadFromFile("Media/Sounds/enemy-canon-shoot.wav");
	_SoundEnemyCanonWeaponShoot.setBuffer(_BufferEnemyCanonWeaponShoot);
	_BufferEnemyBossWeaponShoot.loadFromFile("Media/Sounds/enemy-boss-shoot.wav");
	_SoundEnemyBossWeaponShoot.setBuffer(_BufferEnemyBossWeaponShoot);
	_BufferGameOver.loadFromFile("Media/Sounds/you-loose.wav");
	_SoundGameOver.setBuffer(_BufferGameOver);
	_BufferCongratulation.loadFromFile("Media/Sounds/congratulation.wav");
	_SoundCongratulation.setBuffer(_BufferCongratulation);
	_BufferNextTime.loadFromFile("Media/Sounds/next-time.wav");
	_SoundNextTime.setBuffer(_BufferNextTime);
	_SoundBackgroundMusic.openFromFile("Media/Sounds/Mute_City_F-Zero_X.wav");
	_SoundBackgroundMusic.play();

	// Font

	mFont.loadFromFile("Media/Sansation.ttf");


	InitSprites();
}

void Game::ResetSprites()
{
	_IsGameOver = false;
	_IsGameWon = false;
	_IsEnemyWeaponFired = false;
	_countPlayerWeaponFired = 0;
	_IsEnemyMasterWeaponFired = false;

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		entity->m_enabled = true;
	}
}

void Game::InitSprites()
{
	_playerLives = 3;
	_score = 0;
	_IsGameOver = false;
	_IsGameWon = false;
	_IsEnemyWeaponFired = false;
	_countPlayerWeaponFired = 0;
	_IsEnemyMasterWeaponFired = false;

	//
	// Backgournd
	//
	_Background.setTexture(_TextureBackground);
	_Background.setPosition(0.f, 0.f);	
	std::shared_ptr<Entity> seba = std::make_shared<Entity>();
	seba->m_sprite = _Background;
	seba->m_type = EntityType::background;
	seba->m_size = _TextureBackground.getSize();
	seba->m_position = _Background.getPosition();
	seba->m_enabled = true;
	EntityManager::m_Entities.push_back(seba);

	//
	// Player
	//

	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(70.f, 300.f);
	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

	//
	// Enemy Master
	//

	_EnemyMaster.setTexture(_TextureEnemyMaster);
	_EnemyMaster.setPosition(700.f, 100.f);
	std::shared_ptr<Entity> sem = std::make_shared<Entity>();
	sem->m_sprite = _EnemyMaster;
	sem->m_type = EntityType::enemyMaster;
	sem->m_size = _TextureEnemyMaster.getSize();
	sem->m_position = _EnemyMaster.getPosition();
	sem->m_enabled = false;
	EntityManager::m_Entities.push_back(sem);

	//
	// Enemies Squad 1
	//

	for (int i = 0; i < ENEMIES_COUNT; i++)
	{
		int r = rand() % 100;

		_EnemySquad1[i].setTexture(_TextureEnemy);
		_EnemySquad1[i].setPosition(700.f + 200.f * (i + 1), 100);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _EnemySquad1[i];
		se->m_type = EntityType::enemy;
		se->m_size = _TextureEnemy.getSize();
		se->m_position = _EnemySquad1[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}

	//
	// Enemies Squad 2
	//

	for (int i = 0; i < ENEMIES_COUNT; i++)
	{
		int r = rand() % 100;

		_EnemySquad2[i].setTexture(_TextureEnemy);
		_EnemySquad2[i].setPosition(800.f + 200.f * (i + 1), 500);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _EnemySquad2[i];
		se->m_type = EntityType::enemy;
		se->m_size = _TextureEnemy.getSize();
		se->m_position = _EnemySquad2[i].getPosition();
		se->m_squad = 2;
		EntityManager::m_Entities.push_back(se);
	}

	//
	// Canon
	//

	_EnemyCanon.setTexture(_TextureCanon);
	_EnemyCanon.setPosition(600.f, 660.f);
	std::shared_ptr<Entity> sb = std::make_shared<Entity>();
	sb->m_sprite = _EnemyCanon;
	sb->m_type = EntityType::enemyCanon;
	sb->m_size = _TextureCanon.getSize();
	sb->m_position = _EnemyCanon.getPosition();
	EntityManager::m_Entities.push_back(sb);
	

	//
	// Boss
	//

	_EnemyBoss.setTexture(_TextureEnemyBoss);
	_EnemyBoss.setPosition(850.f, 225.f);
	std::shared_ptr<Entity> seb = std::make_shared<Entity>();
	seb->m_sprite = _EnemyBoss;
	seb->m_type = EntityType::enemyBoss;
	seb->m_size = _TextureEnemyBoss.getSize();
	seb->m_position = _EnemyBoss.getPosition();
	seb->m_enabled = false;
	EntityManager::m_Entities.push_back(seb);


	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	//
	// Lives
	//

	_LivesText.setFillColor(sf::Color::Green);
	_LivesText.setFont(mFont);
	_LivesText.setPosition(10.f, 50.f);
	_LivesText.setCharacterSize(20);
	_LivesText.setString(std::to_string(_playerLives));

	//
	// Text
	//

	_ScoreText.setFillColor(sf::Color::Green);
	_ScoreText.setFont(mFont);
	_ScoreText.setPosition(10.f, 100.f);
	_ScoreText.setCharacterSize(20);
	_ScoreText.setString(std::to_string(_score));
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			HandlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			HandlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.f, 0.f);
	sf::Vector2f playerCurrentPos = EntityManager::GetPlayer()->m_sprite.getPosition();
	sf::Vector2u playerSize = EntityManager::GetPlayer()->m_size;
	if (mIsMovingUp) {
		if (playerCurrentPos.y - 1.f < 0) {
			movement.y = movement.y;
		}
		else {
			movement.y -= PlayerSpeed;
		}
	}
	if (mIsMovingDown) {
		if (playerCurrentPos.y + playerSize.y + 1.f > windowHeight) {
			movement.y = movement.y;
		}
		else {
			movement.y += PlayerSpeed;
		}
	}
	if (mIsMovingLeft) {
		if (playerCurrentPos.x - 1.f < 0) {
			movement.x = movement.x;
		}
		else {
			movement.x -= PlayerSpeed;
		}
	}
	if (mIsMovingRight) {
		if (playerCurrentPos.x + playerSize.x + 1.f > windowWidth) {
			movement.x = movement.x;
		}
		else {
			movement.x += PlayerSpeed;
		}
	}
		
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::player)
		{
			continue;
		}

		entity->m_sprite.move(movement * elapsedTime.asSeconds());
	}
}

void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	mWindow.draw(mStatisticsText);
	mWindow.draw(mText);
	mWindow.draw(_LivesText);
	mWindow.draw(_ScoreText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}

	//
	// Handle collisions
	//

	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		if (_IsGameOver == true || _IsGameWon == true)
			return;

		HandleTexts();
		HandleGameOver();
		HandleCollisionEnemyWeaponPlayer();
		HandleCollisionEnemyMasterWeaponPlayer();
		HandleCollisionEnemyCanonWeaponPlayer();
		HandleCollisionEnemyPlayer();
		HandleCollisionEnemyMasterPlayer();
		HandleCollisionEnemyCanonPlayer();
		HandleCollisionEnemyBossPlayer();
		HandleCollisionEnemyBossWeaponPlayer();
		HandleEnemyPastPlayer();
		HandleEnemyCanonArrival();
		HandleEnemyBossArrival();
		HanldeEnemyWeaponMoves();
		HandleEnemyMasterWeaponMoves();
		HandleEnemyCanonWeaponMove();
		HandleEnemyBossWeaponMove();
		HandleEnemyMoves();
		HandleEnemyMasterMove();
		HandleEnemyBossMoves();
		HandleEnemyWeaponFiring();
		HandleEnemyMasterWeaponFiring();
		HandleEnemyCanonWeaponFiring();
		HandleEnemyBossWeaponFiring();
		HanldeWeaponMoves();
		HandleCollisionWeaponEnemy();
		HandleCollisionWeaponEnemyMaster();
		HandleCollisionWeaponEnemyCanon();
		HandleCollisionWeaponEnemyBoss();
		HandleCollisionWeaponEnemyCanonWeapon();
	}
}

void Game::HandleTexts()
{
	std::string lives = "Lives: " + std::to_string(_playerLives);
	_LivesText.setString(lives);
	std::string score = "Score: " + std::to_string(_score);
	_ScoreText.setString(score);
	return;
}

void Game::HandleCollisionEnemyMasterWeaponPlayer()
{
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::enemyMasterWeapon)
		{
			continue;
		}

		sf::FloatRect boundWeapon;
		boundWeapon = weapon->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundWeapon.intersects(boundPlayer) == true)
		{
			weapon->m_enabled = false;
			_IsEnemyMasterWeaponFired = false; 
			_playerLives--;
			goto end;
		}
	}

end:
	//nop
	return;
}


void Game::HandleCollisionEnemyBossWeaponPlayer() {
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::enemyBossWeapon)
		{
			continue;
		}

		sf::FloatRect boundWeapon;
		boundWeapon = weapon->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundWeapon.intersects(boundPlayer) == true)
		{
			weapon->m_enabled = false;
			_playerLives--;
			goto end;
		}
	}

end:
	return;
}

void Game::HandleCollisionEnemyCanonWeaponPlayer() {
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::enemyCanonWeapon)
		{
			continue;
		}

		sf::FloatRect boundWeapon;
		boundWeapon = weapon->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundWeapon.intersects(boundPlayer) == true)
		{
			weapon->m_enabled = false;
			_IsEnemyCanonWeaponFired = false;
			_playerLives--;
			goto end;
		}
	}

end:
	//nop
	return;
}

void Game::HandleEnemyMasterWeaponMoves()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemyMasterWeapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		x -= 4;

		if (x <= 0)
		{
			entity->m_enabled = false;
			_IsEnemyMasterWeaponFired = false;
		}

		entity->m_sprite.setPosition(x, y);
	}
}

void Game::HandleCollisionEnemyWeaponPlayer()
{
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::enemyWeapon)
		{
			continue;
		}

		sf::FloatRect boundWeapon;
		boundWeapon = weapon->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundWeapon.intersects(boundPlayer) == true)
		{
			weapon->m_enabled = false;
			_IsEnemyWeaponFired = false;
			_playerLives--;
			goto end;
		}
	}

end:
	//nop
	return;
}



void Game::HanldeEnemyWeaponMoves()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemyWeapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		x -= 2.0f;

		if (x <= 0)
		{
			entity->m_enabled = false;
			_IsEnemyWeaponFired = false;
		}
		else
		{
			entity->m_sprite.setPosition(x, y);
		}
	}
}

void Game::HandleEnemyMasterWeaponFiring()
{
	if (_IsEnemyMasterWeaponFired == true)
		return;

	if (EntityManager::GetEnemyMaster()->m_enabled == false)
		return;

	// a little random...
	int r = rand() % 50;
	if (r != 10)
		return;

	float x, y;
	x = EntityManager::GetEnemyMaster()->m_sprite.getPosition().x;
	y = EntityManager::GetEnemyMaster()->m_sprite.getPosition().y;
	y--;

	std::shared_ptr<Entity> sw = std::make_shared<Entity>();
	sw->m_sprite.setTexture(_TextureWeaponEnemyMaster);
	sw->m_sprite.setPosition(
		x + (_TextureEnemyMaster.getSize().x / 2) - 50,
		y + _TextureEnemyMaster.getSize().y - 30);
	sw->m_type = EntityType::enemyMasterWeapon;
	sw->m_size = _TextureWeaponEnemyMaster.getSize();
	EntityManager::m_Entities.push_back(sw);

	_IsEnemyMasterWeaponFired = true;
	_SoundEnemyMasterWeaponShoot.play();
}


void Game::HandleEnemyBossWeaponFiring() {
	if (!_IsBossArrived)
		return;

	/*if (_IsEnemyBossWeaponFired == true)
		return;*/

	if (EntityManager::GetEnemyBoss()->m_enabled == false)
		return;

	// a little random...
	int r = rand() % 100;
	if (r != 1)
		return;

	float x, y;
	x = EntityManager::GetEnemyBoss()->m_sprite.getPosition().x;
	y = EntityManager::GetEnemyBoss()->m_sprite.getPosition().y;
	y--;

	std::shared_ptr<Entity> sw = std::make_shared<Entity>();
	sw->m_sprite.setTexture(_TextureWeaponEnemyBoss);
	sw->m_sprite.setPosition(
		x + (_TextureEnemyBoss.getSize().x / 2) - 50,
		y + _TextureEnemyBoss.getSize().y - 30);
	sw->m_type = EntityType::enemyBossWeapon;
	sw->m_size = _TextureWeaponEnemyBoss.getSize();
	EntityManager::m_Entities.push_back(sw);

	_SoundEnemyBossWeaponShoot.play();
}


void Game::HandleEnemyBossWeaponMove() {
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemyBossWeapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;

		x -= 4;


		if (x <= 0 || y <= 0 || y >= windowWidth)
		{
			entity->m_enabled = false;
		}

		entity->m_sprite.setPosition(x, y);
	}
}

void Game::HandleEnemyWeaponFiring()
{
	std::vector<std::shared_ptr<Entity>>::reverse_iterator rit = EntityManager::m_Entities.rbegin();
	for (; rit != EntityManager::m_Entities.rend(); rit++)
	{
		std::shared_ptr<Entity> entity = *rit;

		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemy)
		{
			continue;
		}

		if (entity->m_sprite.getPosition().x >= windowWidth) {
			continue;
		}

		// a big random...
		int r = rand() % 300;
		if (r != 10)
			continue;

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		y--;

		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureWeaponEnemy);
		sw->m_sprite.setPosition(
			x + _TextureWeaponEnemy.getSize().x / 2,
			y + _TextureWeaponEnemy.getSize().y);

		sw->m_sprite.setPosition(
			entity->m_sprite.getPosition().x - 18,
			entity->m_sprite.getPosition().y + 23);

		sw->m_type = EntityType::enemyWeapon;
		sw->m_size = _TextureWeaponEnemy.getSize();
		EntityManager::m_Entities.push_back(sw);

		_IsEnemyWeaponFired = true;
		_SoundEnemyWeaponShoot.play();
		break;
	}
}

void Game::HandleEnemyCanonWeaponFiring() {
	if (_IsEnemyCanonWeaponFired)
		return;
	if (!_IsCanonOnScreen)
		return;
	std::vector<std::shared_ptr<Entity>>::reverse_iterator rit = EntityManager::m_Entities.rbegin();
	for (; rit != EntityManager::m_Entities.rend(); rit++)
	{
		std::shared_ptr<Entity> entity = *rit;

		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemyCanon)
		{
			continue;
		}

		// a little random...
		int r = rand() % 50;
		if (r != 10)
			continue;

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;

		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureCanonWeapon);
		sw->m_sprite.setPosition(
			x + _TextureCanonWeapon.getSize().x / 2,
			y + _TextureCanonWeapon.getSize().y);

		sw->m_sprite.setPosition(
			entity->m_sprite.getPosition().x,
			entity->m_sprite.getPosition().y);

		sw->m_type = EntityType::enemyCanonWeapon;
		sw->m_size = _TextureCanonWeapon.getSize();
		EntityManager::m_Entities.push_back(sw);

		_IsEnemyCanonWeaponFired = true;
		_SoundEnemyCanonWeaponShoot.play();
		break;
	}
}

void Game::HandleEnemyCanonWeaponMove() {
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemyCanonWeapon)
		{
			continue;
		}

		float x, y, xPlayer, yPlayer;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		xPlayer = EntityManager::GetPlayer()->m_sprite.getPosition().x;
		yPlayer = EntityManager::GetPlayer()->m_sprite.getPosition().y;

		x -= 3.0f;
		if(x < xPlayer)
			y -= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.5f));
		else {
			y = SelfGuidedShoot(y, yPlayer + 5);
		}

		if (x <= 0 || y <= 0)
		{
			entity->m_enabled = false;
			_IsEnemyCanonWeaponFired = false;
		}
		else
		{
			entity->m_sprite.setPosition(x, y);
		}
	}
}

float Game::SelfGuidedShoot(float yShoot, float yPlayer) {
	float ret = yShoot;
	if (yShoot > yPlayer) {
		ret = yShoot - yPlayer;
		if (ret > 3.f)
			ret = yShoot - 3.f;
		else
			ret = yShoot;
	}
	else if (yShoot < yPlayer) {
		ret = yShoot + yPlayer;
		if (ret > 3.f)
			ret = yShoot + 3.f;
		else
			ret = yShoot;
	}

	return ret;
}

void Game::HandleEnemyMasterMove()
{
	if (!_IsMasterSummonned) {
		int r = rand() % 1500;
		if (r == 1) {
			_IsMasterSummonned = true;
			EntityManager::GetEnemyMaster()->m_enabled = true;
		}
	}
	else {
		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
		{
			if (entity->m_enabled == false)
			{
				continue;
			}

			if (entity->m_type != EntityType::enemyMaster)
			{
				continue;
			}

			float x, y;
			x = entity->m_sprite.getPosition().x;
			y = entity->m_sprite.getPosition().y;

			if (entity->m_bTopToBottom == true) {
				y += 2;
			}
			else {
				y -= 2;
			}
			entity->m_times += 2;

			if (entity->m_times >= 400.0)
			{
				if (entity->m_bTopToBottom == true)
				{
					entity->m_bTopToBottom = false;
					entity->m_times = 0;
				}
				else
				{
					entity->m_bTopToBottom = true;
					entity->m_times = 0;
				}
			}

			entity->m_sprite.setPosition(x, y);
		}
	}
}

void Game::HandleEnemyBossArrival()
{
	if (!_IsBossArrived) {

		// enemies counts (2 squads) + enemy canon count + enemy master
		if (_nbEnemyKilled == ((ENEMIES_COUNT * 2) + CANON_COUNT + 1)) {
			// Disable player shoot during boss arrival
			_countPlayerWeaponFired = 3;

			EntityManager::GetEnemyBoss()->m_enabled = true;
			for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
			{
				if (entity->m_enabled == false)
				{
					continue;
				}

				if (entity->m_type != EntityType::enemyBoss)
				{
					continue;
				}

				float x, y;
				x = entity->m_sprite.getPosition().x;
				y = entity->m_sprite.getPosition().y;

				if (x <= 670) {
					_IsBossArrived = true;
					_countPlayerWeaponFired = 0;
				}
				else {
					x--;
				}

				entity->m_sprite.setPosition(x, y);
			}
		}
	}
}

void Game::HandleEnemyBossMoves()
{
	//
	// Handle Enemy Boss moves
	//
	if (_IsBossArrived) {
		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
		{
			if (entity->m_enabled == false)
			{
				continue;
			}

			if (entity->m_type != EntityType::enemyBoss)
			{
				continue;
			}

			float x, y;
			x = entity->m_sprite.getPosition().x;
			y = entity->m_sprite.getPosition().y;

			if (entity->m_TopToLeft == true) {
				y -= 1.5;
				x -= 1.5;
			}
			else if (entity->m_LeftToBottom == true)
			{
				y -= 1.5;
				x += 1.5;
			}
			else if (entity->m_BottomToRight == true) {
				y += 1.5;
				x += 1.5;
			}
			else {
				y += 1.5;
				x -= 1.5;
			}
			entity->m_times++;

			if (entity->m_times >= 120) //0)
			{
				if (entity->m_TopToLeft == true)
				{
					entity->m_LeftToBottom = true;
					entity->m_TopToLeft = false;
					entity->m_times = 0;
				}
				else if(entity->m_LeftToBottom == true)
				{
					entity->m_BottomToRight = true;
					entity->m_LeftToBottom = false;
					entity->m_times = 0;
				}
				else if (entity->m_BottomToRight == true) {
					entity->m_RightToTop = true;
					entity->m_BottomToRight = false;
					entity->m_times = 0;
				}
				else {
					entity->m_TopToLeft = true;
					entity->m_LeftToBottom = false;
					entity->m_times = 0;
				}
			}

			entity->m_sprite.setPosition(x, y);
		}
	}
}

void Game::HandleEnemyMoves()
{
	//
	// Handle Enemy moves
	//


	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemy)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		
		if (entity->m_squad == 1) {
			if (entity->m_bTopToBottom == true)
				y++;
			else
				y--;
		}
		else {
			if (entity->m_bTopToBottom == true)
				y--;
			else
				y++;
		}
		x -= 0.3f;
		entity->m_times++;

		if (entity->m_times >= 400) //0)
		{
			if (entity->m_squad == 1) {
				if (entity->m_bTopToBottom == true)
				{
					entity->m_bTopToBottom = false;
					entity->m_times = 0;
				}
				else
				{
					entity->m_bTopToBottom = true;
					entity->m_times = 0;
					y += 1;
				}
			}
			else {
				if (entity->m_bTopToBottom == false)
				{
					entity->m_bTopToBottom = true;
					entity->m_times = 0;
				}
				else
				{
					entity->m_bTopToBottom = false;
					entity->m_times = 0;
					y -= 1;
				}
			}
		}

		entity->m_sprite.setPosition(x, y);
	}
}

void Game::HandleEnemyCanonArrival()
{
	if (!_IsCanonOnScreen && _canonKilled < CANON_COUNT) {
		int r = rand() % 10;
		if (r == 1) {
			EntityManager::GetEnemyCanon()->m_enabled = true;
			for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
			{
				if (entity->m_enabled == false)
				{
					continue;
				}

				if (entity->m_type != EntityType::enemyCanon)
				{
					continue;
				}

				float x, y;
				x = entity->m_sprite.getPosition().x;
				y = entity->m_sprite.getPosition().y;

				if (y <= 555) {
					_IsCanonOnScreen = true;
				}
				else {
					y -= 1.f;
				}

				entity->m_sprite.setPosition(x, y);
			}
		}
	}
}

void Game::HandleEnemyPastPlayer() {
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemy)
		{
			continue;
		}

		// Enemy width sprite is 63px
		if (entity->m_sprite.getPosition().x <= -63) {
			entity->m_enabled = false;
			_playerLives--;
		}
	}
}

void Game::HanldeWeaponMoves()
{
	//
	// Handle Weapon moves
	//

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::weapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		x += 3.5;

		if (x >= windowWidth)
		{
			entity->m_enabled = false;
			_countPlayerWeaponFired--;
		}

		entity->m_sprite.setPosition(x, y);
	}
}

void Game::HandleCollisionWeaponEnemy()
{
	// Handle collision weapon enemies

	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
		{
			if (enemy->m_type != EntityType::enemy)
			{
				continue;
			}

			if (enemy->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundEnemy;
			boundEnemy = enemy->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundEnemy) == true)
			{
				enemy->m_enabled = false;
				weapon->m_enabled = false;
				_countPlayerWeaponFired--;
				_score += 10;
				_nbEnemyKilled++;
				//break;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionWeaponEnemyMaster()
{
	// Handle collision weapon master enemy

	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
		{
			if (enemy->m_type != EntityType::enemyMaster)
			{
				continue;
			}

			if (enemy->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundEnemy;
			boundEnemy = enemy->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundEnemy) == true)
			{
				if (enemy->m_livesMaster > 1) {
					enemy->m_livesMaster--;
					weapon->m_enabled = false;
					_countPlayerWeaponFired--;
					goto end;
				}
				else {
					enemy->m_enabled = false;
					weapon->m_enabled = false;
					_countPlayerWeaponFired--;
					_score += 100;
					_nbEnemyKilled++;
					//break;
					goto end;
				}
			}

		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionWeaponEnemyBoss() {

	// Handle collision weapon enemy boss

	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
		{
			if (entity->m_type != EntityType::enemyBoss)
			{
				continue;
			}

			if (entity->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundEnemy;
			boundEnemy = entity->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundEnemy) == true)
			{
				if (entity->m_livesBoss > 1) {
					entity->m_livesBoss--;
					weapon->m_enabled = false;
					_countPlayerWeaponFired--;
					goto end;
				}
				else {
					entity->m_enabled = false;
					weapon->m_enabled = false;
					_countPlayerWeaponFired--;
					_nbEnemyKilled++;
					_score += 200;
					//break;
					goto end;
				}
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionWeaponEnemyCanon() {
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
		{
			if (enemy->m_type != EntityType::enemyCanon)
			{
				continue;
			}

			if (enemy->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundEnemy;
			boundEnemy = enemy->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundEnemy) == true)
			{
				enemy->m_enabled = false;
				enemy->m_sprite.setPosition(enemy->m_sprite.getPosition().x, enemy->m_sprite.getPosition().y + 105);
				weapon->m_enabled = false;
				_countPlayerWeaponFired--;
				_score += 20;
				_nbEnemyKilled++;
				_canonKilled++;
				_IsCanonOnScreen = false;
				//break;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionWeaponEnemyCanonWeapon() {
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
		{
			if (entity->m_type != EntityType::enemyCanonWeapon)
			{
				continue;
			}

			if (entity->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundEnemyWeapon;
			boundEnemyWeapon = entity->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundEnemyWeapon) == true)
			{
				entity->m_enabled = false;
				weapon->m_enabled = false;
				_IsEnemyCanonWeaponFired = false;
				_countPlayerWeaponFired--;
				//break;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionEnemyPlayer() {
	for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
	{
		if (enemy->m_enabled == false)
		{
			continue;
		}

		if (enemy->m_type != EntityType::enemy)
		{
			continue;
		}

		sf::FloatRect boundEnemy;
		boundEnemy = enemy->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundEnemy.intersects(boundPlayer) == true)
		{
			enemy->m_enabled = false;
			_playerLives--;
			_nbEnemyKilled++;
			_score += 10;
			goto end;
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionEnemyMasterPlayer() {
	for (std::shared_ptr<Entity> enemyMaster : EntityManager::m_Entities)
	{
		if (enemyMaster->m_enabled == false)
		{
			continue;
		}

		if (enemyMaster->m_type != EntityType::enemyMaster)
		{
			continue;
		}

		sf::FloatRect boundEnemyMaster;
		boundEnemyMaster = enemyMaster->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundEnemyMaster.intersects(boundPlayer) == true)
		{
			if (enemyMaster->m_livesMaster > 1) {
				enemyMaster->m_livesMaster--;
				enemyMaster->m_enabled = false;
				_countPlayerWeaponFired--;
				goto end;
			}
			else {
				enemyMaster->m_enabled = false;
				enemyMaster->m_enabled = false;
				_countPlayerWeaponFired--;
				_score += 100;
				_nbEnemyKilled++;
				//break;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionEnemyCanonPlayer() {
	for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
	{
		if (enemy->m_enabled == false)
		{
			continue;
		}

		if (enemy->m_type != EntityType::enemyCanon)
		{
			continue;
		}

		sf::FloatRect boundEnemyMaster;
		boundEnemyMaster = enemy->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundEnemyMaster.intersects(boundPlayer) == true)
		{
			enemy->m_enabled = false;
			_playerLives--;
			_nbEnemyKilled++;
			_canonKilled++;
			_score += 20;
			_IsCanonOnScreen = false;
			goto end;
		}
	}

end:
	//nop
	return;
}


void Game::HandleCollisionEnemyBossPlayer() {
	for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
	{
		if (enemy->m_enabled == false)
		{
			continue;
		}

		if (enemy->m_type != EntityType::enemyBoss)
		{
			continue;
		}

		sf::FloatRect boundEnemBoss;
		boundEnemBoss = enemy->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundEnemBoss.intersects(boundPlayer) == true)
		{
			enemy->m_enabled = false;
			_playerLives -= 3;
			enemy->m_livesBoss--;
			_nbEnemyKilled++;
			goto end;
		}
	}

end:
	return;
}

void Game::HandleGameOver()
{
	// Game Over ?
	int count = std::count_if(EntityManager::m_Entities.begin(), EntityManager::m_Entities.end(), [](std::shared_ptr<Entity> element) {
		if (element->m_type == EntityType::enemy || element->m_type == EntityType::enemyMaster || element->m_type == EntityType::enemyCanon)
		{
			if (element->m_enabled == false)
			{
				return true;
			}
		}
		return false;
	});

	// enemies counts (2 squads) + enemy canon count + enemy master + enemy boss
	if (_nbEnemyKilled == ((ENEMIES_COUNT * 2) + CANON_COUNT + 2))
	{
		DisplayWin();
	}

	if (EntityManager::GetPlayer()->m_enabled == false)
	{
		DisplayGameOver();
	}

	if (_playerLives == 0)
	{
		DisplayGameOver();
	}
}

void Game::DisplayGameOver()
{
	if (_playerLives == 0)
	{
		mText.setFillColor(sf::Color::Red);
		mText.setFont(mFont);
		mText.setPosition(200.f, 200.f);
		mText.setCharacterSize(80);

		mText.setString("GAME OVER");

		_IsGameOver = true;
		_SoundBackgroundMusic.setVolume(20);
		_SoundGameOver.play();
	}
	else
	{
		ResetSprites();
	}
}

void Game::DisplayWin()
{
	if (_playerLives != 0)
	{
		mText.setFillColor(sf::Color::Green);
		mText.setFont(mFont);
		mText.setPosition(200.f, 200.f);
		mText.setCharacterSize(80);

		mText.setString("YOU WON !!!");

		_IsGameWon = true;
		if (_playerLives == 3) {
			_SoundBackgroundMusic.setVolume(20);
			_SoundNextTime.play();
		}
		else {
			_SoundBackgroundMusic.setVolume(20);
			_SoundCongratulation.play();
		}

	}
	else
	{
		ResetSprites();
	}
}

void Game::HandlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if(_IsGameOver || _IsGameWon) {
		mIsMovingUp = false;
		mIsMovingDown = false;
		mIsMovingLeft = false;
		mIsMovingRight = false;
		return;
	}
	if (key == sf::Keyboard::Up) {
		mIsMovingUp = isPressed;
	}
	else if (key == sf::Keyboard::Down) {
		mIsMovingDown = isPressed;
	}
	else if (key == sf::Keyboard::Left) {
		mIsMovingLeft = isPressed;
	}
	else if (key == sf::Keyboard::Right) {
		mIsMovingRight = isPressed;
	}

	if (key == sf::Keyboard::Space)
	{
		if (isPressed == false)
		{
			return;
		}

		if (_countPlayerWeaponFired >= MAX_WEAPON_FIRE)
		{
			return;
		}

		_SoundWeaponShoot.play();
		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureWeapon);
		sw->m_sprite.setPosition(
			EntityManager::GetPlayer()->m_sprite.getPosition().x + 45,
			EntityManager::GetPlayer()->m_sprite.getPosition().y + 15);
		sw->m_type = EntityType::weapon;
		sw->m_size = _TextureWeapon.getSize();
		EntityManager::m_Entities.push_back(sw);

		_countPlayerWeaponFired++;
	}
}
