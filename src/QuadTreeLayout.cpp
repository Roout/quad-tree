#include "QuadTreeLayout.h"
#include "QuadTree.h"

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
		// remove all existing marks
		m_marks->Clear();
		// remove all existing quads
		m_rects.clear();
		// first scrap all drawables from the tree
		this->AddTree();
		this->AddSelectPoints();
		// now update all children (points)
		Node::Update(dt);
	}

	void QuadTreeLayout::Init() {
		const float pointSize = 4.f;
		m_marks = new Marks{ pointSize };
		this->AddChild(m_marks);

		m_selected.setFillColor({255, 0, 0, 80});

		auto listener = [this](const sf::Event& event) {
			switch (event.type) {
				case sf::Event::MouseButtonPressed: {
					// select region
					if (event.mouseButton.button == sf::Mouse::Left) {
						m_mouse.emplace(
							static_cast<float>(event.mouseButton.x), 
							static_cast<float>(event.mouseButton.y)
						);
						m_selected.setPosition({ m_mouse->x, m_mouse->y });
						m_selected.setSize({ 1.f, 1.f });
					}
				} break;
				case sf::Event::MouseMoved: {
					if (m_mouse) {
						const mt::Pt mouse{
							static_cast<float>(event.mouseMove.x),
							static_cast<float>(event.mouseMove.y)
						};
						const mt::Rect selected{
							std::min(mouse.x, m_mouse->x),
							std::min(mouse.y, m_mouse->y),
							std::max(mouse.x, m_mouse->x) - std::min(mouse.x, m_mouse->x),
							std::max(mouse.y, m_mouse->y) - std::min(mouse.y, m_mouse->y)
						};
						m_selected.setPosition(selected.GetMinX(), selected.GetMinY());
						m_selected.setSize({ selected.size.width, selected.size.height });
						// update points
						m_selectedPoints = m_tree->GetPointsAt(selected);
					}
				} break;
				case sf::Event::MouseButtonReleased: {
					// deselect region
					if (event.mouseButton.button == sf::Mouse::Left) {
						const mt::Pt mouse{
							static_cast<float>(event.mouseButton.x),
							static_cast<float>(event.mouseButton.y)
						};
						const mt::Rect selected {
							std::min(mouse.x, m_mouse->x),
							std::min(mouse.y, m_mouse->y),
							std::max(mouse.x, m_mouse->x) - std::min(mouse.x, m_mouse->x),
							std::max(mouse.y, m_mouse->y) - std::min(mouse.y, m_mouse->y)
						};
						// update points
						m_selectedPoints = m_tree->GetPointsAt(selected);
						// clean up
						m_mouse.reset();
					}
				} break;
				case sf::Event::KeyPressed: {
					// insert point
					if (event.key.code == sf::Keyboard::F) {
						for (auto&point : m_selectedPoints) {
							m_tree->Erase(point);
						}
						m_selectedPoints.clear();
					}
				} break;
				default: break;
			}
			return false;
		};
		this->AddEventListener(std::move(listener));
	}

	void QuadTreeLayout::AddTree() {
		m_tree->PostOrderVisit([this](tree::Node::pointer& node) {
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

	void QuadTreeLayout::AddSelectPoints() {
		const sf::Color color{ 0, 255, 0, 255 };
		const float pointSize{ m_marks->GetSize() + 2.f };

		for (const auto& point : m_selectedPoints) {
			sf::Vector2f position{
				point.x - pointSize / 2.f,
				point.y - pointSize / 2.f
			};
			sf::RectangleShape shape;
			shape.setPosition(position);
			shape.setSize({ pointSize, pointSize });
			shape.setFillColor(color);
			m_rects.push_back(shape);
		}
	}

	void QuadTreeLayout::OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const {
		for (auto&& rect : m_rects) {
			target.draw(rect, states);
		}
		if (m_mouse) {
			target.draw(m_selected, states);
		}
	}

} // namespace mercury