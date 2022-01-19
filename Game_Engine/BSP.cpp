#include "stdafx.h"
#include "BSP.h"

void BSP::release(BINARY_NODE* node)
{
    if (node)
    {
        if (node->frontchild)
        {
            release(node->frontchild);
        }
        if (node->backchild)
        {
            release(node->backchild);
        }
        RELEASE(node->shape);
        RELEASE(node->plane);
    }
}

bool BSP::shouldSwitchNormal(Vector3& a, Vector3& b, Vector3& c, bool orientationShouldBeNegative)
{
    Vector3 base(0, 1, 0);
    if (orientationShouldBeNegative)
    {
        base *= -1;
    }

    Vector3 e0 = b - a;
    Vector3 e1 = c - a;

    Vector3 sn = e0.Cross(e1);

    sn.Normalize();


    float radians = XMScalarACos(sn.Dot(base));

    float degrees = XMConvertToDegrees(radians);

    if (degrees < 315 && degrees>45)
    {
        return true;
    }
    return false;
}

void BSP::calcNormals(POLYGON_TRIS* poly)
{
    Vector3 vertexA;
    Vector3 vertexB;
    Vector3 vertexC;

    vertexA = poly->vertices[0].pos;

    vertexB = poly->vertices[1].pos;

    vertexC = poly->vertices[2].pos;

    Vector3 edge1 = vertexB - vertexA;
    Vector3 edge2 = vertexC - vertexA;

    Vector3 surfaceNormal = edge2.Cross(edge1);

    surfaceNormal.Normalize();


    poly->vertices[0].normal.x = 0;
    poly->vertices[0].normal.y = 0;
    poly->vertices[0].normal.z = 0;
    poly->vertices[0].tangent.x = 0;
    poly->vertices[0].tangent.y = 0;
    poly->vertices[0].tangent.z = 0;
    poly->vertices[0].biNormal.x = 0;
    poly->vertices[0].biNormal.y = 0;
    poly->vertices[0].biNormal.z = 0;

    poly->vertices[1].normal.x = 0;
    poly->vertices[1].normal.y = 0;
    poly->vertices[1].normal.z = 0;
    poly->vertices[1].tangent.x = 0;
    poly->vertices[1].tangent.y = 0;
    poly->vertices[1].tangent.z = 0;
    poly->vertices[1].biNormal.x = 0;
    poly->vertices[1].biNormal.y = 0;
    poly->vertices[1].biNormal.z = 0;

    poly->vertices[2].normal.x = 0;
    poly->vertices[2].normal.y = 0;
    poly->vertices[2].normal.z = 0;
    poly->vertices[2].tangent.x = 0;
    poly->vertices[2].tangent.y = 0;
    poly->vertices[2].tangent.z = 0;
    poly->vertices[2].biNormal.x = 0;
    poly->vertices[2].biNormal.y = 0;
    poly->vertices[2].biNormal.z = 0;

    poly->vertices[0].normal.x += surfaceNormal.x;
    poly->vertices[0].normal.y += surfaceNormal.y;
    poly->vertices[0].normal.z += surfaceNormal.z;
    poly->vertices[1].normal.x += surfaceNormal.x;
    poly->vertices[1].normal.y += surfaceNormal.y;
    poly->vertices[1].normal.z += surfaceNormal.z;
    poly->vertices[2].normal.x += surfaceNormal.x;
    poly->vertices[2].normal.y += surfaceNormal.y;
    poly->vertices[2].normal.z += surfaceNormal.z;



    //tangents



    Vector3 uv1(poly->vertices[0].uv.x, poly->vertices[0].uv.y, 0);
    Vector3 uv2(poly->vertices[1].uv.x, poly->vertices[1].uv.y, 0);
    Vector3 uv3(poly->vertices[2].uv.x, poly->vertices[2].uv.x, 0);



    Vector3 u = uv2 - uv1;
    Vector3 v = uv3 - uv1;
    float r = 1;

    if ((u.x * v.y) - (u.y * v.x) != 0)
    {
        float r = 1.0f / (((float)u.x * (float)v.y) - ((float)u.y * (float)v.x));

    }

    Vector3 tan((((edge1.x * v.y) - (edge2.x * u.y))) * r,
        (((edge1.y * v.y) - (edge2.y * u.y))) * r,
        (((edge1.z) * v.y) - (edge2.z * u.y)) * r);

    Vector3 bi((((edge1.x * v.x) - (edge2.x * u.x))) * r,
        (((edge1.y * v.x) - (edge2.y * u.x))) * r,
        (((edge1.z) * v.x) - (edge2.z * u.x)) * r);



    Vector3 tanA[3];
    Vector3 tanB[3];

    tanA[0].x += tan.x;
    tanA[0].y += tan.y;
    tanA[0].z += tan.z;

    tanA[1].x += tan.x;
    tanA[1].y += tan.y;
    tanA[1].z += tan.z;

    tanA[2].x += tan.x;
    tanA[2].y += tan.y;
    tanA[2].z += tan.z;


    tanB[0].x += bi.x;
    tanB[0].y += bi.y;
    tanB[0].z += bi.z;

    tanB[1].x += bi.x;
    tanB[1].y += bi.y;
    tanB[1].z += bi.z;

    tanB[2].x += bi.x;
    tanB[2].y += bi.y;
    tanB[2].z += bi.z;

    for (int i = 0; i < 3; i++)
    {
        Vector3 t0(tanA[i].x, tanA[i].y, tanA[i].z);
        Vector3 t1(tanB[i].x, tanB[i].y, tanB[i].z);

        Vector3 n(poly->vertices[i].normal.x, poly->vertices[i].normal.y, poly->vertices[i].normal.z);

        double scalar = n.Dot(t0);

        n *= (float)scalar;

        Vector3 t = t0 - n;

        t.Normalize();

        poly->vertices[0].tangent.x = (float)t.x;
        poly->vertices[1].tangent.y = (float)t.y;
        poly->vertices[2].tangent.z = (float)t.z;

        Vector3 bi = t.Cross(n);

        bi.Normalize();

        poly->vertices[0].biNormal.x = (float)bi.x;
        poly->vertices[1].biNormal.y = (float)bi.y;
        poly->vertices[2].biNormal.z = (float)bi.z;
    }
}

