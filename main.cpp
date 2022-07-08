#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Map.h"
#include "Anim.h"

using namespace sf;

const int WINDOW_HEIGHT(250);
const int WINDOW_WIDTH(400);

class Player
{
public:
	float m_dx, m_dy;	// �������� �� ���� x/y
	FloatRect m_rect;	// ������ ���������, ����������(x/y) � ������(������/������) �������
	bool m_onGround;	// ��������� (� �������/�� �����)
	Sprite m_sprite;	// ������ ���������
	float m_currentFrame;	// ������� ����

public:
	Player(Texture& texture);
	void update(const float time);
	void collision(const bool dir);

};

class Enemy
{
public:
	float m_dx, m_dy;		// �������� �� ���� x/y
	FloatRect m_rect;		// ������ ���������, ����������(x/y) � ������(������/������) �������
	bool m_onGround;		// ��������� (� �������/�� �����)
	Sprite m_sprite;		// ������ ���������
	float m_currentFrame;	// ������� ����
	bool m_life;			// ��������� ���/�����

public:
	Enemy();
	void set(Texture& texture, int x, int y);
	void update(const float time);
	void collision(const bool dir);

};
////////////////////////////////////////////////////////////START - MAIN///////////////////////////////////////////////
int main()
{
	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "This Mario!");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(120);

	// Tile Set
	Texture tileSet_texture;
	tileSet_texture.loadFromFile("resources/images/Mario_Tileset.png");

	// ��������
	Player p1(tileSet_texture);
	// ���������
	Enemy en1;
	en1.set(tileSet_texture, 200, 100);
	// �����
	Sprite tile_sprite(tileSet_texture);
	// ����
	SoundBuffer buffer;
	buffer.loadFromFile("resources/sound/Jump.ogg");
	Sound jump_sound(buffer);
	
	Music theme_music;
	theme_music.openFromFile("resources/sound/Mario_Theme.ogg");
	theme_music.play();


	// ������
	View camera;
	camera.reset(FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	// �������� �������� ���� �� �������
	Clock clock;

	while (window.isOpen())
	{
		// ��� �������� �������� �������� �� ������� � �� � �������� ������� ����������(�� ���������)
		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		time /= 5000.f;		// ��� ������ �����, ��� ������� ��������
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
				jump_sound.play();
				p1.m_dy = -1.16f;		// ������ ������
				p1.m_onGround = false;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{

		}
		// ���������� ��������� � �����(��)
		p1.update(time);
		en1.update(time);

		// ��������� ������������ � ������
		if (p1.m_rect.intersects(en1.m_rect))	// ���� ������ ������ � ����� ������������
			if (en1.m_life)						// ���� ���� ���
				if (p1.m_dy > 0)				// ���� ����� ������
				{
					en1.m_life = false;
					en1.m_dx = 0.f;
					p1.m_dy = -0.8f;
				}		
				else                            // ���� ����� �� ������ 
				{
					p1.m_sprite.setColor(Color::Red);
				}

		// ������� ������
		window.clear(Color(70, 70, 70));
		// ��������� �����
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

		// ��������� ���������
		window.draw(p1.m_sprite);

		// ��������� �����
		window.draw(en1.m_sprite);

		// ��������� ������
		int x = p1.m_rect.left + 8;		// ���� �������� ������ �������
		int y = p1.m_rect.top + 8;		// ���� �������� ������ �������
		if (p1.m_rect.left < (WINDOW_WIDTH / 2) - 8)	// ����� ������ �� ������� �� ����� ���� ������
			x = WINDOW_WIDTH / 2;	// ������� � ��������, ����� �������� ������ �������( ����� ����� ������ ������ ������)
		if (p1.m_rect.top > ((MAP_HEIGHT * 16) / 2) + 28)	// ������������� �� ������ ����� � �� ������
			y = ((MAP_HEIGHT * 16) / 2) + 32;
		camera.setCenter(x, y);	// ������������� ������ �� ������ ���������
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
	collision(0);	// ��������� ������� �� �

	if (!m_onGround)
		m_dy += (0.01f * time);	// ���������� � �����
	m_rect.top += (m_dy * time);
	m_onGround = false;
	collision(1);	// // ��������� ������� �� y

	m_currentFrame += 0.05f * time;
	if (m_currentFrame > 3.f)
		m_currentFrame -= 3.f;
	if (m_dx > 0)
		m_sprite.setTextureRect(IntRect(112 + 30 * static_cast<int>(m_currentFrame), 144, 16, 16));
	if (m_dx < 0)
		m_sprite.setTextureRect(IntRect(112 + 30 * static_cast<int>(m_currentFrame) + 16, 144, -16, 16));
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

Enemy::Enemy()
{
	m_rect = FloatRect(100, 180, 16, 16);
	m_dx = m_dy = 0.f;
	m_currentFrame = 0.f;
	m_onGround = false;
	m_life = true;
}

void Enemy::set(Texture& texture, int x, int y)
{
	m_sprite.setTexture(texture);
	m_rect = FloatRect(x, y, 16, 16);
	m_dx = 0.15f;
}

void Enemy::update(const float time)
{
	m_rect.left += m_dx * time;
	collision(0);

	if (!m_onGround)
		m_dy += (0.01f * time);	// ���������� � �����
	m_rect.top += (m_dy * time);
	m_onGround = false;
	collision(1);

	m_currentFrame += time * 0.03f;
	if (m_currentFrame > 2)
		m_currentFrame -= 2;

	m_sprite.setTextureRect(IntRect(19 * static_cast<int>(m_currentFrame), 0, 16, 16));
	if (!m_life)	// ���� ������ �����, ���������� ���
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
					m_dx = -m_dx;
				}
				// ��������� else, ����� ��� ������������ ��-�� ��������� ����������� �������� ������ ...
				// ... ��������������� ������ �����������
				else if (m_dx < 0 && dir == false)
				{
					m_rect.left = j * 16 + 16;
					m_dx = -m_dx;
				}
			}
		}
}
