#include <stdio.h>

#define rate 0.01
#define threshold 0.005

struct Node {
    float w1;
    float w2;
    float b;
};

void printNode(struct Node* node, int x, int y, float actual){
    printf("(%d, %d) : %.2f  %.2f  %.2f  [ %.2f = %.2f ]\n",x, y, node->w1, node->w2, node->b, node->w1 * x + node->w2 * y + node->b, actual);
}

void update(struct Node *node, float error, int x1, int x2, int y){
    node->w1 += rate * error* x1;
    node->w2 += rate * error* x2;
    node->b += rate * error;
    printNode(node, x1, x2, y);
}

float Abs(float num){
    if(num<0)   num*=-1;
    return num;
}

float testAndUpdate(struct Node* node){
    _Bool ok=1;
    float error = -1 - (node->w1*-1+node->w2*-1+node->b);
    if(Abs(error) > threshold){
        update(node, error, -1, -1, -1);
        ok=0;
    }

    error = -1 - (node->w1*-1+node->w2*1+node->b);
    if(Abs(error) > threshold){
        update(node, error, -1, 1, -1);
        ok=0;
    }
    
    error = -1 - (node->w1*1+node->w2*-1+node->b);
    if(Abs(error)> threshold){
        update(node, error, 1, -1, -1);
        ok=0;
    }

    error = 1 - (node->w1*1+node->w2*1+node->b);
    if(Abs(error)> threshold){
        update(node, error, 1, 1, 1);
        ok=0;
    }
    return ok;
}



int main(){
    struct Node node;
    node.w1 = 0.1;
    node.w2=0.1;
    node.b=-0.1;

    int epochs = 100;
    while(epochs--)
       if(testAndUpdate(&node)) break;

    return 0;
}
