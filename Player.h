#pragma once

#include "Anim.h"

class Player
{
public:
	float m_x, m_y;				// Положение в пространстве
	float m_dx, m_dy;			// Ускорение по осям
	float m_width, m_height;	// Ширина/высота объекта
	bool m_onLadder;			// На лестнице
	bool m_shoot;				// Стреляет
	bool m_hit;					// Получил урон(во временной неуязвимости)
	bool m_dir;					// Направление движения 1-Вправо, 0-Влево
	bool m_onGround;			
	AnimationManager m_anim;
	enum class State
	{
		stay,
		walk,
		duck,
		jump,
		climb,
		swim,
	} m_State;					// Текущее состояние объекта
	std::map<std::string, bool> m_key;	// Массив для информации о нажатии клавиш

public:
	Player(const AnimationManager& am);
	void KeyCheck();			// Обработка нажатия клавиш
	void update(float time);
	void collision(int dir);	// Обработка столкновений

};
/////////////////////////////////////////Определение методов/конструкторов классов//////////////////////////////////
Player::Player(const AnimationManager& am)
{
	// Начальное положение в пространстве
	m_x = 50;
	m_y = 100;
	m_width = 40/*m_anim.m_animList[m_anim.m_currentAnim].m_sprite.getTextureRect().width*/;
	m_height = 46/*m_anim.m_animList[m_anim.m_currentAnim].m_sprite.getTextureRect().width*/;
	m_onGround = false;

	// По уроку
	m_anim = am;
	m_State = State::stay;
}

inline void Player::KeyCheck()
{
	if (m_key["L"])		// Нажато влево
	{
		m_dir = 1;
		if (m_State != State::duck)		// Если лежал/сидел, то ползти?
			m_dx = -0.5f;
		if (m_State == State::stay)		// Если стоял, то идти
			m_State = State::walk;
	}
	if (m_key["R"])		// Нажато вправо
	{
		m_dir = 0;
		if (m_State != State::duck)		// Если лежал/сидел, то ползти?
			m_dx = 0.5f;
		if (m_State == State::stay)		// Если стоял, то идти
			m_State = State::walk;
	}
	if (m_key["Up"])		// Нажато вверх
	{
		if (m_State == State::stay || m_State == State::walk)	// Если стоял или шел то прыгает
		{
			m_dy = -1.15f;
			m_State = State::jump;
		}
		if (m_State == State::swim || m_State == State::climb)	// Если плавает или на лестнице, то медлено подымается
			m_dy = -0.05f;
	}
	if (m_key["Down"])	// Нажата вниз
	{
		if (m_State == State::stay || m_State == State::walk)	// Если мы стояли или шли, то ложимся
		{
			m_dx = 0.f;
			m_State = State::duck;
		}
		if (m_State == State::swim || m_State == State::climb)	// Если мы плаваем или на лестнице, то медленно опускаемся
		{
			m_dy = 0.05f;
		}
	}
	if (m_key["Space"])
		m_shoot = true;
	////////////////////Обработка отпускания клавиш
	if (!(m_key["R"] || m_key["L"]))	// Если не нажаты клавиши в право или в лево, то останавливаемся
	{
		m_dx = 0.f;
		if (m_State == State::walk)
			m_State = State::stay;
	}
	if (!(m_key["Up"] || m_key["Down"]))	
	{
		if (m_State == State::swim || m_State == State::climb)	// Если плывет или на лестнице, то останавливается
			m_dy = 0.f;
	}
	if (!m_key["Down"])
	{
		if (m_State == State::duck)		// Если лежал то встает
			m_State = State::stay;
	}
	if (!m_key["Space"])
	{
		m_shoot = false;
	}
}

inline void Player::update(float time)
{
	KeyCheck();
	if (m_State == State::stay)
		m_anim.set("stay");
	if (m_State == State::walk)
		m_anim.set("walk");
	if (m_State == State::jump)
		m_anim.set("jump");
	if (m_State == State::duck)
		m_anim.set("duck");
	if (m_State == State::climb)
	{
		m_anim.set("climb");
		m_anim.pause();
		if (m_dy != 0.f)
			m_anim.play();
		if (!m_onLadder)			// Если не на лестнице
			m_State = State::stay;
	}
	if (m_shoot)
	{
		m_anim.set("shoot");
		if (m_State == State::walk)
			m_anim.set("shootAndWalk");
	}
	m_anim.flip(m_dir);		// Поворот анимации



	m_x += m_dx * time;
	collision(0);	// Обработка колизии по х

	//if (!m_onGround)
	m_dy += (0.01f * time);	// Притяжение к земле
	m_y += (m_dy * time);
	//m_onGround = false;
	collision(1);	// // Обработка колизии по y

	//m_width = m_anim.m_animList[m_anim.m_currentAnim].m_frames[m_anim.m_animList[m_anim.m_currentAnim].m_currentFrame].width;
	//m_height = m_anim.m_animList[m_anim.m_currentAnim].m_frames[m_anim.m_animList[m_anim.m_currentAnim].m_currentFrame].height;

	m_anim.tick(time);
	m_key["R"] = m_key["L"] = m_key["Up"] = m_key["Down"] = m_key["Space"] = false;
}

inline void Player::collision(int dir)		// -------------Доработать---------------
{
	for (int i = m_y / 16; i < (m_y + m_height) / 16; i++)
		for (int j = m_x / 16; j < (m_x + m_width) / 16; j++)
		{
			if ((TileMap[i][j] == 'P') || (TileMap[i][j] == 'k') || (TileMap[i][j] == '0') || (TileMap[i][j] == 'r') || (TileMap[i][j] == 't'))
			{
				if (m_dy > 0 && dir == true)
				{
					m_y = i * 16 - m_height;
					m_dy = 0;
					m_onGround = true;
					m_State = State::stay;
				}
				if (m_dy < 0 && dir == true)
				{
					m_y = i * 16 + 16;
					m_dy = 0;
				}
				if (m_dx > 0 && dir == false)
				{
					m_x = j * 16 - m_width;
				}
				if (m_dx < 0 && dir == false)
				{
					m_x = j * 16 + 16;
				}
			}

			if (TileMap[i][j] == 'c') {
				// TileMap[i][j]=' '; 
			}
		}
}
