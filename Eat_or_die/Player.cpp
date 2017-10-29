#include "Player.h"
#include<Windows.h>
#include<math.h>


#define M_PI 3.14159265359	


Player::Player()
{
	if (!texture.loadFromFile("../data/graph/player-move.png"))
	{
		MessageBox(NULL, "Player's texture not found", "ERROR", MB_OKCANCEL);
		return;
	}


	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));

	sprite.setOrigin(32, 32);


	status = STOJ;

	frame = 0;
	speed = 15;

	animClock.restart();
	sprite.setPosition(400, 400);
	lifeClock.reset();
	lifeClock.toggle();

	life = 110;
	thirst = 150;


}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite);
}

void Player::idz(sf::Vector2f mouse)
{
	status = IDZ;
}
void Player::stop()
{
	status = STOJ;
	frame = 0;
}
void Player::update(sf::Vector2f mysz, bool water, bool pauza)
{
	if (!pauza && (lifeClock.getElapsedTime() > sf::seconds(.1f)))
	{
		if (life>0)life--;
		if (water && thirst > 0) thirst--;
		lifeClock.reset();
		lifeClock.toggle();
	}

	sf::Vector2f norm = mysz - sprite.getPosition(); // vektor odleglosci playera od myszy 
	float rot = atan2(norm.y, norm.x); //kat miedzy nimi 
	rot = rot * 180 / M_PI; //kat w riadianach

	rot += 90; // plus 90 st bo obrazek trzeba obrazek przewrocic, bo player w nim patrzy do gory, nie przed siebie

	sprite.setRotation(rot);

	if (animClock.getElapsedTime() >  sf::seconds(0.09f))
	{
		if (status == STOJ) return;

		if (frame < 7)
			frame++;

		else frame = 0;

		sprite.move(getSpeed());
		animClock.restart();
	}

	sprite.setTextureRect(sf::IntRect(frame * 64, 0, 64, 64));

	if (sprite.getPosition().x >= 1280)
		sprite.setPosition(0, sprite.getPosition().y);
	else if (sprite.getPosition().x <= 0)
		sprite.setPosition(1280, sprite.getPosition().y);
	else if (sprite.getPosition().y >= 720)
		sprite.setPosition(sprite.getPosition().x, 0);
	else if (sprite.getPosition().y <= 0)
		sprite.setPosition(sprite.getPosition().x, 720);



}

Status Player::getStatus()
{
	return status;
}
sf::Vector2f Player::getPosition()
{
	return sprite.getPosition();
}
float Player::getRotation()
{
	return sprite.getRotation();
}
sf::FloatRect Player::getBoundingBox()
{
	return sprite.getGlobalBounds();
}
sf::Vector2f Player::getSpeed()
{
	float rotation = sprite.getRotation();
	float vx = sin((rotation * M_PI) / 180.0f);
	float vy = -cos((rotation * M_PI) / 180.0f);

	return sf::Vector2f(speed*vx, speed*vy);
}
void Player::restart()
{
	life = 110;
	sprite.setPosition(400, 400);
	thirst = 160;
	lifeClock.reset();
	lifeClock.toggle();
	animClock.restart();
}
Player::~Player()
{
}
