#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Map.h"

using namespace sf;

const int WINDOW_HEIGHT(250);
const int WINDOW_WIDTH(400);

class Player
{
public:
	float m_dx, m_dy;	// Скорость по осям x/y
	FloatRect m_rect;	// Данные персонажа, координаты(x/y) и размер(ширина/высота) спрайта
	bool m_onGround;	// Состояние (в воздухе/на земле)
	Sprite m_sprite;	// Спарйт персонажа
	float m_currentFrame;	// Текущий кадр

public:
	Player(Texture& texture);
	void update(const float time);
	void collision(const bool dir);

};

class Enemy
{
public:
	float m_dx, m_dy;	// Скорость по осям x/y
	FloatRect m_rect;	// Данные персонажа, координаты(x/y) и размер(ширина/высота) спрайта
	bool m_onGround;	// Состояние (в воздухе/на земле)
	Sprite m_sprite;	// Спарйт персонажа
	float m_currentFrame;	// Текущий кадр
	bool m_life;			// Состояние жив/мертв

public:
	void set(Texture& texture, int x, int y);
	void update(const float time);
	void collision(const bool dir);

};
////////////////////////////////////////////////////////////START - MAIN///////////////////////////////////////////////
int main()
{
	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "This Mario!");
	window.setFramerateLimit(120);

	// Tile Set
	Texture tileSet_texture;
	tileSet_texture.loadFromFile("resources/images/Mario_Tileset.png");

	// Персонаж
	Player p1(tileSet_texture);
	// Противник
	Enemy en1;
	en1.set(tileSet_texture, 100, 100);
	// Карта
	Sprite tile_sprite(tileSet_texture);


	// Камера
	View camera;
	camera.reset(FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));


	// Для отрисовки карты
	RectangleShape rectangle(Vector2f(32,32));

	// Привязка скорости игры ко времени
	Clock clock;

	while (window.isOpen())
	{
		// Для привязки скорости анимации ко времени а не к тактовой частоте процессора(по умолчанию)
		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		time /= 5000.f;		// Чем меньше число, тем быстрее анимация
		clock.restart();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p1.m_dx = -0.4f;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p1.m_dx = 0.4f;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (p1.m_onGround)
			{
				p1.m_dy = -1.16f;		// Высота прыжка
				p1.m_onGround = false;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{

		}

		p1.update(time);

		//// Онимация бездействия
		//if (stay)
		//{
		//	if (leftRight)
		//	{
		//		static float currentFrame(0);
		//		currentFrame += 0.03f * time;
		//		if (currentFrame > 3.f)
		//			currentFrame -= 3.f;
		//		p1.m_sprite.setTextureRect(IntRect(43 * static_cast<int>(currentFrame), 190, 43, 48));
		//	}
		//	else
		//	{
		//		static float currentFrame(0);
		//		currentFrame += 0.03f * time;
		//		if (currentFrame > 3.f)
		//			currentFrame -= 3.f;
		//		p1.m_sprite.setTextureRect(IntRect(43 * static_cast<int>(currentFrame) + 43, 190, -43, 48));
		//	}
		//}
		//stay = true;

		// Очистка экрана
		window.clear(Color(70, 70, 70));
		// Отрисовка карты
		for (int i = 0; i < MAP_HEIGHT; i++)
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (TileMap[i][j] == 'P')  tile_sprite.setTextureRect(IntRect(143 - 16 * 3, 112, 16, 16));

				if (TileMap[i][j] == 'k')  tile_sprite.setTextureRect(IntRect(143, 112, 16, 16));

				if (TileMap[i][j] == 'c')  tile_sprite.setTextureRect(IntRect(143 - 16, 112, 16, 16));

				if (TileMap[i][j] == 't')  tile_sprite.setTextureRect(IntRect(0, 47, 32, 95 - 47));

				if (TileMap[i][j] == 'g')  tile_sprite.setTextureRect(IntRect(0, 16 * 9 - 5, 3 * 16, 16 * 2 + 5));

				if (TileMap[i][j] == 'G')  tile_sprite.setTextureRect(IntRect(145, 222, 222 - 145, 255 - 222));

				if (TileMap[i][j] == 'd')  tile_sprite.setTextureRect(IntRect(0, 106, 74, 127 - 106));

				if (TileMap[i][j] == 'w')  tile_sprite.setTextureRect(IntRect(99, 224, 140 - 99, 255 - 224));

				if (TileMap[i][j] == 'r')  tile_sprite.setTextureRect(IntRect(143 - 32, 112, 16, 16));

				if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0')) continue;

				tile_sprite.setPosition(j * 16, i * 16);
				window.draw(tile_sprite);
			}
		// Отрисовка персонажа
		window.draw(p1.m_sprite);

		// Отрисовка врага
		en1.update(time);
		window.draw(en1.m_sprite);

		// Отрисовка камеры
		int x = p1.m_rect.left + 8;		// Плюс половина ширины спрайта
		int y = p1.m_rect.top + 8;		// Плюс половина высоты спрайта
		if (p1.m_rect.left < (WINDOW_WIDTH / 2) - 8)	// Чтобы камера не уходила за левый край экрана
			x = WINDOW_WIDTH / 2;	// Разница с условием, равна половине ширины спрайта( иначе будут резкие прыжки камеры)
		if (p1.m_rect.top > (WINDOW_HEIGHT / 2) + 11)
			y = (WINDOW_HEIGHT / 2) + 22;
		camera.setCenter(x, y);	// Устанавливаем камеру по центру персонажа
		window.setView(camera);

		window.display();
	}
	
	return 0;
}
////////////////////////////////////////////////////////////END - MAIN///////////////////////////////////////////////

