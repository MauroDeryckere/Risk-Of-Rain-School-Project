#pragma once
#include <map>
#include <vector>
class Texture;

class TextureManager final
{
	public:
		enum class Symbols
		{
			Zero = 0,
			Slash = 10,
			Plus = 11,
			Minus = 12,
			Colon = 13,
			Percent = 14,
			Dollar = 15
		};

		enum class NumberTextures
		{
			_20PxWhiteDigits = 0,
			_20PxGreenDigits = 1
		};

		TextureManager();
		~TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;
		TextureManager(TextureManager&&) = delete;
		TextureManager& operator=(TextureManager&&) = delete;

		[[nodiscard("must use texture Ptr")]] const Texture* CreateTexture(const std::string& filePath, const std::string& key);
		[[nodiscard("must use texture Ptr")]] const Texture* CreateTexture(const std::string& text, const std::string& filePath, const int fontSize, const Color4f& color, const std::string& key);

		//Reduce reference count by 1 (delete if == 0)
		bool DeleteTexture(const std::string& key);

		bool DrawNumber(NumberTextures numTextureArr, const Point2f& bottomLeft, size_t number, const float digitSpace, float& numberWidth) const;
		bool DrawNumber(NumberTextures numTextureArr, const Point2f& bottomLeft, size_t number, const float digitSpace) const;

		bool DrawSymbol(NumberTextures numTextureArr, const Point2f& bottomLeft, Symbols symbol, const float symbolSpace, float& symbolWidth) const;
		bool DrawSymbol(NumberTextures numTextureArr, const Point2f& bottomLeft, Symbols symbol, const float symbolSpace) const;

	private:
		/* map with pair of Texture ptr and amount of references to that texture at the moment
			deletes Texture when ref count = 0 or when manager object is destroyed
		*/
		std::map <std::string, std::pair<const Texture*, size_t>> m_pTextures;

		std::vector<std::vector<const Texture*>> m_pDigitAndSymbolTextures;

		void InitializeDigitAndSymbolArr();

		void CreateDigitAndSymbolArr(const std::string& filePath, const int fontSize, const Color4f& color);
		bool DrawDigit(size_t textureIndex, const Point2f& bottomLeft, size_t digit, const float digitSpace, float& previousDigitsWidth) const;
};