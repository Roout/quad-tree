#pragma once
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

		// \return true if event is consumed otherwise false
		bool ProcessEvent(const sf::Event& event) {
			if (m_listener && m_listener(event)) {
				return true;
			}
			if (m_parent) {
				return m_parent->ProcessEvent(event);
			}
			return false;
		}

		void AddEventListener(std::function<bool(const sf::Event&)> listener) {
			m_listener = std::move(listener);
		}

		void AddChild(Node *child) {
			m_children.push_back(child);
			child->m_parent = this;
		}

		void RemoveChild(Node *child) {
			auto it = std::find(m_children.begin(), m_children.end(), child);
			if (it != m_children.end()) {
				(*it)->m_parent = nullptr;
				std::swap(m_children.back(), *it);
				m_children.pop_back();
			}
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			// apply the entity's transform -- combine it with the one that was passed by the caller
			states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
			// you may also override states.shader or states.blendMode if you want
			this->OnDraw(target, states);
			// draw the vertex array
			for (auto&& child : m_children) {
				child->draw(target, states);
			}
		}

	protected:

		virtual void OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const {};
	
	protected:

		std::function<bool(const sf::Event&)> m_listener;

		Node *m_parent{ nullptr };
		std::vector<Node*> m_children;
	};

	class Cross : public Node {
	public:
		Cross(const mt::Pt& center, float length)
			: m_center{ center }
			, m_length{ length }
		{
			this->Init();
		}

		void Init() override {
			m_lines.setPrimitiveType(sf::PrimitiveType::Lines);
			m_color = sf::Color(0, 0, 255, 255);

			const auto half = m_length / 2.f;
			sf::Vector2f corners[] = {
				{ m_center.x - half, m_center.y },
				{ m_center.x + half, m_center.y },
				{ m_center.x, m_center.y + half },
				{ m_center.x, m_center.y - half },
			};
			for (const auto& corner : corners) {
				m_lines.append(sf::Vertex{ corner, m_color });
			}
		}

		void OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override {
			target.draw(m_lines, states);
		}

	private:
		mt::Pt m_center;
		float m_length{ 0.f };

		sf::VertexArray	m_lines;
		sf::Color m_color;
	};

	class Marks : public Node {
	public:
		Marks(float size = 10.f) :
			m_size{ size }
		{
			this->Init();
		}

		void Init() override {
			m_marks.setPrimitiveType(sf::PrimitiveType::Quads);
			m_color = sf::Color(255, 0, 0, 255);
		}

		void OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override {
			target.draw(m_marks, states);
		}

		void AddPoint(const sf::Vector2f& point) {
			const auto half = m_size / 2.f;
			sf::Vector2f corners[4] = {
				{ point.x - half, point.y - half },
				{ point.x - half, point.y + half },
				{ point.x + half, point.y + half },
				{ point.x + half, point.y - half },
			};
			for (const auto& corner : corners) {
				m_marks.append(sf::Vertex{ corner, m_color });
			}
		}

		void Clear() {
			m_marks.clear();
		}

	private:
		sf::VertexArray	m_marks;
		float			m_size;
		sf::Color		m_color;
	};

	class Grid : public Node {
	public:
		Grid (float width
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

		void OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override {
			target.draw(m_vertices, states);
		}

	private:
		sf::VertexArray m_vertices;

		mt::Size m_window;
		int m_columns{ 0 };
		int m_rows{ 0 };
	};

}