/**
 * Mason Mariani
 * 
 * Main file for running the MQ game. Takes 1 command line argument when running, which is the path to the mq file that will be used 
 * to create and populate the MQDecisionTree. Creates the tree, runs the game recursivley then calls MQ_free_tree to free the tree.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mq.h"

/**
 * Main logic for playing the game. Recursivley traverses the MQDecisionTree based on user input recived in 
 * response to each question along the path.
 * 
 * Reaching the end prompts several guesses as to what the answer may be.
 * 
 * Returns a 0 if game played and completed normally, and 1 if an issue was encountered.
*/
int play_game(MQDecisionTreeNode * node) {
    char line[3] = "y\n";
    if (node->yes == NULL || node->no == NULL) {
        int i = 0;
        while (i < node->num_answers) {
            //TODO check if this should print "a (object)" or "an (object)"
            printf("is it a %s? (y/n)\n", *(node->answers+i));

            fgets(line, 3, stdin);
            if (line[0] == 'y') {
                printf("I guessed it!\n");
                return 0;
            }

            i++;
        }
        printf("You got me :)\n");
        return 0;   
    }

    //guessing questions
    printf("%s (y/n)\n", node->text);
    fgets(line, 3, stdin);
    if (line[0] == 'y') {
        play_game(node->yes);
        return 0;
    }

    if (line[0] == 'n') {
        play_game(node->no);
        return 0;
    }

    return 1;    

}

int main(int argc, char * argv[]) {
    char* file_name;
    file_name = argv[1];

    //build and populate tree
    MQDecisionTree* tree = MQ_build_tree(file_name);
    MQ_populate_tree(tree, file_name);

    //play game
    MQDecisionTreeNode* node = tree->root;
    int ret_val = play_game(node);

    MQ_free_tree(tree);

    return ret_val;
}
