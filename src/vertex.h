#ifndef VERTEX_H
#define VERTEX_H

struct vertex
{
    float x, y, z;
    float u, v;
    float nx, ny, nz;

    vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
        : x(x), y(y), z(z), u(u), v(v), nx(nx), ny(ny), nz(nz)
    {
    }

    bool operator==(const vertex& other) const {
        return x == other.x && y == other.y && z == other.z && u == other.u && v == other.v && nx == other.nx && ny == other.ny && nz == other.nz;
    }
};

namespace std {
    template <>
    struct hash<vertex> {
        size_t operator()(const vertex& v) const {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1) ^ (hash<float>()(v.z) << 2) ^ (hash<float>()(v.u) << 3) ^ (hash<float>()(v.v) << 4) ^ (hash<float>()(v.nx) << 5) ^ (hash<float>()(v.ny) << 6) ^ (hash<float>()(v.nz) << 7);
        }
    };
}

#endif