void BSP::buildTree(BINARY_NODE*& node, std::vector<POLYGON_TRIS*>& polygons, int& depth)
{
    if (polygons.empty())
    {
        RELEASE(node);
        return;
    }

    node->plane = getPlaneFromTriangles(polygons);
    //node->plane = getPlaneFromTriangle(polygons[0]);




    if (depth > BSP_MAX_DEPTH)
    {
        node->polygons = polygons;
        return;
    }
    std::vector<POLYGON_TRIS*> front_list,
        back_list;

    for (auto poly : polygons)
    {




        CLASSIFICATION result = classify(poly, node->plane);
        switch (result)
        {
        case CLASSIFICATION::COINCIDENT:
            node->polygons.push_back(poly);
            break;
        case CLASSIFICATION::IN_BACK_OF:
            back_list.push_back(poly);
            break;
        case CLASSIFICATION::IN_FRONT_OF:
            front_list.push_back(poly);
            break;
        case CLASSIFICATION::SPANNING:
            std::vector<POLYGON_TRIS*> front_piece;
            std::vector<POLYGON_TRIS*>back_piece;

            split(poly, node->plane, front_piece, back_piece); //split4

            for (auto back : back_piece)
            {
                back_list.push_back(back);
            }
            for (auto front : front_piece)
            {
                front_list.push_back(front);
            }


            break;
        }
    }
    if (!front_list.empty())
    {
        int d = depth + 1;
        node->frontchild = new BINARY_NODE;
        buildTree(node->frontchild, front_list, d);
    }
    if (!back_list.empty())
    {
        int d = depth + 1;
        node->backchild = new BINARY_NODE;
        buildTree(node->backchild, back_list, d);
    }
}

