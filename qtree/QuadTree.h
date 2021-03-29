#pragma once

#include "healthy.h"
#include <array>
#include <vector>
#include <functional>
#include <cassert>
#include <queue>
#include <optional>

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
		QuadTree(const mt::Rect& fullArea)
			: m_root{ new Node {} }
			, m_size{ 0 }
		{
			m_root->m_box = fullArea;
		}

		~QuadTree() {
			this->PostOrderVisit(m_root, [](Node* node) {
				delete node;
			});
		}

		void Build(const std::vector<mt::Pt>& points) {
			for (auto& point : points) {
				this->Insert(point);
			}
		}

		// return all of points in the area
		std::vector<mt::Pt> GetPointsAt(const mt::Rect& area) const noexcept {
			std::vector<mt::Pt> points;

			std::queue<Node*> processed;
			processed.push(m_root);

			while (!processed.empty()) {
				auto current = processed.front();
				processed.pop();

				for (const auto& point : current->m_data) {
					if (area.Contains(point)) {
						points.push_back(point);
					}
				}

				for (auto quarter : current->m_children) {
					if (quarter && quarter->m_box.Intersect(area)) {
						processed.push(quarter);
					}
				}
			}

			return points;
		}

		// return the closest neighbour point or nullopt if no points present
		std::optional<mt::Pt> FindClosest(const mt::Pt& point) const noexcept {
			// TODO: implement
			assert(false && "Not implemented");
			return std::nullopt;
		}

		void Insert(const mt::Pt& point) {
			if (this->Insert(m_root, point)) {
				m_size++;
			}
		}

		void Erase(const mt::Pt& point) {
			auto node = this->Find(m_root, point);
			if (node) {
				auto it = std::find(node->m_data.begin(), node->m_data.end(), point);
				assert(it != node->m_data.end() && "Tree::Find method failed!");
				std::swap(*it, node->m_data.back());
				node->m_data.pop_back();
				m_size--;
			}
		}

		void PostOrderVisit(const std::function<void(Node*)>& func) {
			// apply func each node while traversing tree
			this->PostOrderVisit(m_root, func);
		}
		
		void PreOrderVisit(const std::function<void(Node*)>& func) {
			// apply func each node while traversing tree
			this->PreOrderVisit(m_root, func);
		}

		bool IsEmpty() const noexcept {
			return m_size == 0;
		}

		// retrun number of points in the tree
		size_t GetSize() const noexcept {
			return m_size;
		}

	private:

		// apply func each node while traversing tree
		void PostOrderVisit(Node* node, const std::function<void(Node*)>& func) {
			for (size_t i = 0; i < Cardinals::COUNT; i++) {
				if (node->m_children[i]) {
					this->PostOrderVisit(node->m_children[i], func);
				}
			}
			std::invoke(func, node);
		}

		void PreOrderVisit(Node* node, const std::function<void(Node*)>& func) {
			for (size_t i = 0; i < Cardinals::COUNT; i++) {
				if (node->m_children[i]) {
					std::invoke(func, node);
					this->PreOrderVisit(node->m_children[i], func);
				}
			}
		}

		// Find the point in the node
		// return nullptr if it doesn't exist
		Node * Find(Node * node, const mt::Pt& point) const noexcept {
			// point is outside the boundary
			if (!node->m_box.Contains(point)) {
				return nullptr;
			}
			
			// find a needed quarter
			const auto cardinal = GetQuarter(point, node->m_box);
			if (auto child = node->m_children[cardinal]; child != nullptr) {
				return this->Find(child, point);
			}
			else if (auto it = std::find(node->m_data.cbegin(), node->m_data.cend(), point);
				it != node->m_data.cend()
			) {
				// point contains in this node
				return node;
			}
			return nullptr;
		}

		// Insert `point` into the `node`
		bool Insert(Node * node, const mt::Pt& point) {
			// TODO: maybe remove this check?
			// point is outside the boundary
			if (!node->m_box.Contains(point)) {
				return false;
			}

			const auto cardinal = GetQuarter(point, node->m_box);
			
			// find a needed quarter
			if (auto& child = node->m_children[cardinal]; child != nullptr) {
				return this->Insert(child, point);
			}
			else if (node->m_data.size() < Node::MAX_POINTS) {
				// see if the node still can accomodate any point
				node->m_data.push_back(point);
				return true;
			}
			else {
				// TODO: check whether a point already exist or not in the tree
				{

				}
				
				child = new Node{};
				child->m_box = GetRect(cardinal, node->m_box);

				// move points which have same quarter to this child node
				size_t removed{ 0 };
				const size_t size = node->m_data.size();
				for (size_t i = 0; i + removed < size; ) {
					if (child->m_box.Contains(node->m_data[i])) {
						// copy to child quarter
						child->m_data.push_back(node->m_data[i]);
						// move to end of the vector points scheduled for removal
						std::swap(node->m_data[i], node->m_data[size - removed - 1]);
						// increase number of points to be removed
						removed++;
					}
					else {
						i++;
					}
				}
				node->m_data.erase(node->m_data.cend() - removed, node->m_data.cend());
				return this->Insert(child, point);
			}
		}

	private:
		Node * m_root{ nullptr };
		size_t m_size{ 0 };
	};

} // namespace tree