#pragma once

#include "healthy.h"
#include <array>
#include <vector>
#include <functional>
#include <cassert>
#include <queue>
#include <optional>

namespace tree {

	enum Cardinals { NW = 0, NE, SW, SE, COUNT };

	struct Node {
		static constexpr size_t MAX_POINTS{ 2 };

		std::array<Node*, Cardinals::COUNT> m_children{ nullptr };
		std::vector<mt::Pt> m_data;
		mt::Rect m_box{ {0.f, 0.f}, {0.f, 0.f} };
	};

	class QuadTree {
	public:
		QuadTree(const mt::Rect& fullArea)
			: m_root{ new Node {} }
			, m_size{ 1 }
		{
			m_root->m_box = fullArea;
		}

		~QuadTree() {
			this->Visit(m_root, [](Node* node) {
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

		void Visit(const std::function<void(Node*)>& func) {
			// apply func each node while traversing tree
			this->Visit(m_root, func);
		}

		bool IsEmpty() const noexcept {
			return m_size == 0;
		}

		size_t GetSize() const noexcept {
			return m_size;
		}

	private:

		// apply func each node while traversing tree
		void Visit(Node* node, const std::function<void(Node*)>& func) {
			for (size_t i = 0; i < Cardinals::COUNT; i++) {
				if (node->m_children[i]) {
					this->Visit(node->m_children[i], func);
				}
			}
			std::invoke(func, node);
		}

		// Find the point in the node
		// return nullptr if it doesn't exist
		Node * Find(Node * node, const mt::Pt& point) const noexcept {
			// point is outside the boundary
			if (!node->m_box.Contains(point)) {
				return nullptr;
			}
			// check whether it contains in this node instead of children
			if (auto it = std::find(node->m_data.cbegin(), node->m_data.cend(), point);
				it != node->m_data.cend()
				) {
				return node;
			}
			// find a needed quarter
			const auto cardinal = this->GetQuarter(point, node->m_box);
			if (auto child = node->m_children[cardinal]; child != nullptr) {
				return this->Find(child, point);
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

			// see if the node still can accomodate any point
			if (node->m_data.size() < Node::MAX_POINTS) {
				node->m_data.push_back(point);
				return true;
			}

			// find a needed quarter
			const auto cardinal = this->GetQuarter(point, node->m_box);
			if (auto& child = node->m_children[cardinal]; child != nullptr) {
				return this->Insert(child, point);
			}
			else {
				child = new Node{};
				child->m_box.origin = node->m_box.GetMid();
				child->m_box.size = { node->m_box.size.width / 2.f,  node->m_box.size.height / 2.f };
				return true;
			}
		}

		Cardinals GetQuarter(const mt::Pt& point, const mt::Rect& box) const noexcept {
			Cardinals cardinal;
			if (point.x > box.GetMidX()) { // EAST
				cardinal = point.y > box.GetMidY() ? Cardinals::NE : Cardinals::SE;
			}
			else { // WEST
				cardinal = point.y > box.GetMidY() ? Cardinals::NW : Cardinals::SW;
			}
			return cardinal;
		}

	private:
		Node * m_root{ nullptr };
		size_t m_size{ 0 };
	};

} // namespace tree