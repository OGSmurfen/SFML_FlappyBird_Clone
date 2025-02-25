
#include <sstream>
#include "MainMenuState.hpp"
#include "DEFINITIONS.hpp"
#include "GameState.hpp"
#include <iostream>

namespace PapaSmurfie
{

	MainMenuState::MainMenuState(GameDataRef data) : _data(data)
	{

	}

	void MainMenuState::Init()
	{
		this->_data->assets.LoadTexture("MainMenuBackground", MAIN_MENU_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("GameTitle", GAME_TITLE_FILEPATH);
		this->_data->assets.LoadTexture("PlayButton", PLAY_BUTTON_FILEPATH);

		_background.setTexture(this->_data->assets.GetTexture("MainMenuBackground"));
		_title.setTexture(this->_data->assets.GetTexture("GameTitle"));
		_playButton.setTexture(this->_data->assets.GetTexture("PlayButton"));

		_title.setPosition((SCREEN_WIDTH / 2) - (_title.getGlobalBounds().width / 2),
							_title.getGlobalBounds().height / 2);
		_playButton.setPosition((SCREEN_WIDTH / 2) - (_playButton.getGlobalBounds().width / 2),
				(SCREEN_HEIGHT / 2) - (_playButton.getGlobalBounds().height / 2));
	}

	void MainMenuState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}
			if (_data->input.IsSpriteClicked(_playButton, sf::Mouse::Left, _data->window))
			{
				//std::cout << "Go To Game Screen" << std::endl;
				_data->machine.AddState(StateRef(new GameState(_data)), true);
			}
		}
	}

	void MainMenuState::Update(float dt)
	{
		
	}

	void MainMenuState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Red);

		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_title);
		this->_data->window.draw(this->_playButton);

		this->_data->window.display();
	}
}