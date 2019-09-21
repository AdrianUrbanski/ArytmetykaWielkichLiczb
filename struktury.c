#include "struktury.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

pnode getNode(token val){
	pnode node = malloc(sizeof(snode));
	node -> value = val;
	node -> left = NULL;
	node -> right = NULL;
	return node;
}

void push(token val, pnode* node){
	if((*node)==NULL){
		*node=getNode(val);
		return;
	}
	pnode temp = getNode(val);
	(*node) -> right = temp;
	temp -> left = (*node);
	*node = temp;
	return;
}

token get_front(pnode* node){
	token result = (*node) -> value;
	if((*node)->right!=NULL){
		*node = (*node)->right;
		free((*node)->left);
		(*node)->left=NULL;
	}
	else{
		free(*node);
		*node = NULL;
	}
	return result;
}

token get_top(pnode* node){
	token result = (*node) -> value;
	if((*node)->left!=NULL){
		*node = (*node)->left;
		free((*node)->right);
		(*node)->right=NULL;
	}
	else{
		free(*node);
		*node = NULL;
	}
	return result;
}

token top(pnode node){
	return node->value;
}

pnode find_front(pnode node){
	while(node->left!=NULL)
		node = node->left;
	return node;
}
