#include <stdio.h>
#include <stdlib.h>

#define TREE struct tree
#define MAX_NODE_NUM 2000000


TREE                                        //structure of tree
{
    int num;
    TREE * left;
    TREE * right;
};


int max(int num1, int num2)                 //count max value of 2 variables
{
    if (num1 > num2)
        return num1;
    return num2;
}


TREE* create(int num)                       //create new element of tree
{
    TREE* result;
    result = (TREE*)malloc(sizeof(TREE));

    result->num = num;
    result->left = NULL;
    result->right = NULL;

    return result;
}


int treeheight(TREE *AVLtree)               //count height of some node
{
    if (AVLtree == NULL)
        return 0;
    int l_height = treeheight(AVLtree->left);
    int r_height = treeheight(AVLtree->right);

    return max(l_height, r_height) + 1;
}


TREE* leftrotate(TREE *AVLtree)                   //left rotation of tree
{
    TREE *treetmp1 = AVLtree->right;
    TREE *treetmp2 = treetmp1->left;

    treetmp1->left = AVLtree;
    AVLtree->right = treetmp2;

    return treetmp1;
}

TREE* rightrotate(TREE *AVLtree)                  //right rotation of tree
{
    TREE *treetmp1 = AVLtree->left;
    TREE *treetmp2 = treetmp1->right;

    treetmp1->right = AVLtree;
    AVLtree->left = treetmp2;

    return treetmp1;
}


int balance_factor(TREE *AVLtree)           //difference between node heights
{
    int l_height = treeheight(AVLtree->left);
    int r_height = treeheight(AVLtree->right);

    return l_height - r_height;
}


void balance_check(TREE **AVLtree)          //check balance and rotate tree
{
    if (*AVLtree == NULL)
        return;

    int balance_factor_node = balance_factor(*AVLtree);

    if (balance_factor_node > 1)
    {
        if (balance_factor((*AVLtree)->left) < 0)
            (*AVLtree)->left = leftrotate((*AVLtree)->left);
        *AVLtree = rightrotate(*AVLtree);
    }
    else if (balance_factor_node < -1)
    {
        if (balance_factor((*AVLtree)->right) > 0)
            (*AVLtree)->right = rightrotate((*AVLtree)->right);
        *AVLtree = leftrotate(*AVLtree);
    }

    balance_check(&((*AVLtree)->left));
    balance_check(&((*AVLtree)->right));
}


void insert(TREE **AVLtree, int num)        //insert new element of tree
{
    if (*AVLtree == NULL)
        *AVLtree = create(num);

    else if (num < (*AVLtree)->num)
        insert(&((*AVLtree)->left), num);

    else
        insert(&((*AVLtree)->right), num);

    balance_check(AVLtree);
}


void delete(TREE* AVL_tree)
{
    if (AVL_tree != NULL)
    {
        delete(AVL_tree->right);
        delete(AVL_tree->left);

        free(AVL_tree);
    }
}


void in_order(TREE *AVLtree)
{
    if (AVLtree == NULL)
        return;
    in_order(AVLtree->left);
    printf("%d\n", AVLtree->num);
    in_order(AVLtree->right);
}


void pre_order(TREE *AVLtree)
{
    if (AVLtree == NULL)
        return;
    printf("%d\n", AVLtree->num);
    pre_order(AVLtree->left);
    pre_order(AVLtree->right);
}


void post_order(TREE *AVLtree)
{
    if (AVLtree == NULL)
        return;
    post_order(AVLtree->left);
    post_order(AVLtree->right);
    printf("%d\n", AVLtree->num);
}


int main()
{
    int numelem, treeelem;
    TREE *tree = NULL;
    scanf("%d", &numelem);

    if (numelem > MAX_NODE_NUM)
        return 0;

    for (int i = 0; i < numelem; i++)
    {
        scanf("%d", &treeelem);
        insert(&tree, treeelem);
    }
    in_order(tree);
    printf("\n");

    pre_order(tree);
    printf("\n");

    post_order(tree);
    printf("\n");

    printf("%d\n", treeheight(tree));
    delete(tree);

    return 0;
}