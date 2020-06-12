#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"

const float Game::PlayerSpeed = 250.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(840, 600), "Zaxxon 2020", sf::Style::Close)
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

	_TextureBackground.loadFromFile("Media/Textures/background.png");
	_TextureWeapon.loadFromFile("Media/Textures/SI_WeaponGreen_horizontal.png");
	_TextureWeaponEnemy.loadFromFile("Media/Textures/SI_WeaponYellow_horizontal.png");
	_TextureWeaponEnemyMaster.loadFromFile("Media/Textures/enemy_master_weapon.png");
	mTexture.loadFromFile("Media/Textures/player.png");
	_TextureEnemyMaster.loadFromFile("Media/Textures/enemy_master.png");
	_TextureEnemy.loadFromFile("Media/Textures/enemy.png");
	_TextureCanon.loadFromFile("Media/Textures/canon_2.png");
	_TextureCanonWeapon.loadFromFile("Media/Textures/canon_bullet.png");
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
	// Player
	//

	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(70.f, 550.f); // 300.f
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
		_EnemySquad1[i].setPosition(600.f + 200.f * (i + 1), 100);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _EnemySquad1[i];
		se->m_type = EntityType::enemy;
		se->m_size = _TextureEnemy.getSize();
		se->m_position = _EnemySquad1[i].getPosition();
		se->m_squad = 1;
		EntityManager::m_Entities.push_back(se);
	}

	//
	// Enemies Squad 2
	//

	for (int i = 0; i < ENEMIES_COUNT; i++)
	{
		int r = rand() % 100;

		_EnemySquad2[i].setTexture(_TextureEnemy);
		_EnemySquad2[i].setPosition(700.f + 200.f * (i + 1), 500);

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

	for (int i = 0; i < CANON_COUNT; i++)
	{
		_Canon[i].setTexture(_TextureCanon);
		_Canon[i].setPosition(600.f + (i * 60), 550.f);

		std::shared_ptr<Entity> sb = std::make_shared<Entity>();
		sb->m_sprite = _Canon[i];
		sb->m_type = EntityType::enemyCanon;
		sb->m_size = _TextureCanon.getSize();
		sb->m_position = _Canon[i].getPosition();
		EntityManager::m_Entities.push_back(sb);
	}

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
	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
		movement.x += PlayerSpeed;

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
		HandleCollisionWeaponEnemy();
		HandleCollisionWeaponEnemyMaster();
		HandleCollisionWeaponEnemyCanon();
		HanldeWeaponMoves();
		HanldeEnemyWeaponMoves();
		HanldeEnemyMasterWeaponMoves();
		HandleEnemyCanonWeaponMove();
		HandleEnemyMoves();
		HandleEnemyPastPlayer();
		HandleEnemyMasterMove();
		HandleEnemyWeaponFiring();
		HandleEnemyMasterWeaponFiring();
		HandleEnemyCanonWeaponFiring();
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
			_IsCanonWeaponFired = false;
			_playerLives--;
			goto end;
		}
	}

end:
	//nop
	return;
}

void Game::HanldeEnemyMasterWeaponMoves()
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
	PlaySound(TEXT("Media/Sounds/laser-shot-master.wav"), NULL, SND_ASYNC);
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

		if (entity->m_sprite.getPosition().x >= 840) {
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
		PlaySound(TEXT("Media/Sounds/laser-shot-enemy.wav"), NULL, SND_ASYNC);
		break;
	}
}

void Game::HandleEnemyCanonWeaponFiring() {
	if (_IsCanonWeaponFired == true)
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

		_IsCanonWeaponFired = true;
		PlaySound(TEXT("Media/Sounds/Canon.wav"), NULL, SND_ASYNC);
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

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		x -= 3.0f;
		y -= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.5f));

		if (x <= 0 || y <= 0)
		{
			entity->m_enabled = false;
			_IsCanonWeaponFired = false;
		}
		else
		{
			entity->m_sprite.setPosition(x, y);
		}
	}
}

void Game::HandleEnemyMasterMove()
{
	if (!_isMasterSummonned) {
		if (rand() % 500 == 10) {
			_isMasterSummonned = true;
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
		x -= 0.3;
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

		if (x >= 840)
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
				enemy->m_enabled = false;
				weapon->m_enabled = false;
				_countPlayerWeaponFired--;
				_score += 100;
				//break;
				goto end;
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
				weapon->m_enabled = false;
				_countPlayerWeaponFired--;
				_score += 20;
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
			enemyMaster->m_enabled = false;
			_playerLives -= 2;
			goto end;
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
			goto end;
		}
	}

end:
	//nop
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

	// enemies counts (2 squads) + enemy canon count + enemy master
	if (count == ((ENEMIES_COUNT * 2) + CANON_COUNT + 1))
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
		PlaySound(TEXT("Media/Sounds/you-lose.wav"), NULL, SND_ASYNC);
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
			PlaySound(TEXT("Media/Sounds/nexttime.wav"), NULL, SND_ASYNC);
		}
		else {
			PlaySound(TEXT("Media/Sounds/congratulation.wav"), NULL, SND_ASYNC);
		}

	}
	else
	{
		ResetSprites();
	}
}

void Game::HandlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
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

		PlaySound(TEXT("Media/Sounds/laser-shot-player.wav"), NULL, SND_FILENAME | SND_ASYNC);

		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureWeapon);
		sw->m_sprite.setPosition(
			EntityManager::GetPlayer()->m_sprite.getPosition().x + 45,
			EntityManager::GetPlayer()->m_sprite.getPosition().y + 20);
		sw->m_type = EntityType::weapon;
		sw->m_size = _TextureWeapon.getSize();
		EntityManager::m_Entities.push_back(sw);

		_countPlayerWeaponFired++;
	}
}
