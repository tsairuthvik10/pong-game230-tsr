#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <time.h>
#include "Ball.h"
#include "Paddle.h"

using namespace sf;

struct Barrier {
private:

	float speed = 200;
public:
	RectangleShape rect;
	Barrier() {
		rect.setSize(Vector2f(20, 150));
		rect.setPosition(Vector2f(390, 0));
	};
	void update(float dt) {
		Vector2f pos = rect.getPosition();
		if (rect.getPosition().y > 600) {
			rect.setPosition(390, 600);
			speed *= -1;
		}
		else if (rect.getPosition().y < 0 - rect.getSize().y) {
			rect.setPosition(390, 0 - rect.getSize().y);
			speed *= -1;
		}
		pos.y += speed * dt;
		rect.setPosition(pos);
	};
	void render(RenderWindow &w) {
		w.draw(rect);
	};

};

void update_state(float dt);
void render_frame();
void move_ball(float dt, Ball &b);
void checkBarrierCollision(Ball &b);
float dist(Vector2f obj1, Vector2f obj2);

bool pause = false;
RenderWindow window;
const double PI = 3.141592653589793238463;
SoundBuffer buf1, buf2, scorebuf, cheerbuf, boobuf, gamepointbuf;
Sound ping1, ping2, score, cheer, boo, gamepoint;
Music start;
char score_one;
char score_two;
Font font;
Text s1, s2, playAgainMessage;
vector<Ball> *barr = new vector<Ball>[5];
Ball b;
Paddle p1(true);
Paddle p2(false);
RectangleShape background;
Texture backTex;
Barrier barrier;




int main()
{

	srand(time(NULL));
	window.create(VideoMode(800, 600), "AIR PONG HOCKEY");
	font.loadFromFile("../Assets/impact.ttf");

	score_one = '0';
	score_two = '0';
	s1.setFont(font);
	s1.setCharacterSize(50);
	s1.setString(score_one);
	s1.setFillColor(Color::Green);
	s1.setPosition(50, 0);
	s2.setFont(font);
	s2.setCharacterSize(50);
	s2.setString(score_two);
	s2.setFillColor(Color::Green);
	s2.setPosition(700, 0);

	

	playAgainMessage.setFont(font);
	playAgainMessage.setCharacterSize(50);
	playAgainMessage.setFillColor(Color::Green);
	playAgainMessage.setPosition(0, 0);

	//create sounds and music
	buf1.loadFromFile("resources/puckhit.wav");
	ping1.setBuffer(buf1);

	buf2.loadFromFile("resources/puckhit.wav");
	ping2.setBuffer(buf2);

	scorebuf.loadFromFile("resources/goal.wav");
	score.setBuffer(scorebuf);

	cheerbuf.loadFromFile("resources/cheer.wav");
	cheer.setBuffer(cheerbuf);

	boobuf.loadFromFile("resources/boo.wav");
	boo.setBuffer(boobuf);

	gamepointbuf.loadFromFile("resources/gamepoint.wav");
	gamepoint.setBuffer(gamepointbuf);

	if (!start.openFromFile("resources/startsong.ogg"))
		return -1; // error
	start.play();
	start.setLoop(true);
	start.setVolume(50.f);

	//create background
	background.setSize(Vector2f(800, 600));
	backTex.loadFromFile("table.png");
	background.setTexture(&backTex);

	barr->push_back(b);

	//GameObject *SceneGraph;
	Clock clock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Space && !pause) {
					if (barr->size() < 6) {
						Ball b;
						barr->push_back(b);
					}
				}
			}
		}

		float dt = clock.restart().asSeconds();

		update_state(dt);
		render_frame();
		window.display();
	}

	return 0;
}

