
#include <sstream>
#include "GameState.hpp"
#include "DEFINITIONS.hpp"
#include "GameOverState.hpp"

#include <iostream>

namespace PapaSmurfie
{

	GameState::GameState(GameDataRef data) : _data(data)
	{

	}

	void GameState::Init()
	{
		if (!_hitSoundBuffer.loadFromFile(HIT_SOUND_FILEPATH))
		{
			std::cout << "Error Loading SFX - hit" << std::endl;
		}
		if (!_wingSoundBuffer.loadFromFile(WING_SOUND_FILEPATH))
		{
			std::cout << "Error Loading SFX - wing" << std::endl;
		}
		if (!_pointSoundBuffer.loadFromFile(POINT_SOUND_FILEPATH))
		{
			std::cout << "Error Loading SFX - point" << std::endl;
		}
		_hitSound.setBuffer(_hitSoundBuffer);
		_wingSound.setBuffer(_wingSoundBuffer);
		_pointSound.setBuffer(_pointSoundBuffer);

		this->_data->assets.LoadTexture("GameBackground", GAME_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("PipeUp", PIPE_UP_FILEPATH);
		this->_data->assets.LoadTexture("PipeDown", PIPE_DOWN_FILEPATH);
		this->_data->assets.LoadTexture("Land", LAND_FILEPATH);
		this->_data->assets.LoadTexture("BirdFrame1", BIRD_FRAME_1_FILEPATH);
		this->_data->assets.LoadTexture("BirdFrame2", BIRD_FRAME_2_FILEPATH);
		this->_data->assets.LoadTexture("BirdFrame3", BIRD_FRAME_3_FILEPATH);
		this->_data->assets.LoadTexture("BirdFrame4", BIRD_FRAME_4_FILEPATH);
		this->_data->assets.LoadTexture("ScoringPipe", SCORING_PIPE_FILEPATH);
		this->_data->assets.LoadFont("FlappyFont", FLAPPY_FONT_FILEPATH);

		pipe = new Pipe(_data);
		land = new Land(_data);
		bird = new Bird(_data);
		flash = new Flash(_data);
		hud = new HUD(_data);

		_background.setTexture(this->_data->assets.GetTexture("GameBackground"));

		_score = 0;
		hud->UpdateScore(_score);

		_gameState = GameStates::eReady;
	}

	void GameState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}
			if (_data->input.IsSpriteClicked(_background, sf::Mouse::Left, _data->window))
			{
				if (GameStates::eGameOver != _gameState)
				{
					_gameState = GameStates::ePlaying;
					bird->Tap();
					_wingSound.play();
				}
			}
		}
	}

	void GameState::Update(float dt)
	{
		if (GameStates::eGameOver != _gameState)
		{
			bird->Animate(dt);
			land->MoveLand(dt);
		}

		if (GameStates::ePlaying == _gameState)
		{
			pipe->MovePipes(dt);

			if (clock.getElapsedTime().asSeconds() > PIPE_SPAWN_FREQUENCY)
			{
				pipe->RandomizePipeOffset();

				pipe->SpawnInvisiblePipe();
				pipe->SpawnBottomPipe();
				pipe->SpawnTopPipe();
				pipe->SpawnScoringPipe(); // maybe spawn inviz pipe ?! 

				clock.restart();
			}

			bird->Update(dt);

			std::vector<sf::Sprite> landSprites = land->GetSprites();

			for (int i = 0; i < landSprites.size(); i++)
			{
				if (collision.CheckSpriteCollision(bird->GetSprite(), 0.7f, landSprites.at(i), 1.0f))
				{
					_gameState = GameStates::eGameOver;

					clock.restart();
					_hitSound.play();
				}
			}

			std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();

			for (int i = 0; i < pipeSprites.size(); i++)
			{
				if (collision.CheckSpriteCollision(bird->GetSprite(), 0.625f, pipeSprites.at(i), 1.0f))
				{
					_gameState = GameStates::eGameOver;
					clock.restart();
					_hitSound.play();
				}
			}

			if (GameStates::ePlaying == _gameState)
			{
				std::vector<sf::Sprite>& scoringPipes = pipe->GetScoringSprites();

				for (int i = 0; i < scoringPipes.size(); i++)
				{
					if (collision.CheckSpriteCollision(bird->GetSprite(), 0.625f, scoringPipes.at(i), 1.0f))
					{
						_score++;
						hud->UpdateScore(_score);
						//std::cout << _score << std::endl;
						scoringPipes.erase(scoringPipes.begin() + i);
						_pointSound.play();
					}
				}
			}
			
		}

		if (GameStates::eGameOver == _gameState)
		{
			flash->Show(dt);

			if (clock.getElapsedTime().asSeconds() > TIME_BEFORE_GAME_OVER_APPEARS)
			{
				_data->machine.AddState(StateRef(new GameOverState(_data, _score)), true);
			}
		}

	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Red);

		this->_data->window.draw(this->_background);
		pipe->DrawPipes();
		land->DrawLand();
		bird->Draw();

		flash->Draw();

		hud->Draw();

		this->_data->window.display();
	}
}