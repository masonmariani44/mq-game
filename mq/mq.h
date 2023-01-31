/**
 * Mason Mariani
 * 
 * Defines functions and structs used to create and destroy MQDecisionTrees and MQDEcisionTreeNodes
*/

#ifndef MQ_H
#define MQ_H

typedef struct MQDecisionTreeNode {
    char text[50];
    int num_answers;
    char** answers;
    struct MQDecisionTreeNode* yes;
    struct MQDecisionTreeNode* no;
} MQDecisionTreeNode;

typedef struct MQDecisionTree {
    MQDecisionTreeNode* root;
} MQDecisionTree;

/**
 * Given an MQDecisionTree pointer, prints out the contents of the MQDecisionTree
*/
void MQ_print_tree(MQDecisionTree* root);

/**
 * Given a char pointer to a file name, creates the MQDecisionTree and all fills in each question in the "text" field. 
 * 
 * Returns - pointer to the newly created MQDecisionTree
*/
MQDecisionTree* MQ_build_tree(char* file_name);

/**
 * Given a char pointer to a file name and an MQDecisionTree pointer, populates the answer fields of each MQDecisionTreeNode in the tree
*/
void MQ_populate_tree(MQDecisionTree* tree, char* file_name);

/**
 * Given an MQDecisionTree pointer, frees all memory allocated for its creation and use. Destroys the tree
*/
void MQ_free_tree(MQDecisionTree* tree);

#endif