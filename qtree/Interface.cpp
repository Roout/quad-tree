#include "Interface.h"
#include "QuadTree.h"

#include <iostream>

namespace mercury {

	Description::Description(const sf::IntRect& boundingBox, tree::QuadTree* tree)
		: m_box{ boundingBox }
		, m_tree{ tree }
	{
		this->Init();
	}

	void Description::Update(float dt) {
		// update all info each frame
		m_vertCount.setString("Verticies: " + std::to_string(m_tree->GetSize()));
		Node::Update(dt);
	}

	void Description::Init() {
		// load font
		const char * fontPath = "assets/TimesNewRomans.ttf";
		if (!m_font.loadFromFile(fontPath)) {
			std::cerr << "Can't load " << fontPath << '\n';
			return;
		}

		const sf::Vector2f margin{ 25.f, 25.f };
		const float verticalSpacing{ 10.f };
		const size_t charSize{ 18U };
		// add header
		m_header.setFont(m_font);
		m_header.setFillColor(sf::Color::White);
		m_header.setString("Quad tree controls:");
		m_header.setCharacterSize(charSize);
		m_header.setPosition(margin + sf::Vector2f{ 0.f, static_cast<float>(charSize) });

		// add info about the controls
		m_space.setFont(m_font);
		m_space.setFillColor(sf::Color::White);
		m_space.setString("Insert point: SPACE");
		m_space.setCharacterSize(charSize);
		m_space.setPosition(m_header.getPosition() + sf::Vector2f{ 0.f, verticalSpacing + static_cast<float>(charSize) });
		
		m_flush.setFont(m_font);
		m_flush.setFillColor(sf::Color::White);
		m_flush.setString("Clear whole tree: F");
		m_flush.setCharacterSize(charSize);
		m_flush.setPosition(m_space.getPosition() + sf::Vector2f{ 0.f, verticalSpacing + static_cast<float>(charSize) });

		// add info about number of vertices in the tree
		m_vertCount.setFont(m_font);
		m_vertCount.setFillColor(sf::Color::White);
		m_vertCount.setString("Verticies: " + std::to_string(m_tree->GetSize()));
		m_vertCount.setCharacterSize(charSize);
		m_vertCount.setPosition(m_flush.getPosition() + sf::Vector2f{ 0.f, verticalSpacing + static_cast<float>(charSize) });

	}

	void Description::OnDraw(sf::RenderTarget& target, const sf::RenderStates& states) const {
		target.draw(m_header, states);
		target.draw(m_space, states);
		target.draw(m_flush, states);
		target.draw(m_vertCount, states);
	}

} // namespace mercury 