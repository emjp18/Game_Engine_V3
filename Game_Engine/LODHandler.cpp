#include "stdafx.h"
#include "LODHandler.h"

CLASSIFICATION LODHandler::classify(POLYGON_TRIS* triangle, PLANE* plane)
{
    // Loop over all polygon vertices and count how many vertices
     // lie in front of and how many lie behind of the thickened plane
    int numInFront = 0, numBehind = 0;
    int numVerts = 3;
    for (int i = 0; i < numVerts; i++) {
        Vector3 p = triangle->vertices[i].pos;
        switch (classify2(p, plane)) {
        case 1:
            numInFront++;
            break;
        case -1:
            numBehind++;
            break;
        }
    }
    // If vertices on both sides of the plane, the polygon is straddling
    if (numBehind != 0 && numInFront != 0)
        return CLASSIFICATION::SPANNING;
    // If one or more vertices in front of the plane and no vertices behind
    // the plane, the polygon lies in front of the plane
    if (numInFront != 0)
        return CLASSIFICATION::IN_FRONT_OF;
    // Ditto, the polygon lies behind the plane if no vertices in front of
    // the plane, and one or more vertices behind the plane
    if (numBehind != 0)
        return CLASSIFICATION::IN_BACK_OF;
    // All vertices lie on the plane so the polygon is coplanar with the plane
    return CLASSIFICATION::IN_BACK_OF;
}

int LODHandler::classify(Vector3& point, PLANE* plane)
{
    return 0;
}

int LODHandler::classify2(Vector3& point, PLANE* plane)
{
    Vector3 N = plane->N;

    float signedD = (point.Dot(N)) - plane->d;

    if (signedD > FLT_EPSILON)
    {
        return 1;
    }
    else if (signedD < -FLT_EPSILON)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

PLANE* LODHandler::getPlaneFromTriangles(std::vector<POLYGON_TRIS*>& tri)
{
    // Blend factor for optimizing for balance or splits (should be tweaked)
    const float K = 0.8f;
    // Variables for tracking best splitting plane seen so far
    PLANE* bestPlane = nullptr;
    float bestScore = FLT_MAX;
    // Try the plane of each polygon as a dividing plane
    for (int i = 0; i < tri.size(); i++) {
        int numInFront = 0, numBehind = 0, numStraddling = 0;
        PLANE* plane = getPlaneFromTriangle(tri[i]);

        // Test against all other polygons
        for (int j = 0; j < tri.size(); j++) {
            // Ignore testing against self
            if (i == j) continue;
            // Keep standing count of the various poly-plane relationships
            switch (classify(tri[j], plane)) {
            case CLASSIFICATION::COINCIDENT:
                /* Coplanar polygons treated as being in front of plane */
                numInFront++;
                break;
            case CLASSIFICATION::IN_FRONT_OF:
                numInFront++;
                break;
            case CLASSIFICATION::IN_BACK_OF:
                numBehind++;
                break;
            case CLASSIFICATION::SPANNING:
                numStraddling++;
                break;
            }
        }
        // Compute score as a weighted combination (based on K, with K in range
        // 0..1) between balance and splits (lower score is better)
        float score = K * numStraddling + (1.0f - K) * fabsf((float)(numInFront - numBehind));
        if (score < bestScore) {
            bestScore = score;
            bestPlane = plane;
        }
    }
    return bestPlane;
}

PLANE* LODHandler::getPlaneFromTriangle(POLYGON_TRIS* tri)
{
    PLANE* plane = new PLANE;
   
    Vector3 a = tri->vertices[0].pos;
    Vector3 b = tri->vertices[1].pos;
    Vector3 c = tri->vertices[2].pos;

    plane->p = a;

    Vector3 edge0 = a - b;
    Vector3 edge1 = a - c;

    Vector3 N = (edge0.Cross(edge1));

    N.Normalize();

    plane->N = N;

    plane->d = N.Dot(a); //noramlize w?


    assert(N != Vector3::Zero);



    return plane;
}

Vector3& LODHandler::rayPlaneInters(Ray& ray, PLANE* plane, float& t)
{
    Vector3 N = plane->N;
    float denominator = N.Dot(ray.direction);
    assert(denominator != 0);
    Vector3 difference = plane->p - ray.position;
    t = difference.Dot(plane->N) / denominator;
    assert(t > -1);
    m_i = ray.position + (t * ray.direction);
    return m_i;
}

Vector4 LODHandler::createPlane(const Vector3& zero, const Vector3& one, const Vector3& two, const Vector3& three)
{
    Vector4 plane;
    Vector3 a = zero;
    Vector3 b = one;
    Vector3 c = two;

    Vector3 edge0 = a - b;
    Vector3 edge1 = a - c;

    Vector3 N = edge0.Cross(edge1);

    if (N != Vector3::Zero)
    {
        Vector3 x = three;
        Vector3 edge2 = a - x;

        N = edge2.Cross(edge1);
    }

    N.Normalize();

    plane.w = N.Dot(zero);



    return plane;
}

Vector4 LODHandler::createPlane(const Vector3& zero, const Vector3& one, const Vector3& two)
{
    Vector4 plane;
    Vector3 a = zero;
    Vector3 b = one;
    Vector3 c = two;

    Vector3 edge0 = a - b;
    Vector3 edge1 = a - c;

    Vector3 N =  edge0.Cross(edge1);

    N.Normalize();
    plane.x = N.x;
    plane.y = N.y;
    plane.z = N.z;
    plane.w = N.Dot(a);
    return plane;
}

Vector4 LODHandler::createPlane(const Vector3& zero, const Vector3& normal)
{
    Vector4 plane;
    Vector3 N = normal;
    N.Normalize();
    plane.x = N.x;
    plane.y = N.y;
    plane.z = N.z;
    plane.w = N.Dot(zero);
    return plane;
}

void QUADTREE_NODE::pureVirtual()
{
}

void OCTREE_NODE::pureVirtual()
{
}
