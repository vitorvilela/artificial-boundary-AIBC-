// gcc train.c -Wall -o train
// ./train

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "genann.h"


const char *variable = "f";

const char *trainX_ux = "./dataset/trainX_ux.csv";
const char *trainY_ux = "./dataset/trainY_ux.csv";
const char *trainX_uy = "./dataset/trainX_uy.csv";
const char *trainY_uy = "./dataset/trainY_uy.csv";
const char *trainX_f = "./dataset/trainX_f.csv";
const char *trainY_f = "./dataset/trainY_f.csv";




// Choose ANN architecture
const int n_input = 3;
const int n_output = 1;
const int n_hidden = 1;
const int n_neurons_per_hidden = 9;
const int epochs = 500;


double *input, *output;
int samples;

// For classification:
// const char *class_names[] = {"Iris-setosa", "Iris-versicolor", "Iris-virginica"};

void load_data() {
  
    // Choose what do you want to train
    char *trainX = trainX_ux;
    char *trainY = trainY_uy;
    
    
  
    /* Load the trainX dataset. */
    char trainX_name[80];
    sprintf(trainX_name, "%s", trainX);
    FILE *in = fopen(trainX_name, "r");
    if (!in) {
        printf("Could not open file: %s\n", trainX);
        exit(1);
    }
    
    /* Load the trainY dataset. */
    char trainY_name[80];
    sprintf(trainY_name, "%s", trainY);
    FILE *out = fopen(trainY_name, "r");
    if (!out) {
        printf("Could not open file: %s\n", trainY);
        exit(1);
    }

    /* Loop through the data to get a count. */
    char line_in[1024];
    char line_out[1024];
    while (!feof(in) && fgets(line_in, 1024, in)) {
        ++samples;
    }
    fseek(in, 0, SEEK_SET);

    printf("Loading %d data points from %s\n", samples, trainX);

    /* Allocate memory for input and output data. */
    input = malloc(sizeof(double) * samples * n_input);
    output = malloc(sizeof(double) * samples * n_output);

    
    
    // TODO Check this part again
    /* Read the file into our arrays. */
    int i, j;
    for (i = 0; i < samples; ++i) {
        double *p = input + i * n_input;
        double *c = output + i * n_output;
	c[0] = c[1] = c[2] = 0.0;

        if (fgets(line_in, 1024, in) == NULL) {
            perror("fgets");
            exit(1);
        }
        
        if (fgets(line_out, 1024, out) == NULL) {
            perror("fgets");
            exit(1);
        }

        char *split = strtok(line_in, ",");
        for (j = 0; j < n_input; ++j) {
            p[j] = atof(split);
            split = strtok(0, ",");
        }

	split = strtok(line_out, "");
	for (j = 0; j < n_output; ++j) {
            c[j] = atof(split);            
        }
	
	
	// For classification evaluation:
        // if (strcmp(split, class_names[0]) == 0) {c[0] = 1.0;}
        // else if (strcmp(split, class_names[1]) == 0) {c[1] = 1.0;}
        // else if (strcmp(split, class_names[2]) == 0) {c[2] = 1.0;}
        // else {
        //    printf("Unknown class %s.\n", split);
        //    exit(1);
        //}

        printf("Data point %d is %f %f %f ->   %f\n", i, p[0], p[1], p[2], c[0]); 
    }

    fclose(in);
    fclose(out);
}


int main(int argc, char *argv[])
{
    printf("GENANN example 4.\n");
    printf("Train an ANN on the BC dataset using backpropagation.\n");

    srand(time(0));

    /* Load the data from file. */
    load_data();

    /* n_input
     * n_hidden layer(s) of n_neurons_per_hidden
     * n_output outputs
     */
    genann *ann = genann_init(n_input, n_hidden, n_neurons_per_hidden, n_output);

    int i, j;

    /* Train the network with backpropagation. */
    printf("Training for %d epochs over data.\n", epochs);
    for (i = 0; i < epochs; ++i) {
        for (j = 0; j < samples; ++j) {
            genann_train(ann, input + j*3, output + j*1, .01);
        }
        
        /* printf("%1.2f ", xor_score(ann)); */
    }

//     int correct = 0;
//     for (j = 0; j < samples; ++j) {
//         const double *guess = genann_run(ann, input + j*3);
// 
// 	
// //         if (output[j*1+0] == 1.0) {if (guess[0] > guess[1] && guess[0] > guess[2]) ++correct;}
// //         else if (output[j*1+1] == 1.0) {if (guess[1] > guess[0] && guess[1] > guess[2]) ++correct;}
// //         else if (output[j*1+2] == 1.0) {if (guess[2] > guess[0] && guess[2] > guess[1]) ++correct;}
// //         else {printf("Logic error.\n"); exit(1);}
//     }

//     printf("%d/%d correct (%0.1f%%).\n", correct, samples, (double)correct / samples * 100.0);

    char model_name[80];
    sprintf(model_name, "./models/model-%s.ann", variable);
    
    FILE *model = fopen(model_name, "w");
    if (!model) {
        printf("Couldn't open file: %s\n", model_name);
        exit(1);
    }
    genann_write(ann, model);

    genann_free(ann);
    free(input);
    free(output);


    return 0;
}
