#include "Ball.h"

sf::Texture texture;


float random() {
	return rand();
}
void randomizeSpeed(float &x, float &y) {
	float r1 = random();
	if (r1 > RAND_MAX/2) x = 200;
	else x = -200;
	float r2 = random();
	if (r2 > RAND_MAX/2) y = 200;
	else y = -200;
}
void Ball::update(float time)
{
	Vector2f pos = ball.getPosition();
	pos.x += velocity.x * time;
	pos.y += velocity.y * time;
	ball.setPosition(pos);
}

Ball::Ball()
{
	ball.setSize(Vector2f(20, 20));
	ball.setPosition(400, 300);
	randomizeSpeed(speedX, speedY);
	velocity.x = speedX;
	velocity.y = speedY;
	if (!texture.loadFromFile("puck.png"))
	{
		// error...
	}
	ball.setTexture(&texture);
}



void Ball::increaseVel()
{
	if (velocity.x < 0) {
		velocity.x -= accel;
	}
	else {
		velocity.x += accel;
	}
	
}

void Ball::reset()
{
	ball.setPosition(400, 300);
	randomizeSpeed(speedX, speedY);
	velocity.x = speedX;
	velocity.y = speedY;
	first = false;
}

Ball::~Ball()
{
}
