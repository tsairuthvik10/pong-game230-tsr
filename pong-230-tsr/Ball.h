#pragma once
#include "GameObject.h"

using namespace sf;
class Ball : public GameObject
{
public:

	bool first = false;

	virtual void update(float time) override;
	virtual void render(RenderWindow &w) override {
		w.draw(ball);
	};
	Ball();
	void reverseY() {
		velocity.y *= -1;
	};
	void reverseX() {
		velocity.x *= -1;
	};
	Vector2f getVelocity() {
		return velocity;
	};
	void setVelocity(Vector2f vel) {
		velocity = vel;
	};
	Vector2f getPosition() {
		return ball.getPosition();
	};
	void setPosition(Vector2f pos) {
		ball.setPosition(pos);
	};
	Vector2f getSize() {
		return ball.getSize();
	};
	
	void increaseVel();
	void reset();
	~Ball();
private: 
	RectangleShape ball;
	Vector2f velocity;
	float accel = 30;
	float speedY;
	float speedX;

	
};

