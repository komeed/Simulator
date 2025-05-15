//
// Created by Omeed on 5/3/25.
//

#ifndef SOLVER_H
#define SOLVER_H

#include <constants.h>
#include <Ball.h>
#include <vector>
#include <iostream>

class Solver {

    float radius;

public:
    Solver(float radius = 1, int subdivision=2, bool smooth=false, unsigned int shaderProgram = 0);
    void append(std::unique_ptr<Ball> ball);
    void nextFrame(float dt);
    void setConstraint(bool set);
    int returnCount() { return balls.size(); }
    void applyForce(glm::vec3 force, float mag);
    void clear() { balls.clear(); }

private:
    //buffers
    Buffers sphereBuffer;
    Buffers lineBuffer;
    Buffers pointBuffer;
    //std::vector<Ball*> balls;
    std::vector<std::unique_ptr<Ball>> balls;
    std::vector<std::vector<std::vector<Cell>>> cells;
    std::vector<glm::vec3> storedIndices;
    unsigned int shaderProgram;
    glm::vec3 sigmaForce;

    void initCells();

    void checkCellCollision(Ball* b);

    void collide(Ball* ball1, Ball* ball2);

    void updateStoredCells(Ball* ball);

    void replaceBall(Ball* ball, glm::vec3 in);

    void generateBuffers();

    ///////////////////////////////////////////////////////////////////////////////
    // REST FOR BALL INITIALIZERS
    ///////////////////////////////////////////////////////////////////////////////

    // static functions
    static void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float normal[3]);
    static void computeVertexNormal(const float v[3], float normal[3]);
    static float computeScaleForLength(const float v[3], float length);
    static void interpolateVertex(const float v1[3], const float v2[3], float alpha, float length, float newV[3]);
    static void interpolateTexCoord(const float t1[2], const float t2[2], float alpha, float newT[2]);
    static float lerp(float from, float to, float alpha);
    static bool isSharedTexCoord(const float t[2]);
    static bool isOnLineSegment(const float a[2], const float b[2], const float c[2]);

    // member functions
    void updateRadius();
    std::vector<float> computeIcosahedronVertices();
    void buildVerticesFlat();
    void buildVerticesSmooth();
    void subdivideVerticesFlat();
    void subdivideVerticesSmooth();
    void buildInterleavedVertices();
    void addVertices(const float v1[3], const float v2[3], const float v3[3]);
    void addNormals(const float n1[3], const float n2[3], const float n3[3]);
    void addTexCoords(const float t1[2], const float t2[2], const float t3[2]);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    void addLineIndices(unsigned int i1, unsigned int i2);
    unsigned int addSubVertexAttribs(const float v[3], const float n[3], const float t[2]);

    void combineVertices();

    int subdivision;
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    std::map<std::pair<float, float>, unsigned int> sharedIndices;   // indices of shared vertices, key is tex coord (s,t)
    std::vector<float> combinedVertices;
    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;
public:
    // getters/setters
    float getRadius() const                 { return radius; }
    void setRadius(float radius);
    int getSubdivision() const              { return subdivision; }
    void setSubdivision(int subdivision);
    bool getSmooth() const                  { return smooth; }
    void setSmooth(bool smooth);
    void reverseNormals();

    // for vertex data
    unsigned int getVertexCount() const     { return (unsigned int)vertices.size() / 3; }
    unsigned int getNormalCount() const     { return (unsigned int)normals.size() / 3; }
    unsigned int getTexCoordCount() const   { return (unsigned int)texCoords.size() / 2; }
    unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }
    unsigned int getLineIndexCount() const  { return (unsigned int)lineIndices.size(); }
    unsigned int getTriangleCount() const   { return getIndexCount() / 3; }

    unsigned int getVertexSize() const      { return (unsigned int)vertices.size() * sizeof(float); }   // # of bytes
    unsigned int getNormalSize() const      { return (unsigned int)normals.size() * sizeof(float); }
    unsigned int getTexCoordSize() const    { return (unsigned int)texCoords.size() * sizeof(float); }
    unsigned int getIndexSize() const       { return (unsigned int)indices.size() * sizeof(unsigned int); }
    unsigned int getLineIndexSize() const   { return (unsigned int)lineIndices.size() * sizeof(unsigned int); }

    std::vector<float> getCombinedVertices() const { return combinedVertices; }
    std::vector<float> getVertices() const        { return vertices; }
    const float* getNormals() const         { return normals.data(); }
    const float* getTexCoords() const       { return texCoords.data(); }
    std::vector<unsigned int> getIndices() const  { return indices; }
    const unsigned int* getLineIndices() const  { return lineIndices.data(); }

    // for interleaved vertices: V/N/T
    unsigned int getInterleavedVertexCount() const  { return getVertexCount(); }    // # of vertices
    unsigned int getInterleavedVertexSize() const   { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
    int getInterleavedStride() const                { return interleavedStride; }   // should be 32 bytes
    const float* getInterleavedVertices() const     { return interleavedVertices.data(); }

    //generate buffers
    void generateSphereBuffer();
    void generateLineBuffer();
    void generatePointBuffer();
};



#endif //SOLVER_H
