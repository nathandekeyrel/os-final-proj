#include "rbtree.h"

/*
 * this hole thing is based off:
 *     https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
 *     https://github.com/torvalds/linux/blob/master/lib/rbtree.c
 * we opted for a much simpler approach for the rbtree, as we are doing a
 * simulation. we focus on core rbtree requirements, as well as simpler memory
 */

//======================================================
// Helper Function
//======================================================
static void rb_set_parent(struct rb_node *node, struct rb_node *parent) {
    // this is here because I kept making types, so I extracted to a method :)
    node->parent = parent;
}

static void rb_set_color(struct rb_node *node, rb_color_t color) {
    // same story here
    node->color = color;
}

static void rotate_left(struct rb_node *node, struct rb_root *root) {
    struct rb_node *right = node->right;  // store right child
    struct rb_node *parent = node->parent;  // store parent

    /*
    before:
                                 node
                                   \
                                   right
                                  /
                              right->left
     */

    node->right = right->left;
    if (right->left) {
        rb_set_parent(right->left, node);
    }

    /*
    after:
                             node  right
                               \
                           right->left
     */

    // reattach nodes
    right->left = node;  // make node the left child of the right
    rb_set_parent(right, parent);  // right node takes this node's parent

    /*
     after:
                                right
                                /
                              node
     */

    // update parent links
    if (parent) {
        if (parent->left == node) {
            parent->left = right;  // if node was a left child
        } else {
            parent->right = right;  // if node was a right child
        }
    } else {
        root->node = right;  // if node was the root
    }

    rb_set_parent(node, right);  // set right as node's parent
}

static void rotate_right(struct rb_node *node, struct rb_root *root) {
    // this method is symmetrical to rotate_left
    struct rb_node *left = node->left;  // store left child
    struct rb_node *parent = node->parent;  // store parent

    /*
     before:
                                node
                                /
                              left
                                \
                              left->right
     */
    node->left = left->right;
    if (left->right) {
        rb_set_parent(left->right, node);
    }
    /*
     after:
                         left node
                               /
                           left->right
     */

    // reattach nodes
    left->right = node;
    rb_set_parent(left, parent);
    /*
     after:
                             left
                               \
                               node
     */

    // update parent links
    if (parent) {
        if (parent->right == node) {
            parent->right = left;  // if node was a right child
        } else {
            parent->left = left;  // if node was a left child
        }
    } else {
        root->node = left;  // if node was the root
    }

    rb_set_parent(node, left);
}

static void rb_fix_removal(struct rb_node *node, struct rb_node *parent, struct rb_root *root) {
    /*
     * removing a black node can cause violations. we keep the rbtree properties valid:
     *  1. all paths have the same number of black nodes
     *  2. red nodes can't have red children (no red->red)
     */
    struct rb_node *sibling;

    while ((!node || node->color == RB_BLACK) && node != root->node) {
        if (node == parent->left) {
            sibling = parent->right;

            /*
             * case 1: sibling is red
             *     transforms tree so node has black sibling. necessary
             *     for fixing black-height requirement in later cases
             */
            if (sibling->color == RB_RED) {
                rb_set_color(sibling, RB_BLACK);
                rb_set_color(parent, RB_RED);
                rotate_left(parent, root);
                sibling = parent->right;
            }

            /*
             * case 2: both of sibling's children are black
             *     handles violation of black-height property by pushing
             *     the problem up the tree while maintaining no red->red
             */
            if ((!sibling->left || sibling->left->color == RB_BLACK) &&
                (!sibling->right || sibling->right->color == RB_BLACK)) {
                sibling->color = RB_RED;
                node = parent;
                parent = node->parent;
            } else {
                /*
                 * case 3: sibling's right child is black
                 *     restructures tree to create case 4 while
                 *     preserving black-height and bst properties
                 */
                if (!sibling->right || sibling->right->color == RB_BLACK) {
                    if (sibling->left) {
                        sibling->left->color = RB_BLACK;
                    }
                    sibling->color = RB_RED;
                    rotate_right(sibling, root);
                    sibling = parent->right;
                }

                /*
                 * case 4: sibling's right child is red
                 *     resolves black-height violation completely by
                 *     redistributing black nodes across paths
                 */
                sibling->color = parent->color;
                rb_set_color(parent, RB_BLACK);
                if (sibling->right) {
                    sibling->right->color = RB_BLACK;
                }
                rotate_left(parent, root);
                node = root->node;
                break;
            }
        }
            // if it's not the left node, it's the right. mirror of left
        else {
            sibling = parent->left;

            // case 1
            if (sibling->color == RB_RED) {
                rb_set_color(sibling, RB_BLACK);
                rb_set_color(parent, RB_RED);
                rotate_right(parent, root);
                sibling = parent->left;
            }

            // case 2
            if ((!sibling->right || sibling->right->color == RB_BLACK) &&
                (!sibling->left || sibling->left->color == RB_BLACK)) {
                sibling->color = RB_RED;
                node = parent;
                parent = node->parent;
            } else {
                // case 3
                if (!sibling->left || sibling->left->color == RB_BLACK) {
                    if (sibling->right) {
                        sibling->right->color = RB_BLACK;
                    }
                    sibling->color = RB_RED;
                    rotate_left(sibling, root);
                    sibling = parent->left;
                }
                // case 4
                sibling->color = parent->color;
                rb_set_color(parent, RB_BLACK);
                if (sibling->left) {
                    sibling->left->color = RB_BLACK;
                }
                rotate_right(parent, root);
                node = root->node;
                break;
            }
        }
    }

    // Ensure root is black and final node (if it exists) is black
    if (node) {
        rb_set_color(node, RB_BLACK);
    }
}

