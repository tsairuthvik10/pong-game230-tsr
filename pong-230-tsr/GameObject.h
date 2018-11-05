#pragma once
#include <vector>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h> 
using namespace std;
using namespace sf;
class GameObject {
public:
	virtual void update(float time) = 0;
	virtual void render(RenderWindow &w) = 0;
	void add(GameObject *go) {
		graph.push_back(go);
	};

private:
	vector<GameObject*> graph;
};