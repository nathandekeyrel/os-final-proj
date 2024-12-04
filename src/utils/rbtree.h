#ifndef RBTREE_H
#define RBTREE_H

#include <stdbool.h>
#include <stddef.h>

//======================================================
// Color Enumeration
//======================================================
typedef enum rb_color {
    RB_BLACK,
    RB_RED
} rb_color_t;

//======================================================
// Basic Node Structure
//======================================================
struct rb_node {
    struct rb_node *parent;
    struct rb_node *left;
    struct rb_node *right;
    rb_color_t color;
};

//======================================================
// Root Structure
//======================================================
struct rb_root {
    struct rb_node *node;
};

//======================================================
// Macros
//======================================================
#define RB_ROOT { NULL }  // initialize empty root

/*
 * macro to get the containing structure from a rb node
 * follows the container_of pattern used in Linux
 * ex:
 *      struct my_data {
            int key;             // offset 0
            char data[10];       // offset 4
            struct rb_node node; // offset 14
            int value;           // offset 30
        };

        say you get a pointer to rb_node inside --> struct rb_node *node_ptr = &data.node
        to get back to the data structure --> struct my_data *data_ptr = rb_entry(node_ptr, struct my_data, node);
        Steps:
            1. offsetof gets the byte offset of 'node' within my_data (14)
            2. (char *)ptr converts rb_node pointer to char pointer for byte math
            3. subtracting the offset gives the start address of the containing structure
            4. (type *) converts back to the correct pointer type

 * Reason I used this:
 *     1. I saw Linux use macros and looked into it
 *     2. It actually made sense when I was designing it:
 *         2.1 (from the example) the rb_node could be anywhere in the structure
 *         2.2 we can now have different types of structures all using the same rbtree implementation
 *         2.3 we no longer need to store extra pointers to get from the node back to our data
 *     3. Especially in our case, we will need it
 *         3.1 traversing a red black tree
 *         3.2 rbtree orders by vruntime in cfs, rbtree uses rb_node, scheduler uses process_t
 *             3.2.1 rb_entry lets us get from rb_node back to process_t
*          3.3 prob more I cba putting more examples you get the idea
 */
#define rb_entry(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))

//======================================================
// Core Red Black Tree Operations
//======================================================
void rb_insert(struct rb_node *node, struct rb_root *root, bool (*compare)(struct rb_node *, struct rb_node *));
void rb_erase(struct rb_node *node, struct rb_root *root);

//======================================================
// Tree Traversal
//======================================================
struct rb_node *rb_first(const struct rb_root *root);
// for simulation purposes, we only use the rb_first traversal
struct rb_node *rb_last(const struct rb_root *root);
struct rb_node *rb_next(const struct rb_node *node);
struct rb_node *rb_prev(const struct rb_node *node);

#endif
