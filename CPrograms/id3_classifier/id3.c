#include <stdio.h>

char* load_dataset(){
    FILE *file = fopen("./car_evaluation.csv", "r");
    
    if(file == NULL){
        printf("Error reading file");
        return (void *)-1;
    }
}

int main(){
     
     
    return 0;
}
