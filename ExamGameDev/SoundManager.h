#pragma once
#include <map>

class SoundStream;
class SoundEffect;

class SoundManager final
{
	public:
		SoundManager();
		~SoundManager();

		SoundManager(const SoundManager&) = delete;
		SoundManager& operator=(const SoundManager&) = delete;
		SoundManager(SoundManager&&) = delete;
		SoundManager& operator=(SoundManager&&) = delete;

		[[nodiscard("must use soundStr Ptr")]] SoundStream* CreateSoundStream(const std::string& key, const std::string& filePath);
		[[nodiscard("must use soundEff Ptr")]] SoundEffect* CreateSoundEffect(const std::string& key, const std::string& filePath);

		bool DeleteSoundStream(const std::string& key);
		bool DeleteSoundEffect(const std::string& key);

		bool PauseAll();
		bool ResumeAll();

		bool AdjustMusicSound();
		bool AdjustGameSound();

		size_t GetMusicVolumePercent() const;
		size_t GetGameVolumePercent() const;

	private:
		/* map with pair of SoundEff or SoundStr ptr and amount of references to that sound at the moment
			deletes sound when ref count = 0 or when manager object is destroyed
		*/
		std::map<std::string, std::pair<SoundStream*, size_t>> m_SoundStreamMap;
		std::map<std::string, std::pair<SoundEffect*, size_t>> m_SoundEffectMap;

		size_t m_CurrentMusicVolPercent;
		size_t m_CurrentGameVolPercent;
};

