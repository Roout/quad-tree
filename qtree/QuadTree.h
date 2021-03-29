#pragma once

#include "healthy.h"
#include <array>
#include <vector>
#include <functional>
#include <optional>
#include <cassert>

namespace tree {

	/**
	 * NE - corresponds to top-right quarter
	 * SE - corresponds to bottom-right quarter
	 * NW - corresponds to top-left quarter
	 * SW - corresponds to bottom-left quarter
	 */
	enum Cardinals { NW = 0, NE, SW, SE, COUNT };

	/**
	 * Get quarter base where the point belongs base on following SFML coordinate system:
	 * (0, 0) ----------- (W, 0)
	 * ...
	 * ...
 	 * (0, H) ----------- (W, H)
	 */
	constexpr Cardinals GetQuarter(const mt::Pt& point, const mt::Rect& box) noexcept {
		Cardinals cardinal = Cardinals::NE;
		if (point.x > box.GetMidX()) { // EAST
			cardinal = point.y > box.GetMidY() ? Cardinals::SE : Cardinals::NE;
		}
		else { // WEST
			cardinal = point.y > box.GetMidY() ? Cardinals::SW : Cardinals::NW;
		}
		return cardinal;
	}

	/**
	 * Form rectangle from quarter on following SFML coordinate system:
	 * (0, 0) ----------- (W, 0)
	 * ...
	 * ...
	 * (0, H) ----------- (W, H)
	 */
	constexpr mt::Rect GetRect(Cardinals cardinal, const mt::Rect& box) noexcept {
		switch (cardinal) {
		case Cardinals::NE:
			return { box.GetMidX(), box.GetMinY(), box.size.width / 2.f, box.size.height / 2.f };
		case Cardinals::SE:
			return { box.GetMidX(), box.GetMidY(), box.size.width / 2.f, box.size.height / 2.f };
		case Cardinals::NW:
			return { box.GetMinX(), box.GetMinY(), box.size.width / 2.f, box.size.height / 2.f };
		case Cardinals::SW:
			return { box.GetMinX(), box.GetMidY(), box.size.width / 2.f, box.size.height / 2.f };
		default: assert(false && "Can't fallthrough here!");  break;
		}

		return box;
	}

	struct Node {
		static constexpr size_t MAX_POINTS{ 2 };

		std::array<Node*, Cardinals::COUNT> m_children{ nullptr };
		// TODO: rewrite to std::array
		std::vector<mt::Pt> m_data;
		mt::Rect m_box{ {0.f, 0.f}, {0.f, 0.f} };
	};

	class QuadTree {
	public:
		QuadTree(const mt::Rect& fullArea);

		~QuadTree();

		void Build(const std::vector<mt::Pt>& points);

		// return all of points in the area
		std::vector<mt::Pt> GetPointsAt(const mt::Rect& area) const noexcept;

		// return the closest neighbour point or nullopt if no points present
		std::optional<mt::Pt> FindClosest(const mt::Pt& point) const noexcept;

		void Insert(const mt::Pt& point);

		void Erase(const mt::Pt& point);

		void PostOrderVisit(const std::function<void(Node*)>& func);
		
		void PreOrderVisit(const std::function<void(Node*)>& func);

		bool IsEmpty() const noexcept;

		// retrun number of points in the tree
		size_t GetSize() const noexcept;

	private:

		// apply func each node while traversing tree
		void PostOrderVisit(Node* node, const std::function<void(Node*)>& func);

		void PreOrderVisit(Node* node, const std::function<void(Node*)>& func);

		// Find the point in the node
		// return nullptr if it doesn't exist
		Node * Find(Node * node, const mt::Pt& point) const noexcept;

		// Insert `point` into the `node`
		bool Insert(Node * node, const mt::Pt& point);

	private:
		Node * m_root{ nullptr };
		size_t m_size{ 0 };
	};


	inline bool QuadTree::IsEmpty() const noexcept {
		return m_size == 0;
	}

	inline size_t QuadTree::GetSize() const noexcept {
		return m_size;
	}

} // namespace tree