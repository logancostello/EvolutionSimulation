#pragma once
#include <entt/entt.hpp>
#include "components/components.h"
#include <cmath>

const int BUCKET_SIZE = 4;
const int OVERFLOW_SIZE = 16;

const int MAX_DEPTH = 8;
const int MAX_NODES = 65536; // 2 ^ (2 * MAX_NODES)
const int MAX_OVERFLOW = 100;

enum class EntityTag : uint8_t {
    Any, // not meant to be assigned, but to be used when querying
    Plant,
    Creature,
    Carcass
};

struct QuadEntity {
    entt::entity entity;
    float x;
    float y;
    EntityTag tag;

    QuadEntity() {};
    QuadEntity(entt::entity entity, float x, float y, EntityTag tag) : entity(entity), x(x), y(y), tag(tag) {};
};

struct QuadOverflow {
    QuadEntity entities[16];
    int count;

    bool is_full();
};

struct QuadNode {
    float xmin, xmax, ymin, ymax;
    int children[4];
    QuadEntity entities[BUCKET_SIZE];
    int count;
    int depth;
    int overflow;

    bool is_leaf();
    bool is_full();
    bool contains(float x, float y); 
    bool intersects_circle(float x, float y, float r);
    bool intersects_circle_sqr(float x, float y, float r_sqr);
    
};

class QuadTree {
    public:
        QuadTree(float world_size_x, float world_size_y);
        void insert(int node_idx, entt::entity entity, float x, float y, EntityTag tag);
        void reset();
        void update_bounds(float x, float y);
        void query(float x, float y, float radius, std::vector<entt::entity>& out);
        entt::entity query_closest(entt::entity self, float x, float y, float max_dist, EntityTag tag = EntityTag::Any);

    private:
        int root;

        QuadNode node_pool[MAX_NODES];
        int node_pool_size;

        QuadOverflow overflow_pool[MAX_OVERFLOW];
        int overflow_pool_size;

        float global_xmin;
        float global_xmax;
        float global_ymin;
        float global_ymax;

        void insert_overflow(int node_idx, entt::entity entity, float x, float y, EntityTag tag);
        int alloc_node(float xmin, float xmax, float ymin, float ymax, int depth);
        int alloc_overflow();
        void divide_node(int node_idx);
        int get_child(QuadNode&, float x, float y);
        void query_node(int node_idx, float x, float y, float radius, std::vector<entt::entity>& out); 
        void collect_leaf(int node_idx, std::vector<entt::entity>& out);
};