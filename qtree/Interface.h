#pragma once
#include "Node.h"

namespace tree {
	class QuadTree;
}

namespace mercury {

	class Description final : public Node {
	public:

		Description(const sf::IntRect& boundingBox, tree::QuadTree* tree);

		void Update(float dt) override;

		void Init() override;

	private:

		void OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

		sf::IntRect m_box;
		tree::QuadTree* const m_tree{ nullptr };
		sf::Font m_font;
		sf::Text m_header;
		sf::Text m_space;
		sf::Text m_flush;
		sf::Text m_vertCount;
	};

} // namespace mercury