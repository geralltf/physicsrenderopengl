#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>

Model::Model(std::string* file_name) 
{
    parse(file_name);
}

void Model::parse(std::string* _file_name)
{
    verts = new std::vector<Vector3f*>();     // array of vertices
    tex_coord = new std::vector<Vector3f*>(); // per-vertex array of tex coords
    norms = new std::vector<Vector3f*>();     // per-vertex array of normal vectors
    facet_verts = new std::vector<int>();
    facet_texcoords = new std::vector<int>();  // per-triangle indices in the above arrays
    facet_normals = new std::vector<int>();

    face_normals = new std::vector<Vector3f*>();
    tex_coordA = new std::vector<Vector3f*>();
    face_verts = new std::vector<Vector3f*>();
    face_tangents = new std::vector<Vector3f*>();
    face_bitangents = new std::vector<Vector3f*>();
    face_indicies = new std::vector<unsigned int>();
    
    this->file_name = _file_name;
    VAO = -1;
    VBO = -1;
    EBO = -1;
    std::ifstream in;
    in.open(file_name->c_str(), std::ifstream::in);
    if (!in.is_open())
    {
        std::cout << "Model mesh file could not be opened ' " << *file_name << "'" << std::endl;
        return;
    }
    if (in.fail())
    {
        std::cout << "Model mesh file could not be opened ' " << *file_name << "'" << std::endl;
        return;
    }
    std::string vline = "v ";
    std::string vnline = "vn ";
    std::string vtline = "vt ";
    std::string fline = "f ";
    std::string line;
    unsigned int face_index = 0;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, vline))
        {
            iss >> trash;
            float x;
            float y;
            float z;
            iss >> x;
            iss >> y;
            iss >> z;
            Vector3f* v = new Vector3f(x, y, z);
            verts->push_back(v);
        }
        else if (!line.compare(0, 3, vnline))
        {
            iss >> trash >> trash;
            float x;
            float y;
            float z;
            iss >> x;
            iss >> y;
            iss >> z;
            Vector3f* n = new Vector3f(x, y, z);
            n = n->Normalised();
            norms->push_back(n);
        }
        else if (!line.compare(0, 3, vtline))
        {
            iss >> trash >> trash;
            float x;
            float y;
            float z;
            iss >> x;
            iss >> y;
            iss >> z;
            //Vector3f* uv = new Vector3f(x, 1.0f - y, z);
            Vector3f* uv = new Vector3f(x, y, z);
            tex_coord->push_back(uv);
        }
        else if (!line.compare(0, 2, fline))
        {
            int f, t, n;
            iss >> trash;
            int cnt = 0;
            Vector3f* vert_pos0 = nullptr;
            Vector3f* vert_pos1 = nullptr;
            Vector3f* vert_pos2 = nullptr;
            Vector3f* uv0 = nullptr;
            Vector3f* uv1 = nullptr;
            Vector3f* uv2 = nullptr;
            while (iss >> f >> trash >> t >> trash >> n)
            {
                int FF = --f;
                int TT = --t;
                int NN = --n;
                facet_verts->push_back(FF);
                facet_texcoords->push_back(TT);
                facet_normals->push_back(NN);

                int face_vert_index = FF;
                face_verts->push_back((*verts)[face_vert_index]);

                int face_tex_index = TT;
                tex_coordA->push_back((*tex_coord)[face_tex_index]);
  
                int face_norm_index = NN;
                face_normals->push_back((*norms)[face_norm_index]);


                face_indicies->push_back(face_index);
                
                // Compute tangents and bitangents for TBN matrix required for lighting in shaders.
                if (cnt == 0) 
                {
                    vert_pos0 = (*verts)[face_vert_index];
                    uv0 = (*tex_coord)[face_tex_index];
                }
                if (cnt == 1) 
                {
                    vert_pos1 = (*verts)[face_vert_index];
                    uv1 = (*tex_coord)[face_tex_index];
                }
                if (cnt == 2) 
                {
                    vert_pos2 = (*verts)[face_vert_index];
                    uv2 = (*tex_coord)[face_tex_index];

                    if (vert_pos0 != nullptr && vert_pos1 != nullptr && vert_pos2 != nullptr)
                    {
                        Vector3f* tangent1 = new Vector3f();
                        Vector3f* bitangent1 = new Vector3f();

                        Vector3f* edge1 = Vector3f::Subtract(vert_pos1, vert_pos0);
                        Vector3f* edge2 = Vector3f::Subtract(vert_pos2, vert_pos0);
                        Vector3f* deltaUV1 = Vector3f::Subtract(uv1, uv0);
                        Vector3f* deltaUV2 = Vector3f::Subtract(uv2, uv0);

                        float f = 1.0f / (deltaUV1->x * deltaUV2->y - deltaUV2->x * deltaUV1->y);

                        tangent1->x = f * (deltaUV2->y * edge1->x - deltaUV1->y * edge2->x);
                        tangent1->y = f * (deltaUV2->y * edge1->y - deltaUV1->y * edge2->y);
                        tangent1->z = f * (deltaUV2->y * edge1->z - deltaUV1->y * edge2->z);

                        bitangent1->x = f * (-deltaUV2->x * edge1->x + deltaUV1->x * edge2->x);
                        bitangent1->y = f * (-deltaUV2->x * edge1->y + deltaUV1->x * edge2->y);
                        bitangent1->z = f * (-deltaUV2->x * edge1->z + deltaUV1->x * edge2->z);

                        face_tangents->push_back(tangent1);
                        face_bitangents->push_back(bitangent1);
                    }
                }

                cnt++;
                face_index++;
            }
            if (3 != cnt) 
            {
                std::cout << "Error: the obj file is supposed to be triangulated '" << *file_name << "'" << std::endl;
                return;
            }
        }
    }

    std::cout << "[Loaded] Mesh verts count: " << verts_count() << " faces: " << faces_count() << " texcoords: " << tex_coord->size() << " vertexnormals: " << norms->size() << " '" << *file_name << "'" << std::endl;
}

