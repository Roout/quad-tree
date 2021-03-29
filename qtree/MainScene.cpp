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
			// select
			if (event.mouseButton.button == sf::Mouse::Left) {
				this->Lock();
				m_mouse.x = static_cast<float>(event.mouseButton.x);
				m_mouse.y = static_cast<float>(event.mouseButton.y);
			}
		} break;
		case sf::Event::KeyPressed: {
			// insert point
			if (event.key.code == sf::Keyboard::Space) {
				const mt::Pt point{
					static_cast<float>(sf::Mouse::getPosition(*m_window).x),
					static_cast<float>(sf::Mouse::getPosition(*m_window).y)
				};
				std::cerr << "Insert point: " << point.x << " " << point.y << '\n';
				// TODO: should insert only clicks on working space
				m_tree->Insert(point);
			}
			else if (event.key.code == sf::Keyboard::F) {
				std::cerr << "Flush whole tree.\n";
				m_tree->Clear();
			}
		} break;
		case sf::Event::MouseButtonReleased: {
			// deselect
			if (event.mouseButton.button == sf::Mouse::Left) {
				this->Unlock();
				m_mouse.x = m_mouse.y = 0.f;
			}
		} break;
		case sf::Event::MouseMoved: {
			// move node if selected
			if (this->IsLocked()) {
				this->move(event.mouseMove.x - m_mouse.x, event.mouseMove.y - m_mouse.y);
				m_mouse.x = static_cast<float>(event.mouseMove.x);
				m_mouse.y = static_cast<float>(event.mouseMove.y);
			}
		} break;
		case sf::Event::MouseWheelScrolled: {
			// zoom in 
			if (event.mouseWheelScroll.delta > 0) {
				this->scale(1.1f, 1.1f);
			}
			else if (event.mouseWheelScroll.delta < 0) {
				this->scale(0.9f, 0.9f);
			}

			// zoom out
		} break;
		default: break;
		}
		return true;
	}

} // namespace mercury