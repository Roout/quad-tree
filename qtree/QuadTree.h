#pragma once

#include "healthy.h"
#include <array>
#include <vector>
#include <functional>
#include <optional>

namespace tree {

	/**
	 * NE - corresponds to top-right quarter
	 * SE - corresponds to bottom-right quarter
	 * NW - corresponds to top-left quarter
	 * SW - corresponds to bottom-left quarter
	 */
	enum Cardinals { NW = 0, NE, SW, SE, COUNT };

	struct Node {
		static constexpr size_t MAX_POINTS{ 2 };

		std::array<Node*, Cardinals::COUNT> m_children{ nullptr };
		// TODO: rewrite to std::array
		std::vector<mt::Pt> m_data;
		mt::Rect m_box{ {0.f, 0.f}, {0.f, 0.f} };
	};

	/**
	 * @note this tree won't create a node for the forth quarter 
	 * until number of points there won't be greater than Node::MAX_POINTS
	 */
	class QuadTree {
	public:

		using Visitor_t = std::function<void(Node*&)>;

		QuadTree(const mt::Rect& fullArea);

		~QuadTree();

		void Build(const std::vector<mt::Pt>& points);

		// return all of points in the area
		std::vector<mt::Pt> GetPointsAt(const mt::Rect& area) const noexcept;

		// return the closest neighbour point or nullopt if no points present
		std::optional<mt::Pt> FindClosest(const mt::Pt& point) const noexcept;

		void Insert(const mt::Pt& point);

		bool Find(const mt::Pt& point) const;

		/**
		* Erase point from the tree.
		* On successfull erasure trying to merge child nodes with parent node if possible
		*/
		void Erase(const mt::Pt& point);

		void PostOrderVisit(const Visitor_t& func);
		
		void PreOrderVisit(const Visitor_t& func);

		bool IsEmpty() const noexcept;

		// retrun number of points in the tree
		size_t GetSize() const noexcept;

		void Clear();

	private:

		void Erase(Node*& node, Node*& parent, const mt::Pt& point);

		// apply func each node while traversing tree
		void PostOrderVisit(Node*& node, const Visitor_t& func);

		void PreOrderVisit(Node*& node, const Visitor_t& func);

		// Find the point in the node
		// return nullptr if it doesn't exist
		Node * Find(Node * node, const mt::Pt& point) const noexcept;

		// Insert `point` into the `node`
		bool Insert(Node * node, const mt::Pt& point);

	private:
		Node * m_root{ nullptr };
		// number of vertices in the tree
		size_t m_size{ 0 };
	};


	inline bool QuadTree::IsEmpty() const noexcept {
		return m_size == 0;
	}

	inline size_t QuadTree::GetSize() const noexcept {
		return m_size;
	}

} // namespace tree