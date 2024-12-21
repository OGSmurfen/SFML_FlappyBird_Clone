#include "AssetManager.hpp"
#include <filesystem>
#include <iostream>

namespace PapaSmurfie
{


	void AssetManager::LoadTexture(std::string name, std::string fileName)
	{
		sf::Texture tex;
		if (tex.loadFromFile(fileName))
		{
			this->_textures[name] = tex;
		}
		else 
		{
			if (std::filesystem::exists(fileName))
			{
				std::cout << "File exists: " << fileName << std::endl;
			}
			else 
			{
				std::cerr << "File not found: " << fileName << std::endl;
			}
		}
	}

	sf::Texture& AssetManager::GetTexture(std::string name)
	{
		//return this->_textures.at(name);
		auto it = this->_textures.find(name);
		if (it != this->_textures.end())
		{
			return it->second;
		}
		throw std::out_of_range("Texture not found" + name);
	}

	void AssetManager::LoadFont(std::string name, std::string fileName)
	{
		sf::Font font;
		if (font.loadFromFile(fileName))
		{
			this->_fonts[name] = font;
		}
	}

	sf::Font& AssetManager::GetFont(std::string name)
	{
		//return this->_fonts.at(name);
		auto it = this->_fonts.find(name);
		if (it != this->_fonts.end()) {
			return it->second;
		}
		throw std::out_of_range("Font not found: " + name);
	}

}