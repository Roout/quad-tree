#pragma once
#include "Graphics.h"
#include "QuadTree.h"

#include <memory>
#include <vector>
#include <functional>
#include <algorithm>

namespace mercury {

	/**
	 * TODO:
	 * - [x] add points to the scene
	 * - [x] draw tree each update
	 *		- [x] draw points
	 *		- [x] draw lines which divide area into squares
	 * - [ ] erase points
	 * - [ ] mark poins (by changing size, color or smth) on selection
	 */

	class QuadTreeLayout : public Node {
	public:

		QuadTreeLayout(tree::QuadTree* tree) :
			m_tree{ tree }
		{
			this->Init();
		};

		~QuadTreeLayout() {
			for (auto& child : m_children) {
				delete child;
				child = nullptr;
			}
		}

		void Update(float dt) override {
			// first scrap all drawables from the tree
			this->AddTreeToScene();
			// now update all children (points)
			Node::Update(dt);
		}

		void Init() override {
			const float pointSize = 4.f;
			m_marks = new Marks{ pointSize };
			this->AddChild(m_marks);
		}

		void AddTreeToScene() {
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
				shape.setSize({ node->m_box.size.width, node->m_box.size.height });
				m_rects.push_back(shape);
				
				// add points
				for (const auto& point : node->m_data) {
					m_marks->AddPoint({ point.x, point.y });
				}
			});
		}

		void OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override {
			for (auto&& rect : m_rects) {
				target.draw(rect, states);
			}
		}

	private:
		tree::QuadTree * const m_tree{ nullptr };
		// marks keep points from the tree
		Marks * m_marks{ nullptr };
		std::vector<sf::RectangleShape> m_rects;
	};


} // namespace mercury