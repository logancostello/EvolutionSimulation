#include "quad_tree.h"

bool QuadOverflow::is_full() { return count == OVERFLOW_SIZE; }; 

bool QuadNode::is_leaf() { return children[0] == -1; };
bool QuadNode::is_full() { return count == BUCKET_SIZE; };

bool QuadNode::contains(float x, float y) {
    return xmin <= x && x < xmax && ymin <= y && y < ymax;
};

QuadTree::QuadTree(float world_size_x, float world_size_y) {
    node_pool_size = 0;
    overflow_pool_size = 0;

    // Add extra space to quad tree to gracefully handle entities that exit the world bounding box
    global_xmin = -(world_size_x / 2) * 1.1;
    global_xmax = (world_size_x / 2) * 1.1;
    global_ymin = -(world_size_y / 2) * 1.1;
    global_ymax = (world_size_y / 2) * 1.1;

    root = alloc_node(global_xmin, global_xmax, global_ymin, global_ymax, 0); 
}

void QuadTree::reset() {
    node_pool_size = 0;
    overflow_pool_size = 0;
    root = alloc_node(global_xmin, global_xmax, global_ymin, global_ymax, 0);
}

int QuadTree::get_child(QuadNode& parent, float x, float y) {
    for (int i = 0; i < 4; i++) {
        int child_idx = parent.children[i];
        if (node_pool[child_idx].contains(x, y)) {
            return child_idx;
        }
    }
    return -1;
}

int QuadTree::alloc_node(float xmin, float xmax, float ymin, float ymax, int depth) {
    int index = node_pool_size++;
    QuadNode& n  = node_pool[index];

    n.xmin = xmin;
    n.xmax = xmax;
    n.ymin = ymin;
    n.ymax = ymax;
    n.count = 0;
    n.depth = depth;
    n.children[0] = -1;
    n.children[1] = -1;
    n.children[2] = -1;
    n.children[3] = -1;
    n.overflow = -1;

    return index;
};

int QuadTree::alloc_overflow() {
    int index = overflow_pool_size++;
    QuadOverflow& o = overflow_pool[index];
    o.count = 0;
    return index;
}

void QuadTree::insert(int node_idx, entt::entity entity, float x, float y) {

    QuadNode& node = node_pool[node_idx];

    if (!node.contains(x, y)) return; // Skip entities that leave world space + buffer room

    if (node.is_leaf() && !node.is_full()) {
        node.entities[node.count++] = QuadEntity(entity, x, y);
        return;
    } else if (node.is_leaf()) {
        if (node.depth <= MAX_DEPTH) divide_node(node_idx);
    } 
    // Insert into correct child
    int correct_child_idx = get_child(node, x, y);
    if (correct_child_idx >= 0) {
        insert(correct_child_idx, entity, x, y);
    } else {
        insert_overflow(node_idx, entity, x, y);
    }   
}

void QuadTree::insert_overflow(int node_idx, entt::entity entity, float x, float y) {
    QuadNode& node = node_pool[node_idx];

    if (node.overflow == -1) {
        node.overflow = alloc_overflow();
    }

    QuadOverflow& overflow = overflow_pool[node.overflow];

    if (overflow.is_full()) return; // Branch and overflow are full, dont add this entry
    overflow.entities[overflow.count++] = QuadEntity(entity, x, y);
}

void QuadTree::divide_node(int node_idx) {

    QuadNode& node = node_pool[node_idx];

    float xmid = (node.xmin + node.xmax) * 0.5f;
    float ymid = (node.ymin + node.ymax) * 0.5f;

    // Create empty children
    node.children[0] = alloc_node(node.xmin, xmid, node.ymin, ymid, node.depth+1);
    node.children[1] = alloc_node(xmid, node.xmax, node.ymin, ymid, node.depth+1);
    node.children[2] = alloc_node(node.xmin, xmid, ymid, node.ymax, node.depth+1);
    node.children[3] = alloc_node(xmid, node.xmax, ymid, node.ymax, node.depth+1);

    // Move contents to children
    for (int i = 0; i < BUCKET_SIZE; i++) {
        QuadEntity& e = node.entities[i];
        int correct_child_idx = get_child(node, e.x, e.y);
        QuadNode& child_node = node_pool[correct_child_idx];
        child_node.entities[child_node.count++] = e;
    }

    node.count = 0;
}