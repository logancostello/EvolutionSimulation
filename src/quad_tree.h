#include <entt/entt.hpp>
#include "components/components.h"
#include <cmath>

const int BUCKET_SIZE = 4;
const int MAX_DEPTH = 8;
const int MAX_NODES = 65536; // 2 ^ (2 * MAX_NODES)

struct QuadEntity {
    entt::entity entity;
    float x;
    float y;

    QuadEntity() {};
    QuadEntity(entt::entity entity, float x, float y) : entity(entity), x(x), y(y) {};
};

struct QuadNode {
    float xmin, xmax, ymin, ymax;
    int children[4];
    QuadEntity entities[BUCKET_SIZE];
    int count;
    int depth;

    bool is_leaf();
    bool is_full();
    bool contains(float x, float y); 
    
};

class QuadTree {
    public:
        QuadTree(float world_size_x, float world_size_y);
        void insert(int node_idx, entt::entity entity, float x, float y);
        void reset();

    private:
        int root;
        QuadNode node_pool[MAX_NODES];
        int node_pool_size;

        float global_xmin;
        float global_xmax;
        float global_ymin;
        float global_ymax;

        
        int alloc_node(float xmin, float xmax, float ymin, float ymax, int depth);
        void divide_node(int node_idx);
        int get_child(QuadNode&, float x, float y);
};