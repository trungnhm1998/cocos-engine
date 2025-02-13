#include "cocos/core/geometry/Distance.h"

#include "cocos/core/geometry/AABB.h"
#include "cocos/core/geometry/Obb.h"
#include "cocos/core/geometry/Plane.h"
#include "cocos/math/Mat4.h"

#include <algorithm>
#include "base/std/container/array.h"
#include "cocos/math/Utils.h"
#include "cocos/math/Vec3.h"

namespace cc {
namespace geometry {

float pointPlane(const Vec3 &point, const Plane &plane) {
    return Vec3::dot(plane.n, point) - plane.d;
}

Vec3 *ptPointPlane(Vec3 *out, const Vec3 &point, const Plane &plane) {
    auto t = pointPlane(point, plane);
    *out   = point - t * plane.n;
    return out;
}

Vec3 *ptPointAabb(Vec3 *out, const Vec3 &point, const AABB &aabb) {
    auto min = aabb.getCenter() - aabb.getHalfExtents();
    auto max = aabb.getCenter() + aabb.getHalfExtents();
    *out     = {cc::mathutils::clamp(point.x, min.x, max.x),
            cc::mathutils::clamp(point.y, min.y, max.y),
            cc::mathutils::clamp(point.z, min.z, max.z)};
    return out;
}

Vec3 *ptPointObb(Vec3 *out, const Vec3 &point, const OBB &obb) {
    ccstd::array<Vec3, 3> u = {
        Vec3{obb.orientation.m[0], obb.orientation.m[1], obb.orientation.m[2]},
        Vec3{obb.orientation.m[3], obb.orientation.m[4], obb.orientation.m[5]},
        Vec3{obb.orientation.m[6], obb.orientation.m[7], obb.orientation.m[8]},
    };
    ccstd::array<float, 3> e = {obb.halfExtents.x, obb.halfExtents.y, obb.halfExtents.z};

    auto  d    = point - obb.center;
    float dist = 0.0F;

    // Start result at center of obb; make steps from there
    *out = obb.center;

    // For each OBB axis...
    for (int i = 0; i < 3; i++) {
        // ...project d onto that axis to get the distance
        // along the axis of d from the obb center
        dist = Vec3::dot(d, u[i]);
        // if distance farther than the obb extents, clamp to the obb
        dist = cc::mathutils::clamp(dist, -e[i], e[i]);

        // Step that distance along the axis to get world coordinate
        *out += (dist * u[i]);
    }
    return out;
}

Vec3 *ptPointLine(Vec3 *out, const Vec3 &point, const Vec3 &linePointA, const Vec3 &linePointB) {
    auto dir        = linePointA - linePointB;
    auto dirSquared = dir.lengthSquared();

    if (dirSquared == 0.0F) {
        // The point is at the segment start.
        *out = linePointA;
    } else {
        // Calculate the projection of the point onto the line extending through the segment.
        auto ap = point - linePointA;
        auto t  = Vec3::dot(ap, dir) / dirSquared;
        *out    = linePointA + cc::mathutils::clamp(t, 0.0F, 1.0F) * dir;
    }
    return out;
}

} // namespace geometry
} // namespace cc