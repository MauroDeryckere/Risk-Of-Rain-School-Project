#include "pch.h"
#include "SoundManager.h"

#include "SoundStream.h"
#include "SoundEffect.h"

#include <iostream>
#include <cassert>

typedef std::map <std::string, std::pair<SoundStream*, size_t>>::iterator soundStreamMapIterator;
typedef std::map <std::string, std::pair<SoundEffect*, size_t>>::iterator soundEffectMapIterator;
typedef std::map <std::string, std::pair<SoundStream*, size_t>>::const_iterator soundStreamMapConstIterator;
typedef std::map <std::string, std::pair<SoundEffect*, size_t>>::const_iterator soundEffectMapConstIterator;

SoundManager::SoundManager() :
	m_CurrentMusicVolPercent{ 100 },
	m_CurrentGameVolPercent {100}
{
}

SoundManager::~SoundManager()
{
	for (soundStreamMapIterator it{ m_SoundStreamMap.begin() }; it != m_SoundStreamMap.end(); ++it)
	{
		delete it->second.first;
	}

	for (soundEffectMapIterator it{ m_SoundEffectMap.begin() }; it != m_SoundEffectMap.end(); ++it)
	{
		delete it->second.first;
	}
}

SoundStream* SoundManager::CreateSoundStream(const std::string& key, const std::string& filePath)
{
	soundStreamMapIterator it{ m_SoundStreamMap.find(key) };

	if (it == m_SoundStreamMap.end())
	{
		it = m_SoundStreamMap.emplace(key, std::make_pair(new SoundStream{filePath}, 1)).first;

		assert(it->second.first->IsLoaded());
	}
	else
	{
		it->second.second++;
	}

	return it->second.first;
}

SoundEffect* SoundManager::CreateSoundEffect(const std::string& key, const std::string& filePath)
{
	soundEffectMapIterator it{ m_SoundEffectMap.find(key) };

	if (it == m_SoundEffectMap.end())
	{
		it = m_SoundEffectMap.emplace(key, std::make_pair(new SoundEffect{ filePath }, 1)).first;

		assert(it->second.first->IsLoaded());
	}
	else
	{
		it->second.second++;
	}

	return it->second.first;
}

bool SoundManager::DeleteSoundStream(const std::string& key)
{
	soundStreamMapIterator it{ m_SoundStreamMap.find(key) };

	if (it == m_SoundStreamMap.end())
	{
		std::cerr << key << " SoundStream doesn't exist \n";
		return false;
	}

	if (--it->second.second == 0)
	{
		delete it->second.first;
		m_SoundStreamMap.erase(it);
		return true;
	}

	return false;
}

bool SoundManager::DeleteSoundEffect(const std::string& key)
{
	soundEffectMapIterator it{ m_SoundEffectMap.find(key) };

	if (it == m_SoundEffectMap.end())
	{
		std::cerr << key << " SoundEffect doesn't exist \n";
		return false;
	}
	
	if (--it->second.second == 0)
	{
		delete it->second.first;
		m_SoundEffectMap.erase(it);
		return true;
	}

	return false;
}

bool SoundManager::PauseAll()
{
	SoundStream::Pause();
	SoundEffect::PauseAll();
	return true;
}

bool SoundManager::ResumeAll()
{
	SoundStream::Resume();
	SoundEffect::ResumeAll();
	return true;
}

bool SoundManager::AdjustMusicSound()
{
	constexpr size_t maxVol{ 128 };

	if (m_CurrentMusicVolPercent == 100)
	{
		m_CurrentMusicVolPercent = 0;
	}
	else
	{
		m_CurrentMusicVolPercent += 10;
	}

	size_t newVol{ (m_CurrentMusicVolPercent == 0 ? 0 : std::min(maxVol, maxVol / 100 * m_CurrentMusicVolPercent)) };

	SoundStream::SetVolume(newVol);

	return true;
}

bool SoundManager::AdjustGameSound()
{
	constexpr size_t maxVol{ 128 };

	if (m_CurrentGameVolPercent == 100)
	{
		m_CurrentGameVolPercent = 0;
	}
	else
	{
		m_CurrentGameVolPercent += 10;
	}

	size_t newVol{ (m_CurrentGameVolPercent == 0 ? 0 : std::min(maxVol, maxVol / 100 * m_CurrentGameVolPercent)) };

	for (soundEffectMapIterator it{ m_SoundEffectMap.begin() }; it != m_SoundEffectMap.end(); ++it)
	{
		it->second.first->SetVolume(newVol);
	}

	return true;
}

size_t SoundManager::GetMusicVolumePercent() const
{
	return m_CurrentMusicVolPercent;
}

size_t SoundManager::GetGameVolumePercent() const
{
	return m_CurrentGameVolPercent;
}
