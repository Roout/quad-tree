#include "Node.h"

namespace mercury {

	void Node::Update(float dt) {
		for (auto&& child : m_children) {
			child->Update(dt);
		}
	}

	// \return true if event is consumed otherwise false
	bool Node::ProcessEvent(const sf::Event& event) {
		if (m_listener && m_listener(event)) {
			return true;
		}
		if (m_parent) {
			return m_parent->ProcessEvent(event);
		}
		return false;
	}

	void Node::AddChild(Node *child) {
		m_children.push_back(child);
		child->m_parent = this;
	}

	void Node::RemoveChild(Node *child) {
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it != m_children.end()) {
			(*it)->m_parent = nullptr;
			std::swap(m_children.back(), *it);
			m_children.pop_back();
		}
	}

	void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// apply the entity's transform -- combine it with the one that was passed by the caller
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
		// you may also override states.shader or states.blendMode if you want
		this->OnDraw(target, states);
		// draw the vertex array
		for (auto&& child : m_children) {
			child->draw(target, states);
		}
	}

} // namespace mercury