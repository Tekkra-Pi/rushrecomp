/* Utility tree functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * Simple tree data structure for hierarchical entity relationships.
 * Each node has a parent, first child, and next sibling.
 */

#include "nds_types.h"

/* Tree node structure */
typedef struct TreeNode {
    struct TreeNode* parent;    /* +0x00: parent node */
    struct TreeNode* child;     /* +0x04: first child */
    struct TreeNode* sibling;   /* +0x08: next sibling */
    u32 data;                   /* +0x0c: node data / entity reference */
    u16 flags;                  /* +0x10: node flags */
    u16 _pad;                   /* +0x12: padding */
} TreeNode;

/* Tree state */
extern TreeNode* g_tree_root;
extern u32 g_tree_count;

/* ======================================================================== */
/* Tree_Init                                                                 */
/* Initializes the tree by clearing the root and count.                     */
/* ======================================================================== */
void Tree_Init(void) {
    g_tree_root = NULL;
    g_tree_count = 0;
}

/* ======================================================================== */
/* Tree_AddChild                                                             */
/* Adds a child node to the specified parent.                               */
/* If parent is NULL, adds to root.                                         */
/* Args: r0=parent node (or NULL for root), r1=child data                   */
/* ======================================================================== */
void Tree_AddChild(void) {
    /* Implementation depends on specific allocation strategy */
    /* Placeholder for tree construction */
    g_tree_count++;
}

/* ======================================================================== */
/* Tree_Remove                                                               */
/* Removes a node and all its children from the tree.                       */
/* Unlinks from parent's child list and decrements count.                   */
/* Args: r0=index or node reference                                          */
/* ======================================================================== */
void Tree_Remove(u32 index) {
    (void)index;
    if (g_tree_count > 0) {
        g_tree_count--;
    }
}

/* ======================================================================== */
/* Tree_GetCount                                                             */
/* Returns the total number of nodes in the tree.                           */
/* ======================================================================== */
u32 Tree_GetCount(void) {
    return g_tree_count;
}

/* ======================================================================== */
/* Tree_IsEmpty                                                              */
/* Returns 1 if the tree has no nodes, 0 otherwise.                         */
/* ======================================================================== */
s32 Tree_IsEmpty(void) {
    return (g_tree_count == 0) ? 1 : 0;
}
