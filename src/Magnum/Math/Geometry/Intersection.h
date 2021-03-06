#ifndef Magnum_Math_Geometry_Intersection_h
#define Magnum_Math_Geometry_Intersection_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Class @ref Magnum::Math::Geometry::Intersection
 */

#include "Magnum/Math/Vector3.h"

namespace Magnum { namespace Math { namespace Geometry {

/** @brief Functions for computing intersections */
class Intersection {
    public:
        Intersection() = delete;

        /**
         * @brief Intersection of two line segments in 2D
         * @param p             Starting point of first line segment
         * @param r             Direction of first line segment
         * @param q             Starting point of second line segment
         * @param s             Direction of second line segment
         * @return Intersection point positions `t`, `u` on both lines, NaN if
         *      the lines are collinear or infinity if they are parallel.
         *      Intersection point can be then computed with `p + t*r` or
         *      `q + u*s`. If `t` is in range @f$ [ 0 ; 1 ] @f$, the
         *      intersection is inside the line segment defined by `p` and
         *      `p + r`, if `u` is in range @f$ [ 0 ; 1 ] @f$, the intersection
         *      is inside the line segment defined by `q` and `q + s`.
         *
         * The two lines intersect if **t** and **u** exist such that: @f[
         *      \boldsymbol p + t \boldsymbol r = \boldsymbol q + u \boldsymbol s
         * @f]
         * Crossing both sides with **s**, distributing the cross product and
         * eliminating @f$ \boldsymbol s \times \boldsymbol s = 0 @f$, then
         * solving for **t** and similarly for **u**: @f[
         *      \begin{array}{rcl}
         *          (\boldsymbol p + t \boldsymbol r) \times s & = & (\boldsymbol q + u \boldsymbol s) \times s \\
         *          t (\boldsymbol r \times s) & = & (\boldsymbol q - \boldsymbol p) \times s \\
         *          t & = & \cfrac{(\boldsymbol q - \boldsymbol p) \times s}{\boldsymbol r \times \boldsymbol s} \\
         *          u & = & \cfrac{(\boldsymbol q - \boldsymbol p) \times r}{\boldsymbol r \times \boldsymbol s}
         *      \end{array}
         * @f]
         *
         * See also @ref lineSegmentLine() which computes only **t**, which is
         * useful if you don't need to test that the intersection lies inside
         * line segment defined by `q` and `q + s`.
         */
        template<class T> static std::pair<T, T> lineSegmentLineSegment(const Vector2<T>& p, const Vector2<T>& r, const Vector2<T>& q, const Vector2<T>& s) {
            const Vector2<T> qp = q - p;
            const T rs = cross(r, s);
            return {cross(qp, s)/rs, cross(qp, r)/rs};
        }

        /**
         * @brief Intersection of line segment and line in 2D
         * @param p             Starting point of first line segment
         * @param r             Direction of first line segment
         * @param q             Starting point of second line
         * @param s             Direction of second line
         * @return Intersection point position `t` on first line, NaN if the
         *      lines are collinear or infinity if they are parallel.
         *      Intersection point can be then with `p + t*r`. If returned
         *      value is in range @f$ [ 0 ; 1 ] @f$, the intersection is inside
         *      the line segment defined by `p` and `p + r`.
         *
         * Unlike @ref lineSegmentLineSegment() computes only **t**.
         */
        template<class T> static T lineSegmentLine(const Vector2<T>& p, const Vector2<T>& r, const Vector2<T>& q, const Vector2<T>& s) {
            return cross(q - p, s)/cross(r, s);
        }

        /**
         * @brief Intersection of a plane and line
         * @param planePosition Plane position
         * @param planeNormal   Plane normal
         * @param p             Starting point of the line
         * @param r             Direction of the line
         * @return Intersection point position `t` on the line, NaN if the
         *      line lies on the plane or infinity if the intersection doesn't
         *      exist. Intersection point can be then computed from with
         *      `p + t*r`. If returned value is in range @f$ [ 0 ; 1 ] @f$, the
         *      intersection is inside the line segment defined by `p` and `r`.
         *
         * First the parameter *f* of parametric equation of the plane
         * is computed from plane normal **n** and plane position: @f[
         *      \begin{pmatrix} n_0 \\ n_1 \\ n_2 \end{pmatrix} \cdot
         *      \begin{pmatrix} x \\ y \\ z \end{pmatrix} - f = 0
         * @f]
         * Using plane normal **n**, parameter *f* and line defined by **p**
         * and **r**, value of *t* is computed and returned. @f[
         *      \begin{array}{rcl}
         *          f & = & \boldsymbol n \cdot (\boldsymbol p + t \boldsymbol r) \\
         *          \Rightarrow t & = & \cfrac{f - \boldsymbol n \cdot \boldsymbol p}{\boldsymbol n \cdot \boldsymbol r}
         *      \end{array}
         * @f]
         */
        template<class T> static T planeLine(const Vector3<T>& planePosition, const Vector3<T>& planeNormal, const Vector3<T>& p, const Vector3<T>& r) {
            const T f = dot(planePosition, planeNormal);
            return (f-dot(planeNormal, p))/dot(planeNormal, r);
        }
};

}}}

#endif
