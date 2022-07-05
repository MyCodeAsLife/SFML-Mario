#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(640, 480), "This Mario!");
	window.setFramerateLimit(120);

	Texture hero_texture;
	hero_texture.loadFromFile("resources/images/fang.png");
	Sprite hero_sprite;
	hero_sprite.setTexture(hero_texture);
	hero_sprite.setTextureRect(IntRect(0, 244, 40, 50));
	hero_sprite.setOrigin(20, 25);
	hero_sprite.setPosition(70, 70);

	//bool moveToLeft(false);
	//bool moveToRight(false);


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			hero_sprite.move(-1.3f, 0.f);
			static float currentFrame(0);
			currentFrame += 0.07;
			if (currentFrame > 6)
				currentFrame -= 6;
			// Зеркальное отображение картинки
			hero_sprite.setTextureRect(IntRect(40 * static_cast<int>(currentFrame)+40, 244, -40, 50));
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			hero_sprite.move(1.3f, 0.f);
			static float currentFrame(0);
			currentFrame += 0.07;
			if (currentFrame > 6)
				currentFrame -= 6;
			hero_sprite.setTextureRect(IntRect(40 * static_cast<int>(currentFrame), 244, 40, 50));
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			hero_sprite.move(0.f, -1.3f);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			hero_sprite.move(0.f, 1.3f);
		}

		// Очистка экрана
		window.clear(Color(70, 70, 70));
		// Отрисовка персонажа
		window.draw(hero_sprite);
		window.display();
	}
	
	return 0;
}