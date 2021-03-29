#pragma once
#include "Graphics.h"
#include <vector>

namespace tree {
	class QuadTree;
}

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

		QuadTreeLayout(tree::QuadTree* tree);

		~QuadTreeLayout();

		void Update(float dt) override;

		void Init() override;

	private:
		
		void AddTreeToScene();

		void OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
	
		tree::QuadTree * const m_tree{ nullptr };
		// marks keep points from the tree
		Marks * m_marks{ nullptr };
		std::vector<sf::RectangleShape> m_rects;
	};


} // namespace mercury