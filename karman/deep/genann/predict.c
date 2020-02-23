#include <stdio.h>
#include <stdlib.h>
#include "genann.h"


const char *variable = "uy";



int main(int argc, char *argv[])
{
    printf("GENANN example 3.\n");
    
    char model_name[80];
    sprintf(model_name, "./models/model-%s.ann", variable);

    FILE *model = fopen(model_name, "r");
    if (!model) {
        printf("Couldn't open file: %s\n", model_name);
        exit(1);
    }

    genann *ann = genann_read(model);
    fclose(model);

    if (!ann) {
        printf("Error loading ANN from file: %s.", model_name);
        exit(1);
    }

    const double input[1][3] = {{1.e-4, 0., 0.5}};

    /* Run the network and see what it predicts. */
    printf("Output for [%1.8g, %1.8g, %1.8g] is %1.8g.\n", input[0][0], input[0][1], input[0][2], *genann_run(ann, input[0]));

    genann_free(ann);
    
    return 0;
}
