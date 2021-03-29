#include "QuadTreeLayout.h"
#include "QuadTree.h"

#include <iostream>

namespace mercury {

	QuadTreeLayout::QuadTreeLayout(tree::QuadTree* tree) 
		: m_tree{ tree }
	{
		this->Init();
	};

	QuadTreeLayout::~QuadTreeLayout() {
		for (auto& child : m_children) {
			delete child;
			child = nullptr;
		}
	}

	void QuadTreeLayout::Update(float dt) {
		// first scrap all drawables from the tree
		this->AddTreeToScene();
		// now update all children (points)
		Node::Update(dt);
	}

	void QuadTreeLayout::Init() {
		const float pointSize = 4.f;
		m_marks = new Marks{ pointSize };
		this->AddChild(m_marks);

		auto listener = [](const sf::Event& event) {
			switch (event.type) {
				case sf::Event::MouseButtonPressed: {
					// select
					if (event.mouseButton.button == sf::Mouse::Left) {
						const sf::Vector2f mouse{
							static_cast<float>(event.mouseButton.x),
							static_cast<float>(event.mouseButton.y)
						};
						std::cerr << "Pressed: " << mouse.x << " " << mouse.y << '\n';
					}
				} break;
				case sf::Event::MouseButtonReleased: {
					// deselect
					if (event.mouseButton.button == sf::Mouse::Left) {
						const sf::Vector2f mouse{
							static_cast<float>(event.mouseButton.x),
							static_cast<float>(event.mouseButton.y)
						};
						std::cerr << "Released: " << mouse.x << " " << mouse.y << '\n';
					}
				} break;
				default: break;
			}
			return false;
		};
		this->AddEventListener(std::move(listener));
	}

	void QuadTreeLayout::AddTreeToScene() {
		// remove all existing marks
		m_marks->Clear();
		// remove all existing quads
		m_rects.clear();

		m_tree->PostOrderVisit([this](tree::Node* node) {
			// add quad shape:
			sf::RectangleShape shape;
			shape.setFillColor(sf::Color::Transparent);
			shape.setOutlineColor(sf::Color::Blue);
			shape.setOutlineThickness(2.f);
			sf::Vector2f vec2{ node->m_box.origin.x, node->m_box.origin.y };
			shape.setPosition(vec2);
			shape.setSize({ node->m_box.size.width - 1.f, node->m_box.size.height - 1.f });
			m_rects.push_back(shape);

			// add points
			for (const auto& point : node->m_data) {
				m_marks->AddPoint({ point.x, point.y });
			}
		});
	}

	void QuadTreeLayout::OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const {
		for (auto&& rect : m_rects) {
			target.draw(rect, states);
		}
	}

} // namespace mercury