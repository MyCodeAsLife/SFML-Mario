#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
public:
	std::vector<IntRect> m_frames;		// Массив обычных анимаций
	std::vector<IntRect> m_frames_flip;	// Массив отзеркаленых анимаций
	float m_currentFrame;				// Текущий кадр
	float m_speed;						// Скорость анимации
	bool m_flip;						// Отображать анимацию зеркальную\нет
	bool m_isPlaying;					// Проигровать/нет анимацию
	Sprite m_sprite;

public:
	Animation() {};
	Animation(const Texture& texture, int x, int y, int width, int height, int count, float speed, int step);
	void tick(const float time);	// Проигрывание анимации

};

class AnimationManager
{
public:
	std::string m_currentAnim;						// Имя текущей анимации
	std::map<std::string, Animation> m_animList;	// Массив/список соответствия имен анимациям

public:
	AnimationManager() {}
	void create(const std::string& name, const Texture& texture, int x, int y, int width, int height,
		int count, float speed, int step);						// Создание анимации
	void draw(RenderWindow& window, int x = 0, int y = 0);		// Отрисовка анимации в указанной позиции
	////// Функции для управления анимациями /////
	void set(const std::string& name) { m_currentAnim = name; }			// Выбор анимации 
	void flip(bool flip) { m_animList[m_currentAnim].m_flip = flip; }	// Вкл/выкл отзеркаливание
	void tick(float time) { m_animList[m_currentAnim].tick(time); }		// Промотка/проигрывание кадров анимации
	void pause() { m_animList[m_currentAnim].m_isPlaying = false; }		// Отсановка промотки/проигрывания кадров анимации
	void play() { m_animList[m_currentAnim].m_isPlaying = true; }		// Запуск промотки/проигрывания кадров анимации

};

Animation::Animation(const Texture& texture, int x, int y, int width, int height, int count, float speed, int step) :
	m_speed(speed), m_currentFrame(0), m_isPlaying(true), m_flip(false)
{
	m_sprite.setTexture(texture);

	// Создаем массивы прямоугольников
	for (int i(0); i < count; ++i)
	{
		m_frames.push_back(IntRect(x + i * step, y, width, height));
		m_frames_flip.push_back(IntRect(x + i * step + width, y, -width, height));
	}
}

inline void Animation::tick(const float time)
{
	if (!m_isPlaying)
		return;
	m_currentFrame += m_speed * time;
	if (m_currentFrame > m_frames.size())		// Переделать в цикл, чтобы случайно не выскочить за кол-во кадров
		m_currentFrame -= m_frames.size();
	if (m_flip)									// Выбираем обычную или повернутую анимацию
		m_sprite.setTextureRect(m_frames_flip[m_currentFrame]);
	else
		m_sprite.setTextureRect(m_frames[m_currentFrame]);
}

void AnimationManager::create(const std::string& name, const Texture& texture, int x, int y, int width, int height,
	int count, float speed, int step = 0)
{
	m_animList[name] = Animation(texture, x, y, width, height, count, speed, step);
	m_currentAnim = name;
}

inline void AnimationManager::draw(RenderWindow& window, int x, int y)
{
	m_animList[m_currentAnim].m_sprite.setPosition(x, y);		// Установка координат отрисовки
	window.draw(m_animList[m_currentAnim].m_sprite);			// Отрисовка
}
