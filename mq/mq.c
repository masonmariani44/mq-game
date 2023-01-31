#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mq.h"

void MQ_print_tree_helper(MQDecisionTreeNode* node, int level) {
    if (node->yes == NULL || node->no == NULL) {
        int i = 0;
        int run = 0;
        while (i < node->num_answers) {
            printf("| %s ", *(node->answers+i));
            run = 1;
            i++;
        }
        if (run == 1) {
            printf("|\n");
        }
        if (run == 0) {
            printf("\n");
        }
        return;
    }

    //print cur
    printf("[");
    printf("%s", node->text);
    printf("]\n");

    //print yes recurse
    for (int i=0; i < level; i++) {
        printf("    ");
    }
    printf("-y-> ");
    MQ_print_tree_helper(node->yes, level+1);

    //print no recurse
    for (int i=0; i < level; i++) {
        printf("    ");
    }
    printf("-n-> ");
    MQ_print_tree_helper(node->no, level+1);
}

void MQ_print_tree(MQDecisionTree* root){
    MQ_print_tree_helper(root->root, 0);
}

void MQ_build_tree_helper(MQDecisionTreeNode* node, char * questions[], int q_index, int max_questions) {
    if (q_index == max_questions) {
        node->yes = NULL;
        node->no = NULL;
        node->num_answers = 0;
        node->answers = NULL;
        return;
    }

    //add question to text
    for (int i = 0; i < 50; i++) {
	node->text[i] = '\0';
    }
    char * str_to_add = *(questions+q_index);
    strcpy(node->text, str_to_add);
    for (int i = 0; i < 50; i++) {
    	if (node->text[i] == '\r') {
		node->text[i] = '\0';
	}
    }

    //recurse into yes and no
    MQDecisionTreeNode* yes_tree = malloc(sizeof(MQDecisionTreeNode));
    node->yes = yes_tree;
    MQ_build_tree_helper(yes_tree, questions, q_index+1, max_questions);
    MQDecisionTreeNode* no_tree = malloc(sizeof(MQDecisionTreeNode));
    node->no = no_tree;
    MQ_build_tree_helper(no_tree, questions, q_index+1, max_questions);
}

MQDecisionTree* MQ_build_tree(char* file_name){
    MQDecisionTree* ret_tree = malloc(sizeof(MQDecisionTree));
    MQDecisionTreeNode* cur_node = malloc(sizeof(MQDecisionTreeNode));
    ret_tree->root = cur_node;
    FILE* mq_file_inst_1;
    FILE* mq_file_inst_2;
    mq_file_inst_1 = fopen(file_name, "r");
    mq_file_inst_2 = fopen(file_name, "r");
	if (mq_file_inst_1 == NULL) {
		fprintf(stderr, "error opening the file.\n");
	}

    //get number of questions for this tree
    int q_counter = 0;
    while (1 == 1) {
        char character = (char) fgetc(mq_file_inst_1);
        if (character == '\n') {
            while (1 == 1) {
                character = (char) fgetc(mq_file_inst_1);
                if (character == '\n') {break;}
                if (character == '?') {q_counter++;}
            }
            break;
        }
    }
	fflush(mq_file_inst_1);
	fclose(mq_file_inst_1);

    //get char* array of questions
    char line[(50*q_counter)+q_counter];
    fgets(line, (50*q_counter)+q_counter, mq_file_inst_2);
    char* questions[q_counter];
    int i = 0;
    char * token = strtok(fgets(line, (50*q_counter)+q_counter, mq_file_inst_2), ",");
    while(token != NULL) {
        questions[i] = token;
        token = strtok(NULL, ",");
        i++;
    }
    //strip off \n char from last string
    char* last_str = questions[q_counter-1];
    *(last_str+strlen(last_str)-1) = '\0';

    MQ_build_tree_helper(cur_node, questions, 0, q_counter);
    
	fflush(mq_file_inst_2);
	fclose(mq_file_inst_2);

    return ret_tree;
}



void MQ_populate_tree_helper(MQDecisionTreeNode * node, char * answer, int answer_path[], int ans_path_i, int total_ans){
    if (node->yes == NULL || node->no == NULL) {
        //set answer
        if (node->num_answers == 0) {
            node->answers = (char**) malloc(sizeof(char*) * total_ans + 1);
        }

        char * new_ans;
        new_ans = (char*) malloc((strlen(answer))+1);
        strcpy(new_ans, answer);
        *(node->answers+(node->num_answers)) = new_ans;

        node->num_answers = node->num_answers+1;

        return;
    }

    if (answer_path[ans_path_i] == 1) {
        MQ_populate_tree_helper(node->yes, answer, answer_path, ans_path_i+1, total_ans);
    }
    else {
        MQ_populate_tree_helper(node->no, answer, answer_path, ans_path_i+1, total_ans);
    }
}

void MQ_populate_tree(MQDecisionTree* tree, char* file_name){
    FILE* mq_file_inst_1;
    FILE* mq_file_inst_2;
	mq_file_inst_1 = fopen(file_name, "r");
    mq_file_inst_2 = fopen(file_name, "r");
	if (mq_file_inst_1 == NULL) {
		fprintf(stderr, "error opening the file.\n");
	}

    MQDecisionTreeNode* cur_node = tree->root;

    //get number of questions for this tree (realizing now that this is less efficient than just counting one of the next lines)
    int q_counter = 0;
    while (1 == 1) {
        char character = (char) fgetc(mq_file_inst_1);
        if (character == '\n') {
            while (1 == 1) {
                character = (char) fgetc(mq_file_inst_1);
                if (character == '\n') {break;}
                if (character == '?') {q_counter++;}
            }
            break;
        }
    }
	fflush(mq_file_inst_1);
	fclose(mq_file_inst_1);

    //get char* array of questions
    char line[(50*q_counter)+q_counter];
    fgets(line, (50*q_counter)+q_counter, mq_file_inst_2);
    int num_answers = atoi(line);
    fgets(line, (50*q_counter)+q_counter, mq_file_inst_2);

    int answer_path[q_counter+1];
    char answer[50];
    int i = 0;
    int j = 0;
    int num_to_add;
    while (i < num_answers) {
        j = 0;
        char * token = strtok(fgets(line, (50*q_counter)+q_counter, mq_file_inst_2), ",");
        strcpy(answer, token);
        while(token != NULL) {
            token = strtok(NULL, ",");
	    if (token != NULL) {	
	    	num_to_add = atoi(token);
		answer_path[j] = num_to_add;
	    }
            j++;
        }
        i++;
        
        MQ_populate_tree_helper(cur_node, answer, answer_path, 0, num_answers);
    }

    fflush(mq_file_inst_2);
	fclose(mq_file_inst_2);
}

void MQ_free_tree_helper(MQDecisionTreeNode* node) {
	if (node->yes == NULL || node->no == NULL) {
        int run = 0;
		for (int i = 0; i < node->num_answers; i++) {
			free(*(node->answers+i));
            run = 1;
		}
        if (run == 1) {
            free(node->answers);
        }
		free(node);
		return;
	}
	MQ_free_tree_helper(node->yes);
	MQ_free_tree_helper(node->no);
	free(node);
}

void MQ_free_tree(MQDecisionTree* tree){
	MQ_free_tree_helper(tree->root);

	free(tree);
}