Player::Player(Texture& texture)
{
	m_sprite.setTexture(texture);
	m_rect = FloatRect(100, 180, 16, 16);
	m_dx = m_dy = 0.f;
	m_currentFrame = 0.f;
	m_onGround = false;
	m_sprite.setTextureRect(IntRect(112 + 31 * static_cast<int>(m_currentFrame), 144, 16, 16));
}

void Player::update(const float time)
{
	m_rect.left += m_dx * time;
	collision(0);	// Обработка колизии по х

	if (!m_onGround)
		m_dy += (0.01f * time);	// Притяжение к земле
	m_rect.top += (m_dy * time);
	m_onGround = false;
	collision(1);	// // Обработка колизии по y

	m_currentFrame += 0.05f * time;
	if (m_currentFrame > 3.f)
		m_currentFrame -= 3.f;
	if (m_dx > 0)
		m_sprite.setTextureRect(IntRect(112 + 31 * static_cast<int>(m_currentFrame), 144, 16, 16));
	if (m_dx < 0)
		m_sprite.setTextureRect(IntRect(112 + 31 * static_cast<int>(m_currentFrame) + 16, 144, -16, 16));
	m_sprite.setPosition(m_rect.left, m_rect.top);

	m_dx = 0;
}

void Player::collision(bool dir)
{

	for (int i = m_rect.top / 16; i < (m_rect.top + m_rect.height) / 16; i++)
		for (int j = m_rect.left / 16; j < (m_rect.left + m_rect.width) / 16; j++)
		{
			if ((TileMap[i][j] == 'P') || (TileMap[i][j] == 'k') || (TileMap[i][j] == '0') || (TileMap[i][j] == 'r') || (TileMap[i][j] == 't'))
			{
				if (m_dy > 0 && dir == true)
				{
					m_rect.top = i * 16 - m_rect.height;
					m_dy = 0;
					m_onGround = true;
				}
				if (m_dy < 0 && dir == true)
				{
					m_rect.top = i * 16 + 16;
					m_dy = 0;
				}
				if (m_dx > 0 && dir == false)
				{
					m_rect.left = j * 16 - m_rect.width;
				}
				if (m_dx < 0 && dir == false)
				{
					m_rect.left = j * 16 + 16;
				}
			}

			if (TileMap[i][j] == 'c') {
				// TileMap[i][j]=' '; 
			}
		}

}

void Enemy::set(Texture& texture, int x, int y)
{
	m_sprite.setTexture(texture);
	m_rect = FloatRect(x, y, 16, 16);
	m_dx = 0.05f;
	m_currentFrame = 0.f;
	m_life = true;
}

void Enemy::update(const float time)
{
	m_rect.left += m_dx * time;
	collision(0);

	if (!m_onGround)
		m_dy += (0.01f * time);	// Притяжение к земле
	m_rect.top += (m_dy * time);
	m_onGround = false;
	collision(1);

	m_currentFrame += time * 0.05f;
	if (m_currentFrame > 2)
		m_currentFrame -= 2;

	m_sprite.setTextureRect(IntRect(18 * static_cast<int>(m_currentFrame), 0, 16, 16));
	if (!m_life)
		m_sprite.setTextureRect(IntRect(58, 0, 16, 16));
	m_sprite.setPosition(m_rect.left, m_rect.top);
}

void Enemy::collision(const bool dir)
{
	for (int i = m_rect.top / 16; i < (m_rect.top + m_rect.height) / 16; i++)
		for (int j = m_rect.left / 16; j < (m_rect.left + m_rect.width) / 16; j++)
		{
			if ((TileMap[i][j] == 'P') || (TileMap[i][j] == 'k') || (TileMap[i][j] == '0') || (TileMap[i][j] == 'r') || (TileMap[i][j] == 't'))
			{
				if (m_dy > 0 && dir == true)
				{
					m_rect.top = i * 16 - m_rect.height;
					m_dy = 0;
					m_onGround = true;
				}
				if (m_dy < 0 && dir == true)
				{
					m_rect.top = i * 16 + 16;
					m_dy = 0;
				}
				if (m_dx > 0 && dir == false)
				{
					m_rect.left = j * 16 - m_rect.width;
				}
				if (m_dx < 0 && dir == false)
				{
					m_rect.left = j * 16 + 16;
				}
			}
		}
}
