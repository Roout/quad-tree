#include "Graphics.h"
#include "QuadTree.h"
#include "MainScene.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(850, 600), "Quad Tree", sf::Style::Titlebar | sf::Style::Close);	
	mercury::MainScene scene{ &window };
	sf::Clock clock;
	clock.restart();
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else {
				(void) scene.ProcessEvent(event);
			}
		}
		sf::Time elapsed = clock.restart();
		scene.Update(elapsed.asSeconds());
		window.clear();
		window.draw(scene);
		window.display();
	}

	return 0;
}