#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include "Vector2f.h"
#include "Vector3f.h"

#include <GL\glew.h>
//#define GLFW_DLL
#include "GLFW\glfw3.h"

class Model 
{
public:
    std::vector<Vector3f*>* verts;     // array of vertices
    std::vector<Vector3f*>* tex_coord; // per-vertex array of tex coords
    std::vector<Vector3f*>* norms;     // per-vertex array of normal vectors
    std::vector<int>* facet_verts;
    std::vector<int>* facet_texcoords;  // per-triangle indices in the above arrays
    std::vector<int>* facet_normals;

    std::vector<unsigned int>* face_indicies;
    std::vector<Vector3f*>* face_verts;
    std::vector<Vector3f*>* tex_coordA;
    std::vector<Vector3f*>* face_normals;
    std::vector<Vector3f*>* face_tangents;
    std::vector<Vector3f*>* face_bitangents;
public:
    std::string* file_name;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int normalsBuffer;
    unsigned int texcoordsBuffer;
    unsigned int tangentsBuffer;
    unsigned int bitangentsBuffer;
    unsigned int EBO;
    bool has_init = false;
    Model(std::string* filename);
    void parse(std::string* _file_name);
    Vector3f* uv(const int iface, const int nthvert) const;
    int verts_count() const;
    int faces_count() const;
    void init();
    void bind();
};

#endif