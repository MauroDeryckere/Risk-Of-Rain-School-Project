#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

#include <iostream>
#include <cassert>

typedef std::map <std::string, std::pair<const Texture*, size_t>>::iterator textureMapIterator;

TextureManager::TextureManager()
{
	InitializeDigitAndSymbolArr();
}

TextureManager::~TextureManager()
{
	//Clear map in case reference count != 0 
	for (textureMapIterator it{ m_pTextures.begin() }; it != m_pTextures.end(); ++it)
	{
		delete it->second.first;
	}

	for (size_t textureArr{0}; textureArr < m_pDigitAndSymbolTextures.size(); ++textureArr)
	{
		for (size_t digit{ 0 }; digit < m_pDigitAndSymbolTextures[textureArr].size(); ++digit)
		{
			delete m_pDigitAndSymbolTextures[textureArr][digit];
		}
	}
}

const Texture* TextureManager::CreateTexture(const std::string& filePath, const std::string& key)
{
	textureMapIterator it{ m_pTextures.find(key) };

	if (it == m_pTextures.end())
	{
		it = m_pTextures.emplace( key, std::make_pair(new Texture{ filePath },1 )).first;

		assert(it->second.first->IsCreationOk());
	}
	else //Increaase ref count
	{
		it->second.second++;
	}

	return it->second.first;
}

const Texture* TextureManager::CreateTexture(const std::string& text, const std::string& filePath, const int fontSize, const Color4f& color, const std::string& key)
{
	textureMapIterator it{ m_pTextures.find(key) };

	if (it == m_pTextures.end())
	{
		it = m_pTextures.emplace(key, std::make_pair(new Texture{ text, filePath, fontSize, color }, 1)).first;

		assert(it->second.first->IsCreationOk());
	}
	else //Increase ref count
	{
		it->second.second++;
	}

	return it->second.first;
}

void TextureManager::InitializeDigitAndSymbolArr()
{
	CreateDigitAndSymbolArr("the-bomb-sound.regular.ttf", 20, Color4f{ 1.f,1.f,1.f,1.f });
	CreateDigitAndSymbolArr("the-bomb-sound.regular.ttf", 20, Color4f{0.f,1.f,0.f,1.f});
}

void TextureManager::CreateDigitAndSymbolArr(const std::string& filePath, const int fontSize, const Color4f& color)
{
	std::vector<const Texture*> textureArr;
	textureArr.reserve(16);

	for (size_t digit {0}; digit <= 9; ++digit)
	{
		textureArr.emplace_back(new Texture{ std::to_string(digit), filePath, fontSize, color });
	}

	textureArr.emplace_back(new Texture{"/", filePath, fontSize, color });
	textureArr.emplace_back(new Texture{"+", filePath, fontSize, color });
	textureArr.emplace_back(new Texture{"-", filePath, fontSize, color });
	textureArr.emplace_back(new Texture{":", filePath, fontSize, color });
	textureArr.emplace_back(new Texture{"%", filePath, fontSize, color });
	textureArr.emplace_back(new Texture{"$", filePath, fontSize, color });

	m_pDigitAndSymbolTextures.emplace_back(textureArr);

	for (size_t index{0}; index < m_pDigitAndSymbolTextures.back().size(); ++index)
	{
		assert(m_pDigitAndSymbolTextures.back()[index]->IsCreationOk());
	}
}

bool TextureManager::DeleteTexture(const std::string& key)
{
	textureMapIterator it{ m_pTextures.find(key) };

	if (it == m_pTextures.end()) 
	{
		std::cerr << key << " Texture " << key << " doesn't exist \n";
		return false;
	}

	if (--it->second.second == 0) 
	{
		delete it->second.first;
		m_pTextures.erase(it);
		return true;
	}

	return false;
}

bool TextureManager::DrawNumber(NumberTextures numTextureArr, const Point2f& bottomLeft, size_t number, const float digitSpace, float& numberWidth) const
{
	if (size_t(numTextureArr) > m_pDigitAndSymbolTextures.size() )
	{
		std::cerr << "NumberTexture doesn't exist (not created) \n";
		return false;
	}

	if (number == 0)
	{
		numberWidth = m_pDigitAndSymbolTextures[int(numTextureArr)][int(Symbols::Zero)]->GetWidth();
		return DrawSymbol(numTextureArr, bottomLeft, Symbols::Zero, digitSpace);
	}

	float previousDigitsWidth{};

	size_t currentNumber{ number };

	std::vector<size_t> digits;
	while (currentNumber != 0)
	{
		const size_t leastSigDigit = currentNumber % 10;
		digits.emplace_back(leastSigDigit);
		currentNumber /= 10;
	}

	while (!digits.empty())
	{
		if (!DrawDigit(size_t(numTextureArr), bottomLeft, digits.back(), digitSpace, previousDigitsWidth))
		{
			return false;
		}
		digits.pop_back();
	}

	numberWidth = previousDigitsWidth;

	return true;
}

bool TextureManager::DrawNumber(NumberTextures numTextureArr, const Point2f& bottomLeft, size_t number, const float digitSpace) const
{
	float numberWidth{};
	return DrawNumber(numTextureArr, bottomLeft, number, digitSpace, numberWidth);
}

bool TextureManager::DrawSymbol(NumberTextures numTextureArr, const Point2f& bottomLeft, Symbols symbol, const float symbolSpace, float& symbolWidth) const
{
	if (size_t(numTextureArr) > m_pDigitAndSymbolTextures.size())
	{
		std::cerr << "NumberTexture doesn't exist (not created) \n";
		return false;
	}

	const Texture* pSymbolTexture{ m_pDigitAndSymbolTextures[size_t(numTextureArr)][size_t(symbol)]};

	const Rectf symbDstRect{ bottomLeft.x + symbolSpace,
							bottomLeft.y,
							pSymbolTexture->GetWidth(),
							pSymbolTexture->GetHeight() };

	pSymbolTexture->Draw(symbDstRect);

	symbolWidth = pSymbolTexture->GetWidth();
	return true;
}

bool TextureManager::DrawSymbol(NumberTextures numTextureArr, const Point2f& bottomLeft, Symbols symbol, const float symbolSpace) const
{
	float symbolWidth{};
	return DrawSymbol(numTextureArr, bottomLeft, symbol, symbolSpace, symbolWidth);
}

bool TextureManager::DrawDigit(size_t textureIndex, const Point2f& bottomLeft, size_t digit, const float digitSpace, float& previousDigitsWidth) const
{
	if (digit > 9)
	{
		std::cerr << " digit is to large \n";
		return false;
	}

	const Texture* pNumTexture{ m_pDigitAndSymbolTextures[textureIndex][digit] };

	const Rectf numDstRect{ bottomLeft.x + previousDigitsWidth + digitSpace,
							bottomLeft.y,
							pNumTexture->GetWidth(),
							pNumTexture->GetHeight() };

	pNumTexture->Draw(numDstRect);

	previousDigitsWidth += pNumTexture->GetWidth();

	return true;
}

