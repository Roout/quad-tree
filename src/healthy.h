#ifndef HEALTHY_HPP__
#define HEALTHY_HPP__

#include <cmath>
#include <chrono>

namespace mt {

	struct Pt {
		float x, y;

		constexpr Pt(float x_ = -1, float y_ = -1) : x(x_), y(y_) {}

		constexpr Pt Negate() const noexcept {
			return { -x, -y };
		}

		constexpr float SquareLength() const noexcept {
			return x * x + y * y;
		}

		constexpr float ManhDistance(const Pt& pt) const noexcept {
			return (pt.x > x ? (pt.x - x) : (x - pt.x)) + (pt.y > y ? (pt.y - y) : (y - pt.y));
		}

		float Length() const noexcept {
			return sqrtf(this->SquareLength());
		}

		constexpr float Dot(const Pt& p) const noexcept {
			return x * p.x + y * p.y;
		}

		constexpr float operator*(const Pt& rhs) const noexcept {
			return { this->Dot(rhs) };
		}

		constexpr Pt operator+(const Pt& rhs) const noexcept {
			return { x + rhs.x, y + rhs.y };
		}

		constexpr Pt operator-(const Pt& rhs) const noexcept {
			return { x - rhs.x, y - rhs.y };
		}

		constexpr Pt operator+(float value) const noexcept {
			return { x + value, y + value };
		}

		constexpr Pt operator-(float value) const noexcept {
			return { x - value, y - value };
		}

		constexpr Pt operator/(float value) const noexcept {
			return { x / value, y / value };
		}

		constexpr Pt operator*(float value) const noexcept {
			return { x * value, y * value };
		}

	};

	constexpr bool operator==(const Pt& lsh, const Pt& rsh) noexcept {
		return lsh.x == rsh.x && lsh.y == rsh.y;
	}

	constexpr bool operator!=(const Pt& lsh, const Pt& rsh) noexcept {
		return lsh.x != rsh.x || lsh.y != rsh.y;
	}

	struct Size {
		float width;
		float height;

		constexpr Pt asPt() const noexcept {
			return { width, height };
		}
	};

	struct Rect {
		Pt origin;
		Size size;

		constexpr Rect(float x, float y, float w, float h)
			: origin{ x, y }
			, size{ w, h }
		{}

		constexpr Rect(const Pt& pt, const Size& sz)
			: origin{ pt }
			, size{ sz }
		{}

		constexpr float GetMinX() const noexcept {
			return origin.x;
		}

		constexpr float GetMinY() const noexcept {
			return origin.y;
		}

		constexpr float GetMidX() const noexcept {
			return origin.x + size.width / 2.f;
		}

		constexpr float GetMidY() const noexcept {
			return origin.y + size.height / 2.f;
		}

		constexpr float GetMaxX() const noexcept {
			return origin.x + size.width;
		}

		constexpr float GetMaxY() const noexcept {
			return origin.y + size.height;
		}

		constexpr Pt GetMid() const noexcept {
			return { GetMidX(), GetMidY() };
		}

		constexpr bool Contains(const Pt& pt) const noexcept {
			return (pt.x >= origin.x
				&& pt.x < GetMaxX()
				&& pt.y >= origin.y
				&& pt.y < GetMaxY()
			);
		}

		constexpr bool Contains(float x, float y) const noexcept {
			return this->Contains({ x, y });
		}

		constexpr bool Intersect(const Rect& box) const noexcept {
			// If one rectangle is on left side of other 
			if (origin.x > box.GetMaxX() || box.origin.x > GetMaxX())
				return false;

			// If one rectangle is above other 
			if (origin.y > box.GetMaxY() || box.origin.y > GetMaxY())
				return false;

			return true;
		}

	};

	namespace Asserts {

		static_assert(Pt{ 1.f, 10.f }.ManhDistance(Pt{ 11.f, -10.f }) == 30.f, "Manheten distance failed");
		static_assert(Pt{ 0.f, 0.f }.ManhDistance(Pt{ -1.f, -5.f }) == 6.f, "Manheten distance failed");

		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Contains(5.f, 5.f) == true, "Contains failed a check!");
		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Contains(-1.f, 5.f) == false, "Contains failed a check!");
		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Contains(5.f, 15.f) == false, "Contains failed a check!");

		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Intersect(Rect{ 5.f, 11.f, 2.f, 2.f }) == false, "Intersect failed a check!");
		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Intersect(Rect{ 11.f, 5.f, 2.f, 2.f }) == false, "Intersect failed a check!");
		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Intersect(Rect{ 5.f, 10.f, 2.f, 2.f }) == true, "Intersect failed a check!");
		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Intersect(Rect{ 5.f, 5.f, 2.f,  2.f }) == true, "Intersect failed a check!");
		static_assert(Rect{ 0.f, 0.f, 10.f, 10.f }.Intersect(Rect{ 5.f, 5.f, 20.f, 20.f }) == true, "Intersect failed a check!");
	}

}

#endif // HEALTHY_HPP__