void BSP::split(POLYGON_TRIS* poly, PLANE* part, std::vector<POLYGON_TRIS*>& front, std::vector<POLYGON_TRIS*>& back)
{
    Vector3 pointA = poly->vertices[0].pos;
    Vector3 pointB = poly->vertices[1].pos;
    Vector3 pointC = poly->vertices[2].pos;
    VERTEX vA = poly->vertices[0];
    VERTEX vB = poly->vertices[1];
    VERTEX vC = poly->vertices[2];
    int v1 = classify2(pointA, part);         // -1, 0 or 1 depending on which side we are in
    int v2 = classify2(pointB, part);
    int v3 = classify2(pointC, part);
    bool INHERITEDORIENTATION = poly->negaticeFace;
    int val = v1 + v2 + v3;

    switch (val)
    {
    case 3:
    {

        front.push_back(poly);

        return;
    }


    case -3:
    {

        back.push_back(poly);

        return;
    }


    case -2:
    {
        // triangle with two vtx on the plane and the other on negative side
        // no need to divide it

        back.push_back(poly);

        return;
    }

    case 2:
    {
        // triangle with two vtx on the plane and the other on positive side
       // no need to divide it

        front.push_back(poly);

        return;
    }

    case 0:
    {
        // triangle in plane or triangle with one vertex in plane and
        // other two in opposite sides. The latter requires a divide.
        if (v1 || v2 || v3) // two vertices on opposite sides... divide
        {
            Vector3 pivot, positive, negative;
            VERTEX pivotV, positiveV, negativeV;
            if (v1 == 0)
            {
                pivot = pointA;
                pivotV = vA;
                if (v2 > 0) { positive = pointB; negative = pointC; positiveV = vB; negativeV = vC; }
                else {
                    positive = pointC; negative = pointB; positiveV = vC; negativeV = vB;
                }
            }
            if (v2 == 0)
            {
                pivot = pointB;
                pivotV = vB;
                if (v1 > 0) { positive = pointA; negative = pointC; positiveV = vA; negativeV = vC; }
                else { positive = pointC; negative = pointA; positiveV = vC; negativeV = vA; }
            }
            if (v3 == 0)
            {
                pivot = pointC;
                pivotV = vC;
                if (v1 > 0) { positive = pointA; negative = pointB; positiveV = vA; negativeV = vB; }
                else { positive = pointB; negative = pointA; positiveV = vB; negativeV = vA; }
            }
            // here positive, pivot and negative are ready
            Vector3 i;
            float t;
            Ray ray(positive, (negative - positive));
            i = rayPlaneInters(ray, part, t);
            POLYGON_TRIS* polygon = new POLYGON_TRIS;
           
          
            VERTEX vI = interpolate(positiveV, negativeV, findT(positive, negative, i));
            vI.pos = i;

            polygon->vertices[0] = positiveV;
            polygon->vertices[1] = vI;
            polygon->vertices[2] = pivotV;
            polygon->negaticeFace = INHERITEDORIENTATION;
     



            front.push_back(polygon);
            POLYGON_TRIS* polygon2 = new POLYGON_TRIS;
           
           
            polygon2->vertices[0] = negativeV;
            polygon2->vertices[1] = pivotV;
            polygon2->vertices[2] = vI;
            polygon2->negaticeFace = INHERITEDORIENTATION;
        




            back.push_back(polygon2);
            m_allpolygons.push_back(polygon);
            m_allpolygons.push_back(polygon2);

         


            return;
        }
        else // triangle is inside plane... assign to positive node
        {

            front.push_back(poly);
            return;
        }
    }


    case -1:
    {
        // can be: two vtx on plane and one on negative side, one vertex
        // on positive and two vertices on negative. Latter requires a divide
        if (v1 * v2 * v3 == 0) // one of them was zero: we're on the first case
        {

            back.push_back(poly);
            return;
        }
        // one vertex on positive, two on negative. Split
        Vector3 positive, negative1, negative2;
        VERTEX positiveV, negative1V, negative2V;
        if (v1 == 1) { positive = pointA; negative1 = pointB; negative2 = pointC; positiveV = vA; negative1V = vB; negative2V = vC; }
        if (v2 == 1) { positive = pointB; negative1 = pointA; negative2 = pointC; positiveV = vB; negative1V = vA; negative2V = vC; }
        if (v3 == 1) { positive = pointC; negative1 = pointA; negative2 = pointB; positiveV = vC; negative1V = vA; negative2V = vB; }

        Vector3 i1, i2;
        VERTEX i1V, i2V;
        float t;
        Ray ray(negative1, (positive - negative1));
        i1 = rayPlaneInters(ray, part, t);
        Ray ray2(negative2, (positive - negative2));
        i2 = rayPlaneInters(ray2, part, t);

        t = findT(negative1, positive, i1);
        i1V = interpolate(negative2V, negative1V, t);
        i1V.pos = i1;
        t = findT(negative2, positive, i2);
        i2V = interpolate(positiveV, i1V, t);
        i2V.pos = i2;


        POLYGON_TRIS* polygon = new POLYGON_TRIS;
    
        polygon->vertices[0] = positiveV;
        polygon->vertices[1] = i1V;
        polygon->vertices[2] = i2V;
        polygon->negaticeFace = INHERITEDORIENTATION;
   




        front.push_back(polygon);
        POLYGON_TRIS* polygon2 = new POLYGON_TRIS;

        POLYGON_TRIS* polygon3 = new POLYGON_TRIS;
  
  

        polygon2->vertices[0] = negative2V;
        polygon2->vertices[1] = i2V;
        polygon2->vertices[2] = i1V;

        polygon2->negaticeFace = INHERITEDORIENTATION;
     

        back.push_back(polygon2);

        polygon3->vertices[0] = negative2V;
        polygon3->vertices[1] = i1V;
        polygon3->vertices[2] = negative1V;

        polygon3->negaticeFace = INHERITEDORIENTATION;
      


        back.push_back(polygon3);
        m_allpolygons.push_back(polygon);
        m_allpolygons.push_back(polygon2);
        m_allpolygons.push_back(polygon3);



        return;
    }



    case 1:
    {
        // can be: two vtx on plane and one on positive side, one vertex
      // on negative and two vertices on positive.Latter requires a divide
        if (v1 * v2 * v3 == 0) // one of them was zero: we're on the first case
        {

            front.push_back(poly);

            return;
        }
        // one vertex on negative, two on positive. Split
        Vector3 positive1, positive2, negative;
        VERTEX positive1V, positive2V, negativeV;
        if (v1 == -1) { negative = pointA; positive1 = pointB; positive2 = pointC; negativeV = vA; positive1V = vB; positive2V = vC; }
        if (v2 == -1) { negative = pointB; positive1 = pointA; positive2 = pointC; negativeV = vB; positive1V = vA; positive2V = vC; }
        if (v3 == -1) { negative = pointC; positive1 = pointA; positive2 = pointB; negativeV = vC; positive1V = vA; positive2V = vB; }

        Vector3 i1, i2;
        VERTEX i1V, i2V;
        float t;
        Ray ray(positive1, (negative - positive1));
        i1 = rayPlaneInters(ray, part, t);

        Ray ray2(positive2, (negative - positive2));
        i2 = rayPlaneInters(ray2, part, t);

        t = findT(positive1, negative, i2);

        i2V = interpolate(positive1V, negativeV, t);
        i2V.pos = i2;
        t = findT(positive2, negative, i1);

        i1V = interpolate(positive2V, negativeV, t);
        i1V.pos = i1;
        POLYGON_TRIS* polygon = new POLYGON_TRIS;


        POLYGON_TRIS* polygon2 = new POLYGON_TRIS;


      //  polygon2->vertices = new VERTEX[3];

        polygon->negaticeFace = INHERITEDORIENTATION;
        polygon->vertices[0] = positive1V;
        polygon->vertices[1] = i1V;
        polygon->vertices[2] = i2V;

        


        front.push_back(polygon);

        polygon2->vertices[0] = positive1V;
        polygon2->vertices[1] = i2V;
        polygon2->vertices[2] = positive2V;


        polygon2->negaticeFace = INHERITEDORIENTATION;

        front.push_back(polygon2);
        POLYGON_TRIS* polygon3 = new POLYGON_TRIS;

        polygon3->vertices[0] = negativeV;
        polygon3->vertices[1] = i2V;
        polygon3->vertices[2] = i1V;

        
        polygon3->negaticeFace = INHERITEDORIENTATION;
        back.push_back(polygon3);
        m_allpolygons.push_back(polygon);
        m_allpolygons.push_back( polygon2);
        m_allpolygons.push_back( polygon3);




        return;
    }

    }
}

