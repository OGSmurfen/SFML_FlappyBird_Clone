
#include <sstream>
#include "GameOverState.hpp"
#include "DEFINITIONS.hpp"
#include "GameState.hpp"
#include <iostream>
#include <fstream>

namespace PapaSmurfie
{

	GameOverState::GameOverState(GameDataRef data, int score) : _data(data), _score(score)
	{

	}

	void GameOverState::Init()
	{
		std::ifstream readfile;
		readfile.open("Resources\\highscore.txt");

		if (readfile.is_open())
		{
			while (!readfile.eof())
			{
				readfile >> _highScore;
			}
		}
		readfile.close();

		std::ofstream writefile("Resources\\highscore.txt");

		if (writefile.is_open())
		{
			if (_score > _highScore)
			{
				_highScore = _score;
			}
			writefile << _highScore;
		}
		writefile.close();

		this->_data->assets.LoadTexture("GameOverBackground", GAME_OVER_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("GameOverTitle", GAME_OVER_TITLE_FILEPATH);
		this->_data->assets.LoadTexture("GameOverBody", GAME_OVER_BODY_FILEPATH);

		this->_data->assets.LoadTexture("BronzeMedal", BRONZE_MEDAL_FILEPATH);
		this->_data->assets.LoadTexture("SilverMedal", SILVER_MEDAL_FILEPATH);
		this->_data->assets.LoadTexture("GoldMedal", GOLD_MEDAL_FILEPATH);
		this->_data->assets.LoadTexture("PlatinumMedal", PLATINUM_MEDAL_FILEPATH);

		_background.setTexture(this->_data->assets.GetTexture("GameOverBackground"));
		_gameOverTitle.setTexture(this->_data->assets.GetTexture("GameOverTitle"));
		_gameOverContainer.setTexture(this->_data->assets.GetTexture("GameOverBody"));
		_retryButton.setTexture(this->_data->assets.GetTexture("PlayButton"));

		_gameOverContainer.setPosition(_data->window.getSize().x / 2 - _gameOverContainer.getGlobalBounds().width / 2, 
								_data->window.getSize().y / 2 - _gameOverContainer.getGlobalBounds().height / 2);

		 _gameOverTitle.setPosition(_data->window.getSize().x / 2 - _gameOverTitle.getGlobalBounds().width / 2,
								_gameOverContainer.getPosition().y - _gameOverTitle.getGlobalBounds().height * 1.2f); //error ?

		 _retryButton.setPosition(_data->window.getSize().x / 2 - _retryButton.getGlobalBounds().width / 2,
								_gameOverContainer.getPosition().y + _gameOverContainer.getGlobalBounds().height + _retryButton.getGlobalBounds().height * 0.2f);

		 _scoreText.setFont(_data->assets.GetFont("FlappyFont"));
		 _scoreText.setString(std::to_string(_score));
		 _scoreText.setCharacterSize(56);
		 _scoreText.setFillColor(sf::Color::White);
		 _scoreText.setOrigin(_scoreText.getGlobalBounds().width / 2, _scoreText.getGlobalBounds().height / 2);
		 _scoreText.setPosition(_data->window.getSize().x / 10 * 7.25f, _data->window.getSize().y / 2.15f);

		 _highScoreText.setFont(_data->assets.GetFont("FlappyFont"));
		 _highScoreText.setString(std::to_string(_highScore));
		 _highScoreText.setCharacterSize(56);
		 _highScoreText.setFillColor(sf::Color::White);
		 _highScoreText.setOrigin(_highScoreText.getGlobalBounds().width / 2, _highScoreText.getGlobalBounds().height / 2);
		 _highScoreText.setPosition(_data->window.getSize().x / 10 * 7.25f, _data->window.getSize().y / 1.78f);

		 if (_score >= PLATINUM_MEDAL_SCORE)
		 {
			 _medal.setTexture(_data->assets.GetTexture("PlatinumMedal"));
		 }
		 else if (_score >= GOLD_MEDAL_SCORE)
		 {
			 _medal.setTexture(_data->assets.GetTexture("GoldMedal"));
		 }
		 else if (_score >= SILVER_MEDAL_SCORE)
		 {
			 _medal.setTexture(_data->assets.GetTexture("SilverMedal"));
		 }
		 else
		 {
			 _medal.setTexture(_data->assets.GetTexture("BronzeMedal"));
		 }

		 _medal.setPosition(175, 465);
	}

	void GameOverState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}
			if (_data->input.IsSpriteClicked(_retryButton, sf::Mouse::Left, _data->window))
			{
				_data->machine.AddState(StateRef(new GameState(_data)), true);
			}
		}
	}

	void GameOverState::Update(float dt)
	{

	}

	void GameOverState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Red);
		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_gameOverContainer);
		this->_data->window.draw(this->_gameOverTitle);
		this->_data->window.draw(this->_retryButton);

		this->_data->window.draw(_scoreText);
		this->_data->window.draw(_highScoreText);
		this->_data->window.draw(_medal);

		this->_data->window.display();
	}
}