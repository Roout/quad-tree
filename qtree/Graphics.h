#pragma once
#include "healthy.h"
#include "Node.h"

namespace mercury {

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

		float GetSize() const noexcept {
			return m_size;
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
			this->Init();
		}

		void Init() override {
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