void update_state(float dt)
{
	if (!pause) {
		if (Keyboard::isKeyPressed(Keyboard::Up))
			p1.moveUp(dt, window);
		if (Keyboard::isKeyPressed(Keyboard::Down))
			p1.moveDown(dt, window);

		Ball b;
		float closest = 0;
		for (vector<Ball>::iterator i = barr->begin(); i != barr->end(); ++i) {
			if ((*i).getVelocity().x > 0 && (*i).getPosition().x > closest) {
				b = (*i);
				closest = (*i).getPosition().x;
			}
		}
		if (b.getVelocity().x > 0 && b.getPosition().x > window.getSize().x / 4) {
			float midpoint = p2.getPosition().y + p2.getSize().y / 2;
			if (b.getPosition().y < midpoint)
				p2.moveUp(dt, window);
			if (b.getPosition().y > midpoint)
				p2.moveDown(dt, window);
		}
		for (vector<Ball>::iterator i = barr->begin(); i != barr->end(); ++i) {
			move_ball(dt, (*i));
		}
		barrier.update(dt);
	}
	else {
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			score_one = '0';
			score_two = '0';
			s1.setString(score_one);
			s2.setString(score_two);
			pause = false;
			barr->clear();
			b.reset();
			barr->push_back(b);
		}
	}

	//if (Keyboard::isKeyPressed(Keyboard::Space) && sound.getStatus() != SoundSource::Playing)


}
void move_ball(float dt, Ball &b) {
	Vector2f bpos = b.getPosition();
	const Vector2f bsize = b.getSize();
	const Vector2f ppos = p1.getPosition();
	const Vector2f psize = p1.getSize();
	const Vector2f ppos2 = p2.getPosition();
	const Vector2f psize2 = p2.getSize();
	if (bpos.y > window.getSize().y - bsize.y) {
		bpos.y = window.getSize().y - bsize.y;
		b.setPosition(bpos);
		b.reverseY();
		ping2.play();
	}
	else if (bpos.y < 0) {
		bpos.y = 0;
		b.setPosition(bpos);
		b.reverseY();
		ping2.play();
	}
	if (bpos.x < ppos.x + psize.x &&
		bpos.x > ppos.x + psize.x / 2 &&
		bpos.y + bsize.y >= ppos.y &&
		bpos.y <= ppos.y + psize.y)
	{
		b.first = true;
		bpos.x = ppos.x + psize.x;
		b.setPosition(bpos);
		b.reverseX();
		b.increaseVel();
		Vector2f vel = b.getVelocity();
		float midpoint = ppos.y + psize.y / 2;
		float distance = dist(Vector2f(ppos.x + psize.x, midpoint), Vector2f(bpos.x, bpos.y + bsize.y / 2));
		if (vel.y > 0) {
			if (bpos.y > midpoint) {
				vel.y = -distance * 2;
			}
			else {
				vel.y = distance * 2;
			}
		}
		else {
			if (bpos.y > midpoint) {
				vel.y = -distance * 2;
			}
			else {
				vel.y = distance * 2;
			}
		}
		b.setVelocity(vel);
		ping1.play();
	}
	if (bpos.x + bsize.x > ppos2.x &&
		bpos.x + bsize.x < ppos2.x + psize2.x / 2 &&
		bpos.y + bsize.y >= ppos2.y &&
		bpos.y <= ppos2.y + psize2.y)
	{
		b.first = true;
		bpos.x = ppos2.x - bsize.x;
		b.setPosition(bpos);
		b.reverseX();
		b.increaseVel();
		Vector2f vel = b.getVelocity();
		float midpoint = ppos2.y + psize2.y / 2;
		float distance = dist(Vector2f(ppos2.x, midpoint), Vector2f(bpos.x + bsize.x, bpos.y + bsize.y / 2));
		//int angle = (int)distance % 300;
		if (vel.y > 0) {
			if (bpos.y > midpoint) {
				vel.y = -distance * 2;
			}
			else {
				vel.y = distance * 2;
			}
		}
		else {
			if (bpos.y > midpoint) {
				vel.y = -distance * 2;
			}
			else {
				vel.y = distance * 2;
			}
		}
		b.setVelocity(vel);
		ping1.play();
	}

	checkBarrierCollision(b);

	if (bpos.x + bsize.x > window.getSize().x) {
		score.play();
		if (++score_one == '5') {
			pause = true;
			cheer.play();
			playAgainMessage.setString("You won!\nPress space to continue\n the winning streak!");
		}
		s1.setString(score_one);
		b.reset();
	}
	else if (bpos.x < 0) {
		score.play();
		if (++score_two == '5') {
			pause = true;
			boo.play();
			playAgainMessage.setString("You lost!\nPress space to get\n revenge from the Freak!");
		}
		s2.setString(score_two);
		b.reset();
	}


	b.update(dt);

}
void checkBarrierCollision(Ball &b) {
	if (b.first == false) return;
	Vector2f bpos = b.getPosition();
	Vector2f ppos = barrier.rect.getPosition();
	Vector2f bsize = b.getSize();
	Vector2f psize = barrier.rect.getSize();
	if (bpos.x < ppos.x + psize.x &&
		bpos.x > ppos.x + psize.x / 2 &&
		bpos.y + bsize.y >= ppos.y &&
		bpos.y <= ppos.y + psize.y) {
		bpos.x = ppos.x + psize.x;
		b.setPosition(bpos);
		b.reverseX();
		ping1.play();
	}
	if (bpos.x + bsize.x > ppos.x &&
		bpos.x + bsize.x < ppos.x + psize.x / 2 &&
		bpos.y + bsize.y >= ppos.y &&
		bpos.y <= ppos.y + psize.y) {
		bpos.x = ppos.x - bsize.x;
		b.setPosition(bpos);
		b.reverseX();
		ping1.play();
	}
}
void render_frame()
{
	window.clear();
	window.draw(background);
	if (!pause) {
		window.draw(s1);
		window.draw(s2);
		p1.render(window);
		p2.render(window);
		barrier.render(window);
		for (vector<Ball>::iterator i = barr->begin(); i != barr->end(); ++i) {
			(*i).render(window);
		}

	}
	else {
		window.draw(playAgainMessage);
	}
}
float dist(Vector2f obj1, Vector2f obj2) {
	return 50 + sqrt((obj2.x - obj1.x) * (obj2.x - obj1.x) + (obj2.y - obj1.y) * (obj2.y - obj1.y));
}