Vector3f* Model::uv(const int iface, const int nthvert) const {
    return (*tex_coord)[(*facet_texcoords)[iface * 3 + nthvert]];
}

int Model::verts_count() const 
{
    return verts->size();
}

int Model::faces_count() const 
{
    return facet_verts->size() / 3;
}

float* to_float_array(Vector3f** array, size_t size)
{
    std::vector<float>* primative_array = new std::vector<float>();
    for (int i = 0; i < size; i++) {
        primative_array->push_back(array[i]->x);
        primative_array->push_back(array[i]->y);
        primative_array->push_back(array[i]->z);
    }
    return primative_array->data();
}
float* to_float_array(Vector2f** array, size_t size)
{
    std::vector<float>* primative_array = new std::vector<float>();
    for (int i = 0; i < size; i++) {
        primative_array->push_back(array[i]->x);
        primative_array->push_back(array[i]->y);
    }
    return primative_array->data();
}
float* to_float_array(Vector4f** array, size_t size)
{
    std::vector<float>* primative_array = new std::vector<float>();
    for (int i = 0; i < size; i++) {
        primative_array->push_back(array[i]->x);
        primative_array->push_back(array[i]->y);
        primative_array->push_back(array[i]->z);
        primative_array->push_back(array[i]->w);
    }
    return primative_array->data();
}
void Model::init()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    float* farray = to_float_array(face_verts->data(), face_verts->size());
    float* farray_norms = to_float_array(face_normals->data(), face_normals->size());
    float* farray_texcoords = to_float_array(tex_coordA->data(), tex_coordA->size());
    float* farray_tangents = to_float_array(face_tangents->data(), face_tangents->size());
    float* farray_bitangents = to_float_array(face_bitangents->data(), face_bitangents->size());

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, face_verts->size() * sizeof(float) * 3,farray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    //glVertexPointer(3, GL_FLOAT, 0, farray);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //glBufferData(GL_ARRAY_BUFFER, norms->size() * sizeof(float) * 3, farray_norms, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glGenBuffers(1, &normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, face_normals->size() * sizeof(float) * 3, farray_norms, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glNormalPointer(GL_FLOAT, 0, face_normals->data());
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(verts->size()));

    glGenBuffers(1, &texcoordsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, tex_coordA->size() * sizeof(float) * 3, farray_texcoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glTexCoordPointer(3, GL_FLOAT, 0, tex_coordA->data());
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(verts->size() + norms->size()));

    glGenBuffers(1, &tangentsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
    glBufferData(GL_ARRAY_BUFFER, face_tangents->size() * sizeof(float) * 3, farray_tangents, GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    //glTexCoordPointer(3, GL_FLOAT, 0, face_tangents->data());
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &bitangentsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
    glBufferData(GL_ARRAY_BUFFER, face_bitangents->size() * sizeof(float) * 3, farray_bitangents, GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    //glTexCoordPointer(3, GL_FLOAT, 0, face_tangents->data());
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //glVertexPointer(3, GL_FLOAT, 0, bananaVerts);
    //glNormalPointer(GL_FLOAT, 0, bananaNormals);
    //glTexCoordPointer(2, GL_FLOAT, 0, bananaTexCoords);
    




    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_indicies->size() * sizeof(unsigned int), face_indicies->data(), GL_STATIC_DRAW);





    //glBufferData(GL_ARRAY_BUFFER, verts->size() * sizeof(float), verts->data(), GL_STATIC_DRAW);

    // fill buffer
    //glBufferSubData(GL_ARRAY_BUFFER, 0, verts->size() * 3, to_float_array(verts->data(), verts->size()));
    //glBufferSubData(GL_ARRAY_BUFFER, verts->size(), norms->size(), to_float_array(norms->data(), norms->size()));
    //glBufferSubData(GL_ARRAY_BUFFER, verts->size() + norms->size(), tex_coord->size(), to_float_array(tex_coord->data(), tex_coord->size()));

    //glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(verts->size()));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(verts->size() + norms->size()));

    has_init = true;

    std::cout << "[Init] model mesh '" << *this->file_name << "'" << std::endl;

}
void Model::bind()
{
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}
