#ifndef FUHRER_H
#define FUHRER_H

#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cmath>
#include <vector>
#include <initializer_list>
#include <utility>

#define M_PI 3.14159265358979323846

struct FuArm : public sf::Drawable
{
	FuArm() : FuArm(0,0,sf::Color::White) {};
	FuArm(const float r, const float s, const sf::Color c, const bool outline = false, const float deg = 0)
		: _radius{ r }, _speed{ s }, _line{ sf::VertexArray{sf::Lines, 2} }, _deg{ deg }, _cir{ sf::CircleShape{r} }, _isOutline{outline} {
		_cir.setFillColor(sf::Color::Transparent);
		_cir.setOutlineColor(sf::Color::White);
		_cir.setOutlineThickness(0.5);
		_line[0].color = _line[1].color = c;
	};
	sf::Vector2f update(const sf::Vector2f start, float spdMult)
	{
		float d = _deg / 180 * M_PI;
		sf::Vector2f end(start.x + _radius * cos(d), start.y + _radius * sin(d));
		_line[0].position = start;
		if(_isOutline) _cir.setPosition(start.x - _radius, start.y - _radius);
		_line[1].position = end;
		_deg += _speed * spdMult;
		if (_deg > 360) 
			_deg -= 360 * ((int)_deg / 360);
		if (_deg < -360) 
			_deg -= 360 * ((int)_deg / 360);
		return end;
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const final
	{
		target.draw(_line);
		if (_isOutline) target.draw(_cir);
	}
private:
	//sf::Color _col;
	sf::VertexArray _line;
	sf::CircleShape _cir;
	float _radius;
	float _speed;
	float _deg = 0;
	bool _isOutline;
};

class Fourier : public sf::Drawable
{
public:
	Fourier(std::initializer_list<float> ls, bool rangeOutline = false) : _fa{ std::vector<FuArm>((size_t)ls.size() / 2)} {
		
		for (size_t i{};i<ls.size(); i+=2)
		{
			
			_fa[i / 2] = FuArm(ls.begin()[i], ls.begin()[i + 1],
				sf::Color((i * 150) % 255, (i * 110 + 230) % 255, (i * 230 + 156) % 255),
				rangeOutline
			);
		}
	}
	Fourier(std::initializer_list<std::initializer_list<float>> ls, bool rangeOutline = false) : _fa{ std::vector<FuArm>((size_t)ls.size()) } {

		for (size_t i{}; i < ls.size(); i ++)
		{
			_fa[i] = FuArm(ls.begin()[i].begin()[0], ls.begin()[i].begin()[1],
				sf::Color((i * 150) % 255, (i * 110 + 230) % 255, (i * 230 + 156) % 255),
				rangeOutline
			);
		}
	}
	Fourier(std::vector<std::pair<float,float>> ls, bool rangeOutline = false) 
	: _fa{ std::vector<FuArm>((size_t)ls.size()) } {
		for (size_t i{}; i < ls.size(); i++)
		{
			_fa[i] = FuArm(ls[i].first, ls[i].second,
				sf::Color((i * 150) % 255, (i * 110 + 230) % 255, (i * 230 + 156) % 255),
				rangeOutline
			);
		}
	}
	sf::Vector2f update(const sf::Vector2f start, float spdMult)
	{
		sf::Vector2f end = start;
		for (auto& i : _fa)
		{
			end = i.update(end,spdMult);
		}
		
		return end;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const final
	{
		for (auto& i : _fa)
		{
			i.draw(target,states);
		}
	}
	
private:
	std::vector<FuArm> _fa;
};

#endif