void BSP::splitShape(BINARY_NODE* node, ID3D11Device* device)
{
    if (node)
    {
        if (node->backchild)
        {
            splitShape(node->backchild, device);
        }
        if (node->frontchild)
        {
            splitShape(node->frontchild, device);
        }
        
        std::vector<VERTEX> sortedVertices;
        std::vector<unsigned int> sortedIndices;
        int c = 0;
        for (int i = 0; i < node->polygons.size(); i++)
        {


            Vector3 aV = node->polygons[i]->vertices[0].pos;
            Vector3 bV = node->polygons[i]->vertices[1].pos;
            Vector3 cV = node->polygons[i]->vertices[2].pos;

            if (shouldSwitchNormal(aV, bV, cV, true))
            {

                std::swap(node->polygons[i]->vertices[1], node->polygons[i]->vertices[2]);

            }

            for (int j = 0; j < 3; j++)
            {
                std::vector<VERTEX>::iterator it = std::find(sortedVertices.begin(), sortedVertices.end(), node->polygons[i]->vertices[j]);

                if (it == sortedVertices.end())
                {
                    sortedVertices.push_back(node->polygons[i]->vertices[j]);
                    sortedIndices.push_back(c++);

                }
                else
                {
                    unsigned int index = (unsigned int)std::distance(sortedVertices.begin(), it);
                    sortedIndices.push_back(index);
                }

            }


        }
        if (node->polygons.size() > 0)
        {
            Shape* shapePtr = new Shape(device);
            shapePtr->setVertices(device, sortedVertices);
            shapePtr->setIndices(device, sortedIndices);
            shapePtr->generateBounds();
            node->aabb = shapePtr->getBB().getBounds().aabb;
            node->shape = shapePtr;
            node->shape->setTranslation(shapePtr->getLocalTranformations().trans);
            node->shape->setScale(shapePtr->getLocalTranformations().scale);
            DC_BASIC* dc = dynamic_cast<DC_BASIC*>(node->shape->getDrawCall());
            node->shape->updateCBfr();
            m_allShapes.push_back( shapePtr);
        }
    }
}

