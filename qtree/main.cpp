#include <iostream>
#include <functional>
#include <vector>

#include "healthy.h"
#include "QuadTree.h"
#include <SFML/Graphics.hpp>


//struct EventListener {
//	std::function<void(sf::Event *)> m_onKeyPressed;
//	std::function<void(sf::Event *)> m_onKeyReleased;
//	std::function<void(sf::Event *)> m_onMouseDown;
//	std::function<void(sf::Event *)> m_onMouseUp;
//	std::function<void(sf::Event *)> m_onMouseMove;
//	std::function<void(sf::Event *)> m_onMouseScroll;
//};

namespace mercury {

	class Node : public sf::Drawable, public sf::Transformable {
	public:

		virtual void Init() {}

		virtual void Update(float dt) {
			for (auto&& child : m_children) {
				child->Update(dt);
			}
		}

		// \note for now it doesn't allow event descending
		bool ProcessEvent(const sf::Event& event) {
			if (m_listener) {
				return m_listener(event);
			}
			return false;
		}

		void AddEventListener(std::function<bool(const sf::Event&)> listener) {
			m_listener = listener;
		}

		void AddChild(Node * child) {
			m_children.push_back(child);
			child->m_parent = this;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			// apply the entity's transform -- combine it with the one that was passed by the caller
			states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
			// you may also override states.shader or states.blendMode if you want
			// draw the vertex array
			for (auto&& child : m_children) {
				child->draw(target, states);
			}
		}

	protected:

		std::function<bool(const sf::Event&)> m_listener;

		Node * m_parent{ nullptr };
		std::vector<Node*> m_children;
	};

	class Grid : public Node {
	public:
		Grid(float width
			, float height
			, int columns
			, int rows
		)
			: m_window{ width, height }
			, m_columns{ columns }
			, m_rows{ rows }
		{
			m_vertices.resize(2 * (m_rows + m_columns) + 4);
			m_vertices.setPrimitiveType(sf::Lines);

			const sf::Color color{ 255, 255, 255, 255 };
			const mt::Size tileSize{ m_window.width / m_columns, m_window.height / m_rows };
			mt::Pt shift{ 0.f, 0.f };
			int i = 0;
			for (int row = 0; row <= m_rows; row++, shift.y += tileSize.height, i += 2) {
				m_vertices[i].position = sf::Vector2f(0.f, shift.y);
				m_vertices[i].color = color;
				m_vertices[i + 1].position = sf::Vector2f(m_window.width, shift.y);
				m_vertices[i + 1].color = color;
			}
			for (int col = 0; col <= m_columns; col++, shift.x += tileSize.width, i += 2) {
				m_vertices[i].position = sf::Vector2f(shift.x, 0.f);
				m_vertices[i].color = color;
				m_vertices[i + 1].position = sf::Vector2f(shift.x, m_window.height);
				m_vertices[i + 1].color = color;
			}
		}

		~Grid() = default;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			// apply the entity's transform -- combine it with the one that was passed by the caller
			states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
			// you may also override states.shader or states.blendMode if you want
			// draw the vertex array
			target.draw(m_vertices, states);

			for (auto&& child : m_children) {
				child->draw(target, states);
			}
		}

	private:
		sf::VertexArray m_vertices;

		mt::Size m_window;
		int m_columns{ 0 };
		int m_rows{ 0 };
	};


	class MainScene : public Node {
	public:
		MainScene() {
			this->Init();
		}

		~MainScene() {
			for (auto& child : m_children) {
				delete child;
				child = nullptr;
			}
		}

		void Lock() noexcept {
			m_isLocked = true;
		}

		void Unlock() noexcept {
			m_isLocked = false;
		}

		bool IsLocked() const noexcept {
			return m_isLocked;
		}


		void Init() {
			auto grid = new Grid{ 600.f, 600.f, 20, 20 };
			grid->scale( 0.9f, 0.9f );
			grid->move({ 10.f, 10.f });

			this->AddChild(grid);
			this->AddEventListener([this](const sf::Event& event) {
				switch (event.type) {
				case sf::Event::MouseButtonPressed: {
					// select
					if (event.mouseButton.button == sf::Mouse::Left) {
						this->Lock();
						m_mouse.x = static_cast<float>(event.mouseButton.x);
						m_mouse.y = static_cast<float>(event.mouseButton.y);
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
			});
		}

	private:
		bool m_isLocked{ false };

		sf::Vector2f m_mouse{ 0.f, 0.f };
	
	};


}
int main() {
	sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");	

	mercury::MainScene scene;
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
		// sf::Vector2i mouse = sf::Mouse::getPosition(window);
		window.clear();
		window.draw(scene);
		window.display();
	}

	return 0;
}