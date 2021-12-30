#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

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

		virtual void Update(float dt);

		// \return true if event is consumed otherwise false
		bool ProcessEvent(const sf::Event& event);

		void AddEventListener(std::function<bool(const sf::Event&)> listener);

		void AddChild(Node *child);

		void RemoveChild(Node *child);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	protected:

		virtual void OnDraw([[maybe_unused]] sf::RenderTarget& target
			, [[maybe_unused]] const sf::RenderStates& states) const {};

		bool Contains(const sf::Vector2f& point) {
			return sf::FloatRect(getPosition(), m_size).contains(point);
		}

	protected:

		std::function<bool(const sf::Event&)> m_listener;

		Node *m_parent{ nullptr };
		std::vector<Node*> m_children;
		sf::Vector2f m_size;
	};

	inline void Node::AddEventListener(std::function<bool(const sf::Event&)> listener) {
		m_listener = std::move(listener);
	}

} // namespace mercury