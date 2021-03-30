#include "QuadTree.h"

#include <queue>
#include <cassert>

namespace tree {

	/**
	 * Get quarter base where the point belongs base on following SFML coordinate system:
	 * (0, 0) ----------- (W, 0)
	 * ...
	 * ...
	 * (0, H) ----------- (W, H)
	 */
	static constexpr Cardinals GetQuarter(const mt::Pt& point, const mt::Rect& box) noexcept {
		Cardinals cardinal = Cardinals::NE;
		if (point.x >= box.GetMidX()) { // EAST
			cardinal = point.y >= box.GetMidY() ? Cardinals::SE : Cardinals::NE;
		}
		else { // WEST
			cardinal = point.y >= box.GetMidY() ? Cardinals::SW : Cardinals::NW;
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
	static constexpr mt::Rect GetRect(Cardinals cardinal, const mt::Rect& box) noexcept {
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

	static bool IsLeaf(const Node *node) noexcept {
		return std::all_of(
			node->m_children.cbegin(), node->m_children.cend(),
			[](const Node* node) {
				return node == nullptr;
			}
		);
	}

	/**
	* Trying to get rid of the child node (leaf) transfering it's data to parent beforehand
	*/
	static void TryMerge(Node *&child, Node *&parent) noexcept {
		assert(parent != child && "Can't merge root");
		assert(IsLeaf(child) && "Trying to merge non-leaf node");

		if (child->m_data.size() + parent->m_data.size() <= Node::MAX_POINTS) {
			parent->m_data.insert(parent->m_data.end(), child->m_data.cbegin(), child->m_data.cend());
			delete child;
			child = nullptr;
		}

	}

	QuadTree::QuadTree(const mt::Rect& fullArea)
		: m_root{ new Node {} }
		, m_size{ 0 }
	{
		m_root->m_box = fullArea;
	}

	QuadTree::~QuadTree() {
		this->Clear();
		delete m_root;
	}

	void QuadTree::Clear() {
		// clean up everything except the root
		m_root->m_data.clear();
		for (auto&& child : m_root->m_children) {
			if (child) {
				this->PostOrderVisit(child, [](Node*& node) {
					delete node;
					node = nullptr;
				});
			}
		}
		m_size = 0;
	}

	void QuadTree::Build(const std::vector<mt::Pt>& points) {
		for (auto& point : points) {
			this->Insert(point);
		}
	}

	// return all of points in the area
	std::vector<mt::Pt> QuadTree::GetPointsAt(const mt::Rect& area) const noexcept {
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
	std::optional<mt::Pt> QuadTree::FindClosest(const mt::Pt& point) const noexcept {
		// TODO: implement
		assert(false && "Not implemented");
		return std::nullopt;
	}

	void QuadTree::Insert(const mt::Pt& point) {
		if (this->Insert(m_root, point)) {
			m_size++;
		}
	}

	bool QuadTree::Find(const mt::Pt & point) const {
		return this->Find(m_root, point);
	}

	void QuadTree::Erase(const mt::Pt& point) {
		this->Erase(m_root, m_root, point);
	}

	void QuadTree::PostOrderVisit(const Visitor_t& func) {
		// apply func each node while traversing tree
		this->PostOrderVisit(m_root, func);
	}

	void QuadTree::PreOrderVisit(const Visitor_t& func) {
		// apply func each node while traversing tree
		this->PreOrderVisit(m_root, func);
	}

	void QuadTree::Erase(Node *& node, Node *& parent, const mt::Pt & point) {
		// point is outside the boundary
		if (!node->m_box.Contains(point)) {
			return;
		}
		// find a needed quarter
		const auto cardinal = GetQuarter(point, node->m_box);
		if (auto& child = node->m_children[cardinal]; child != nullptr) {
			this->Erase(child, node, point);
			// restore properties of the tree
			if (!child && (parent != node) && IsLeaf(node)) {
				// child was removed and now this node is a leaf 
				// so we can try to merge it with parent (maybe points can be transfered to parent node)
				// and this node will be useless too.
				TryMerge(node, parent);
			}
			else if (child && IsLeaf(child)) {
				// target node (from which we remove the point) wasn't leaf before and now it is
				// so we can try to merge it with parent (maybe points can be transfered to parent node)
				// and this node will be useless too.
				TryMerge(child, node);
			}
		}
		else if (auto it = std::find(node->m_data.begin(), node->m_data.end(), point);
			it != node->m_data.end()
		) {
			// remove point from the node
			std::swap(*it, node->m_data.back());
			node->m_data.pop_back();
			m_size--;

			if (auto isLeaf = IsLeaf(node); isLeaf && node != parent) {
				TryMerge(node, parent);
			}
			else if (!isLeaf) {
				// try to find child which is leaf and data from which can extracted to this node
				for (auto & child : node->m_children) {
					if (child && IsLeaf(child)){
						TryMerge(child, node);
					}
				}
			}
		}
	}

	// apply func each node while traversing tree
	void QuadTree::PostOrderVisit(Node*& node, const Visitor_t& func) {
		for (auto& child : node->m_children) {
			if (child != nullptr) {
				this->PostOrderVisit(child, func);
			}
		}
		std::invoke(func, node);
	}

	void QuadTree::PreOrderVisit(Node*& node, const Visitor_t& func) {
		for (auto& child : node->m_children) {
			if (child != nullptr) {
				std::invoke(func, node);
				this->PreOrderVisit(child, func);
			}
		}
	}

	// Find the point in the node
	// return nullptr if it doesn't exist
	Node * QuadTree::Find(Node * node, const mt::Pt& point) const noexcept {
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
	bool QuadTree::Insert(Node * node, const mt::Pt& point) {
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
		else if (auto it = std::find(node->m_data.cbegin(), node->m_data.cend(), point);
				it != node->m_data.cend()
		) { // point already exist in the tree
			return false;
		}
		else if (node->m_data.size() < Node::MAX_POINTS) { // see if the node still can accomodate any point
			node->m_data.push_back(point);
			return true;
		}
		else {
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

} // namesapce tree