//======================================================
// Core Red Black Tree Operations
//======================================================
void rb_insert(struct rb_node *node, struct rb_root *root, bool (*compare)(struct rb_node *, struct rb_node *)) {
    // insertion happens in 3 phases
    struct rb_node *parent = NULL;  // pointer to parent is null
    struct rb_node **link = &root->node;  // pointer to a pointer of the root node

    // phase 1: find insert --> traverse down tree following comparison function
    while (*link) {
        parent = *link;
        if (compare(node, parent)) {
            link = &parent->left;
        } else {
            link = &parent->right;
        }
    }

    // phase 2: link node --> new nodes are always colored red initially
    rb_set_parent(node, parent);
    node->left = node->right = NULL;
    rb_set_color(node, RB_RED);
    *link = node;

    // phase 3: fix violations --> red nodes can't have red children
    while (parent && parent->color == RB_RED) {  // only need to fix if parent is red
        struct rb_node *grandparent = parent->parent;
        if (parent == grandparent->left) {  // check if parent is a left child
            struct rb_node *uncle = grandparent->right;  // uncle is grandparent's other child (right)

            if (uncle && uncle->color == RB_RED) {
                rb_set_color(parent, RB_BLACK);  // color parent black
                rb_set_color(uncle, RB_BLACK);  // color uncle black
                rb_set_color(grandparent, RB_RED);  // color grandparent red
                node = grandparent;  // move up the tree
                parent = node->parent;
            } else {
                if (node == parent->right) {
                    /*
                     if we have a triangle:
                                            G(B)
                                            /  \
                                         P(R)  U(B)
                                            \
                                            N(R)
                     */
                    rotate_left(parent, root);
                    /*
                     after left rotation:
                                            G(B)
                                            /  \
                                         N(R)  U(B)
                                         /
                                        P(R)
                     */
                    node = parent;
                    parent = node->parent;
                }
                rb_set_color(parent, RB_BLACK);
                rb_set_color(grandparent, RB_RED);
                rotate_right(grandparent, root);
                /*
                 after right rotation:
                                            P(B)
                                            /  \
                                         N(R)  G(R)
                                                 \
                                                 U(B)
                 */
            }
        } else {  // symmetric for grandparent's right child
            struct rb_node *uncle = grandparent->left;

            if (uncle && uncle->color == RB_RED) {
                rb_set_color(parent, RB_BLACK);
                rb_set_color(uncle, RB_BLACK);
                rb_set_color(grandparent, RB_RED);
                node = grandparent;
                parent = node->parent;
            } else {
                if (node == parent->left) {
                    rotate_right(parent, root);
                    node = parent;
                    parent = node->parent;
                }
                rb_set_color(parent, RB_BLACK);
                rb_set_color(grandparent, RB_RED);
                rotate_left(grandparent, root);
            }
        }
    }

    rb_set_color(root->node, RB_BLACK);
}

void rb_erase(struct rb_node *node, struct rb_root *root) {
    struct rb_node *child, *parent;
    rb_color_t color;

    if (!node->left) {  // nil left node
        child = node->right;
    } else if (!node->right) {  // nil right node
        child = node->left;
    } else {  // node has 2 children
        struct rb_node *successor = node->right;
        while (successor->left) {
            successor = successor->left;
        }

        // save successor's info
        color = successor->color;
        child = successor->right;
        parent = successor->parent;

        if (parent == node) {  // successor is direct right child
            parent = successor;
        } else {  // successor is further down
            if (child) {
                rb_set_parent(child, parent);
            }

            parent->left = child;
            successor->right = node->right;
            node->right->parent = successor;
        }

        // put successor in node's place
        rb_set_color(successor, node->color);
        successor->left = node->left;
        node->left->parent = successor;
        successor->parent = node->parent;

        if (node->parent) {
            if (node == node->parent->left) {
                node->parent->left = successor;
            } else {
                node->parent->right = successor;
            }
        } else {
            root->node = successor;
        }

        goto color_fix;  // for all cases check if color needs fixing
    }

    parent = node->parent;
    color = node->color;

    if (child) {
        rb_set_parent(child, parent);
    }

    if (parent) {
        if (node == parent->left) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        root->node = child;
    }

    color_fix:
    if (color == RB_BLACK) {  // we only need to fix tree if we removed a black node
        rb_fix_removal(child, parent, root);
    }
}

//======================================================
// Tree Traversal
//======================================================
struct rb_node *rb_first(const struct rb_root *root) {
    // smallest node by going as left as possible
    struct rb_node *n = root->node;
    if (!n) return NULL;
    while (n->left) n = n->left;
    return n;
}

struct rb_node *rb_last(const struct rb_root *root) {
    // largest node by going as right as possible
    struct rb_node *n = root->node;
    if (!n) return NULL;
    while (n->right) n = n->right;
    return n;
}

struct rb_node *rb_next(const struct rb_node *node) {
    // node has right child, get leftmost of right subtree
    if (node->right) {
        node = node->right;
        while (node->left) node = node->left;
        return (struct rb_node *) node;
    }

    // no right child, go up until we find first right turn
    while (node->parent && node == node->parent->right) node = node->parent;
    return node->parent;
}

struct rb_node *rb_prev(const struct rb_node *node) {
    // node has left child, get rightmost of left subtree
    if (node->left) {
        node = node->left;
        while (node->right) node = node->right;
        return (struct rb_node *) node;
    }

    // no left child, go up until we find the first left turn
    while (node->parent && node == node->parent->left) node = node->parent;
    return node->parent;
}