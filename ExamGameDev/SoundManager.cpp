#include "pch.h"
#include "SoundManager.h"

#include "SoundStream.h"
#include "SoundEffect.h"

#include <iostream>
#include <cassert>

typedef std::map <std::string, std::pair<SoundStream*, unsigned int>>::iterator soundStreamMapIterator;
typedef std::map <std::string, std::pair<SoundEffect*, unsigned int>>::iterator soundEffectMapIterator;
typedef std::map <std::string, std::pair<SoundStream*, unsigned int>>::const_iterator soundStreamMapConstIterator;
typedef std::map <std::string, std::pair<SoundEffect*, unsigned int>>::const_iterator soundEffectMapConstIterator;

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
	constexpr size_t tenPercentJump{ 13 };

	const size_t currentVolume{ size_t(SoundStream::GetVolume()) };

	size_t newVol{};

	if (currentVolume == maxVol)
	{
		newVol = 0;
	}

	else
	{
		newVol = currentVolume + tenPercentJump;

		if (newVol > maxVol)
		{
			newVol = maxVol;
		}
	}

	SoundStream::SetVolume(newVol);
	return true;
}

bool SoundManager::AdjustGameSound()
{
	const soundEffectMapIterator startIt{ m_SoundEffectMap.begin() };

	constexpr size_t maxVol{ 128 };
	constexpr size_t tenPercentJump{ 13 };

	const size_t currentVolume{ size_t(startIt->second.first->GetVolume()) };

	size_t newVol{};

	if (currentVolume == maxVol)
	{
		newVol = 0;
	}

	else
	{
		newVol = currentVolume + tenPercentJump;

		if (newVol > maxVol)
		{
			newVol = maxVol;
		}
	}

	for (soundEffectMapIterator it{ startIt }; it != m_SoundEffectMap.end(); ++it)
	{
		it->second.first->SetVolume(newVol);
	}
	return true;
}
