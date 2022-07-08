#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
public:
	std::vector<IntRect> m_frames;		// ������ ������� ��������
	std::vector<IntRect> m_frames_flip;	// ������ ������������ ��������
	float m_currentFrame;				// ������� ����
	float m_speed;						// ��������
	bool m_flip;						// ���������� �������� ����������\���
	bool m_isPlaying;					// �����������/��� ��������
	Sprite m_sprite;

public:
	Animation(const Texture& texture, int x, int y, int width, int height, int count, float speed, int step);
	void tick(const float time);	// ������������ ��������

};

class AnimationManager
{
public:
	std::string m_currentAnim;						// ��� ������� ��������
	std::map<std::string, Animation> m_animList;	// ������/������ ������������ ���� ���������

public:
	AnimationManager() {};
	void create(const std::string& name, const Texture& texture, int x, int y, int width, int height, int count);

};

Animation::Animation(const Texture& texture, int x, int y, int width, int height, int count, float speed, int step) :
	m_speed(speed), m_currentFrame(0), m_isPlaying(true), m_flip(false)
{
	m_sprite.setTexture(texture);

	// ������� ������� ���������������
	for (int i(0); i < count; ++i)
	{
		m_frames.push_back(IntRect(x + i * step + width, y, width, height));
		m_frames_flip.push_back(IntRect(x + i * step, y, -width, height));
	}
}

inline void Animation::tick(const float time)
{
	if (!m_isPlaying)
		return;
	m_currentFrame += m_speed * time;
	if (m_currentFrame > m_frames.size())		// ���������� � ����, ����� �������� �� ��������� �� ���-�� ������
		m_currentFrame -= m_frames.size();
	if (m_flip)									// �������� ������� ��� ���������� ��������
		m_sprite.setTextureRect(m_frames_flip[m_currentFrame]);
	else
		m_sprite.setTextureRect(m_frames[m_currentFrame]);
}

void AnimationManager::create(const std::string& name, const Texture& texture, int x, int y, int width, int height, int count)
{

}