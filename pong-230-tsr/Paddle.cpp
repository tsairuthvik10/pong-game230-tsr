#include "Paddle.h"



void Paddle::update(float time)
{
	
}

void Paddle::moveUp(float time, const RenderWindow &w)
{
	Vector2f pos = paddle.getPosition();
	if ((pos.y - speed * time) < 0) {
		pos.y = 0;
	}
	else {
		pos.y -= speed * time;
	}
	paddle.setPosition(pos);
}
void Paddle::moveDown(float time, const RenderWindow &w)
{
	Vector2f pos = paddle.getPosition();
	if ((pos.y + speed * time) > (w.getSize().y - paddle.getSize().y)) {
		pos.y = w.getSize().y - paddle.getSize().y;
	}
	else {
		pos.y += speed * time;
	}
	paddle.setPosition(pos);
}

Paddle::Paddle(bool human)
{
	paddle.setSize(Vector2f(20, 150));
	if (human) {
		paddle.setPosition(Vector2f(0, 225));
	}
	else {
		paddle.setPosition(Vector2f(780, 225));
		speed = 200;
	}
}


Paddle::~Paddle()
{
}
