#pragma once

#include "healthy.hpp"
#include <array>
#include <vector>

constexpr mt::Pt INVALID { -100000000.f, -100000000.f }; 

enum Cardinals { NW = 0, NE, SW, SE, COUNT };

struct Node {
    static constexpr size_t MAX_POINTS { 2 };

    std::array<Node*, Cardinals::COUNT> m_children { nullptr };
    std::vector<mt::Pt> m_data ;     
    mt::Rect m_box { {0.f, 0.f}, {0.f, 0.f} };     
};

class QuadTree {
public:
    QuadTree(const mt::Rect& fullArea) 
        : m_root { new Node {} }
        , m_size { 1 }
    {
        m_root->m_box = fullArea;
    }

    void Build(const std::vector<mt::Pt>& points) {
        for(auto& point: points) {
            this->Insert(point);
        }
    }

    // return number of points in the area
    size_t Query(const mt::Rect& area) const noexcept {
        return 0;
    }

    mt::Pt FindClosest(const mt::Pt& point) const noexcept {
        return {0, 0};
    }

    void Insert(const mt::Pt& point) {
        (void) this->Insert(m_root, point);
    }

    void Delete(const mt::Pt& point) {

    }

private:

    Node * Find(const mt::Pt& point) const noexcept {
        return nullptr;
    }

    bool Insert(Node * node, const mt::Pt& point) {
        // TODO: maybe remove this check?
        // point is outside the boundary
        if(!node->m_box.Contains(point)) {
            return false;
        }

        // see if the node still can accomodate any point
        if(node->m_data.size() < Node::MAX_POINTS) {
            node->m_data.push_back(point);
            return true;
        }

        // need to divide node so see what quater will be used
        const auto& box { node->m_box };
        const mt::Size size { box.size.width / 2.f, box.size.height / 2.f };
        // find a needed quarter
        Cardinals cardinal;
        if(point.x > box.GetMidX()) { // EAST
            cardinal = point.y > box.GetMidY()? Cardinals::NE: Cardinals::SE;
        }
        else { // WEST
            cardinal = point.y > box.GetMidY()? Cardinals::NW: Cardinals::SW;
        }

        if(auto& child = node->m_children[cardinal]; child != nullptr) {
            return this->Insert(child, point);
        }
        else {
            child = new Node{};
            child->m_box.origin = box.GetMid();
            child->m_box.size = size;
            return true;
        }
    }

private:
    Node * m_root { nullptr };
    size_t m_size { 0 };
};