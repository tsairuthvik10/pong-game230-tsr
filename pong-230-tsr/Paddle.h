#pragma once
#include "GameObject.h"
using namespace sf;
class Paddle: public GameObject
{
public:
	virtual void update(float time) override;
	virtual void render(RenderWindow &w)override{
		w.draw(paddle);
	};
	void moveDown(float time, const RenderWindow &w);
	void moveUp(float time, const RenderWindow &w);
	Vector2f getPosition() {
		return paddle.getPosition();
	};
	Vector2f getSize() {
		return paddle.getSize();
	};
	Paddle(bool human);
	~Paddle();
private:
	RectangleShape paddle;
	float speed = 250;
};

