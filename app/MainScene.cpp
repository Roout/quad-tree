#include "MainScene.h"
#include "QuadTreeLayout.h"
#include "Interface.h"
#include "Graphics.h"
#include "QuadTree.h"

#include <iostream>

namespace mercury {

	MainScene::MainScene(sf::RenderWindow * window)
		: m_window{ window }
	{
		this->Init();
	}

	MainScene::~MainScene() {
		for (auto& child : m_children) {
			delete child;
			child = nullptr;
		}
	}

	void MainScene::Init() {
		mt::Rect rect{ 0.f, 0.f, 600.f, 600.f };
		m_tree = std::make_unique<tree::QuadTree>(rect);
		m_treeLayout = new QuadTreeLayout{ m_tree.get() };
		this->AddChild(m_treeLayout);

		auto description = new Description(sf::IntRect{ 0, 0, 200, 600 }, m_tree.get());
		description->setPosition(600.f, 0.f);
		this->AddChild(description);

		this->AddEventListener(std::bind(&MainScene::EventListener, this, std::placeholders::_1));
	}

	bool MainScene::EventListener(const sf::Event& event) {
		switch (event.type) {
			case sf::Event::MouseButtonPressed: {
				if (event.mouseButton.button == sf::Mouse::Right) {
					const mt::Pt point{
						static_cast<float>(event.mouseButton.x),
						static_cast<float>(event.mouseButton.y)
					};
					std::cerr << "Trying to insert point: " << point.x << " " << point.y << '\n';
					m_tree->Insert(point);
				}
			} break;
			case sf::Event::MouseMoved: {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					const mt::Pt point{
						static_cast<float>(event.mouseMove.x),
						static_cast<float>(event.mouseMove.y)
					};
					std::cerr << "Trying to insert point: " << point.x << " " << point.y << '\n';
					m_tree->Insert(point);
				}
			} break;
			default: break;
		}
		return true;
	}

} // namespace mercury