VERTEX& BSP::interpolate(VERTEX& a, VERTEX& b, float t)
{
    assert(t <= 1 && t >= 0);
    
    m_v.normal = a.normal + (b.normal - a.normal) * t;
    m_v.tangent = a.tangent + (b.tangent - a.tangent) * t;
    m_v.biNormal = a.biNormal + (b.biNormal - a.biNormal) * t;

    m_v.uv = a.uv + (b.uv - a.uv) * t;
    return m_v;
}

float BSP::findT(Vector3& posA, Vector3& posB, Vector3& result)
{
    //posA + (posB - posA) * t = result;

    Vector3 s = posB - posA;

    //posA + s * t = result;

    Vector3 s2 = result - posA;

    // s * t = s2;
    float t = 0;
    if (s.x != 0)
        t = s2.x / s.x;


    if (t > 1)
        t = 1;

    if (t < 0)
        t = 0;
    return t;
}

BSP::BSP()
{
}

BSP::~BSP()
{
    release(g_root);
    RELEASE(g_root);
    for (auto poly : m_allpolygons)
    {
        RELEASE(poly);
    }
    RELEASE_ARR(m_frontpoly);
    RELEASE_ARR(m_backpoly);
    RELEASE_ARR(m_poly);
}

void BSP::init(Shape* shapeToSplit, ID3D11Device* device)
{
   
    m_poly = new POLYGON_TRIS * [shapeToSplit->getIndices().size() / 3];
    m_polygons.resize(shapeToSplit->getIndices().size() / 3);
    for (int i = 0; i < shapeToSplit->getIndices().size() / 3; i++)
    {
        int index = i * 3;
        m_poly[i] = new POLYGON_TRIS;
        VERTEX list = shapeToSplit->getVertices()[shapeToSplit->getIndices()[index]];
        VERTEX list1 = shapeToSplit->getVertices()[shapeToSplit->getIndices()[index+1]];
        VERTEX list2 = shapeToSplit->getVertices()[shapeToSplit->getIndices()[index+2]];
        m_poly[i]->vertices[0].pos = list.pos;
        m_poly[i]->vertices[0].uv = list.uv;
        m_poly[i]->vertices[0].normal = list.normal;
        m_poly[i]->vertices[0].tangent = list.tangent;
        m_poly[i]->vertices[0].biNormal = list.biNormal;
                   
        m_poly[i]->vertices[1].pos = list1.pos;
        m_poly[i]->vertices[1].uv = list1.uv;
        m_poly[i]->vertices[1].normal = list1.normal;
        m_poly[i]->vertices[1].tangent = list1.tangent;
        m_poly[i]->vertices[1].biNormal = list1.biNormal;

        m_poly[i]->vertices[2].pos = list2.pos;
        m_poly[i]->vertices[2].uv = list2.uv;
        m_poly[i]->vertices[2].normal = list2.normal;
        m_poly[i]->vertices[2].tangent = list2.tangent;
        m_poly[i]->vertices[2].biNormal = list2.biNormal;

        Vector3 aV = m_poly[i]->vertices[0].pos;
        Vector3 bV = m_poly[i]->vertices[1].pos;
        Vector3 cV = m_poly[i]->vertices[2].pos;

        m_poly[i]->negaticeFace = shouldSwitchNormal(aV, bV, cV, true);

        m_polygons[i] = m_poly[i];
    }
    m_allpolygons = m_polygons;
;
    g_root = new BINARY_NODE;
    int d = 0;
    buildTree(g_root, m_polygons, d);
    splitShape(g_root, device);
    RELEASE(shapeToSplit);
}

int BSP::classifyCamera(Vector3& point, PLANE* plane)
{
    Vector3 N = plane->N;
    float signedD = (N.x * point.x) + (N.y * point.y) +
        (N.z * point.z) - plane->d;

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
