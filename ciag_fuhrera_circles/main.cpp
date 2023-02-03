#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <utility>

#include "Fourier.h"

#define XSIZE 1200
#define YSIZE 900

int main()
{
	std::ifstream wej("note.txt");
	std::vector<std::pair<float,float>> data;
	while (wej.good()){
		std::pair<float, float> dataln;
		wej >> dataln.first >> dataln.second;
		data.push_back(dataln);
	}

	sf::RenderWindow window(sf::VideoMode(XSIZE, YSIZE), "Fuhrer", sf::Style::Titlebar | sf::Style::Close); // | sf::Style::Fullscreen 
	sf::Event event;
	sf::Clock clock;
	window.setFramerateLimit(300);
	float FPS;
	unsigned frame_counter = 0;

	sf::RenderTexture rendertexture;
	rendertexture.create(XSIZE, YSIZE);
	rendertexture.clear();
	sf::Sprite sprite;
	sprite.setTexture(rendertexture.getTexture());

	Fourier fourier{ data, true };
	//Fourier fourier{50,10,40,-5};
	//Fourier fourier{ {50,10},{40,5},{80,12}, {60, -24} };
	//Fourier fourier{ {100,5}, {70,10}, {50,20}, {30, 40}, {5, 80} };
	//Fourier fourier{ {{256,2}, {128,4}, {64,8}, {32,16}, {16,32}, {4,64}, {2,128}} };
	//Fourier fourier{ {100,10},{70,40} };
	//Fourier fourier{ {50,10} };

	sf::Vector2f lstPoint (-1,-1);
	sf::VertexArray tail(sf::Lines, 2);
	tail[0].color = tail[1].color = sf::Color(255, 255, 0);

	float speed = 0.2;

	sf::err().rdbuf(NULL); //gamepad error suspenion

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) window.close();
				if (event.key.code == sf::Keyboard::Up) { speed *= 1.1; }
				if (event.key.code == sf::Keyboard::Down) { speed *= 1/1.1; }
				if (event.key.code == sf::Keyboard::C) { rendertexture.clear(); }
			}
		}
		window.clear(sf::Color::Black);
		//////////////////

		sf::Vector2f pointPos = fourier.update(sf::Vector2f(XSIZE / 2, YSIZE / 2),speed);
		
		if (lstPoint.x == -1 && lstPoint.y == -1) lstPoint = pointPos;
		tail[0].position = sf::Vector2f(lstPoint.x,YSIZE - lstPoint.y);
		tail[1].position = sf::Vector2f(pointPos.x, YSIZE - pointPos.y);
		lstPoint = pointPos;
		rendertexture.draw(tail);

		frame_counter++;
		if (clock.getElapsedTime().asSeconds() >= 1.0)
		{
			FPS = frame_counter / clock.getElapsedTime().asSeconds();
			clock.restart();
			frame_counter = 0;
			//auto tmp = std::string(FPS);
			window.setTitle("Fuhrer (" + std::to_string(FPS) + sf::String(")"));
		}

		window.draw(sprite);
		window.draw(fourier);
		window.display();
	}
	return 0;
}