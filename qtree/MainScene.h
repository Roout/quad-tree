#pragma once
#include "Node.h"

#include <functional>
#include <memory>

namespace tree {
	class QuadTree;
}

namespace mercury {

	class QuadTreeLayout;

	class MainScene : public Node {
	public:

		MainScene(sf::RenderWindow * window);

		~MainScene();

		void Init();

	private:

		bool EventListener(const sf::Event& event);

		void Lock() noexcept;

		void Unlock() noexcept;

		bool IsLocked() const noexcept;

	private:
		sf::RenderWindow * const m_window{ nullptr };
		bool m_isLocked{ false };
		sf::Vector2f m_mouse{ 0.f, 0.f };

		std::unique_ptr<tree::QuadTree> m_tree{ nullptr };
		QuadTreeLayout *m_treeLayout{ nullptr };
	};

	inline void MainScene::Lock() noexcept {
		m_isLocked = true;
	}

	inline void MainScene::Unlock() noexcept {
		m_isLocked = false;
	}

	inline bool MainScene::IsLocked() const noexcept {
		return m_isLocked;
	}

}