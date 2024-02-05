#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

//#include "VectorLib.h"
#include "Vector2f.h"
#include "AABB.h"
#include "ShaderProgram.h"
#include "QueryRenderSample.h"

class QuadTree
{
public:
    // Arbitrary constant to indicate how many elements can be stored in this quad tree node
    const int QT_NODE_CAPACITY = 4;

    // Axis-aligned bounding box stored as a center with half-dimensions
    // to represent the boundaries of this quad tree
    AABB* boundary = nullptr;

    std::vector<Vector2f*>* points = nullptr;

    std::vector<QueryRenderSample*>* render_samples = nullptr;
    bool has_init = false;

    QuadTree* north_west = nullptr;
    QuadTree* north_east = nullptr;
    QuadTree* south_west = nullptr;
    QuadTree* south_east = nullptr;

    ShaderProgram* shader_program;

    QuadTree(AABB* _boundary);
    void subdivide();

    // Insert a point into the QuadTree
    bool insert(Vector2f* p);

public:

    // Find all points that appear within a range
    std::vector<QueryRenderSample*>* query_render_range(AABB* range, Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float cameraAngle);

    // Find all points that appear within a range
    std::vector<Vector2f*>* query_range(AABB* range);
};

#endif