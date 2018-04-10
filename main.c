/* 
 * File:   main.c
 * Author: jayro
 *
 * Created on September 5, 2015, 4:52 PM
 */

#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
//#include <errno.h>
//#include <string.h>
//#include<ctype.h>

/*
 * reads integers from file, separates them into two matrices
 *then multiplies them into a 5x5 product array
 * all 3 arrays are written to the output file
 * 
 * reads from COSC450_P1_Data.txt 
 * writes to  COSC450_P1_Output.txt
 */

/*
 * kills the program if there's an error
 * 
 * message - prints this to screen before killing program
 */
void kill(const char *message){
    
    printf("ERROR: %s\n", message);
    exit(1);
}

/*
 * counts number of data entries in file( should be div by 10) and returns it
 * takes the file as a parameter
 * 
 * this probably could have been done simpler, it was done
 * when i didn't know C that well, it works so why replace it
 * 
 * input_file - reads int's from this file( quant must be divisible by 10)
 */
int get_num_of_ints(const char* input_file) {
    
    //tally of terms
    int count = 0;
    
    //holds a character which is read from file
    char c;
    
    //used for add_to_count
    int const YES = 1;
    int const NO = 0;
    
    //decides if the counter should be incremented
    int add_to_count = YES;
     
    //opens the file for read only
    FILE *myInputF = fopen( input_file, "r");
    
    //gathers a tally of signed dec int's in file
    c = getc(myInputF);
    while(c  != EOF){
       
       
        //is this char a part of a number
        if(isdigit(c) || c == '-'){
            
            
            //should i increment the tally
            if(add_to_count == YES){
               
                ++count;
                add_to_count = NO;
            }
                
        } else {
          
            add_to_count = YES;
            
        }
        
        c = getc(myInputF);
    }
    //closes the file
    fclose(myInputF);
    
    
    //validates data from the file
    if(count % 10 != 0){
        kill("number of entries not a multiple of 10.");
    }
    
    return count;
}

/*
 *passes input_file, inner value, first matrix, second matrix as params
 *creates and fills matrices
 * matrix1 - half the values go here 
 * matrix2 - other values go here
 * matrix_Prod - multiply 2 matrices together (matrix1 x martrix2)
 * inner - number of cols for matrix1 and rows for matrix2
 */

void mult_matrices(int inner, long **matrix1, long **matrix2, long **matrix_Prod){

    long sum  = 0,
        temp = 0;
    
    int i,j,h;
    
    for(h = 0; h < 5; h++){
        for( i = 0; i <5; i++){
            for(j = 0; j < inner; j++){

               temp =  matrix1[h][j];
               temp *= matrix2[j][i];

               sum += temp;
            }
               
            matrix_Prod[h][i] = sum;
            sum = 0;
        }
    }
}
/*
 * reads input file and creates two matrices to hold the data
 * then multiplies matrices to create a 5x5 product matrix
 * input_file - read int's from this file( quant must be divisable by 10)
 * matrix1 - half the values go here 
 * matrix2 - other values go here
 * matrix_Prod - multiply 2 matrices together (matrix1 x martrix2)
 * inner - number of cols for matrix1 and rows for matrix2
 */
void create_Matrices(const char *input_file,int inner, long **matrix1, long **matrix2, long **matrix_Prod){

    //used to index arrays
    int i;
    int j;
    
    
    FILE *myInputF = fopen(input_file,"r"); 
    
    for (i = 0; i < 5; i++){
        for(j =0; j < inner; j++){
            
            //if the file is empty  when it shouldn't be
            if(feof(myInputF))(kill("prematurely reached end of file"));
            
            
            //places a number into each array
            fscanf(myInputF, "%ld", &matrix1[i][ j]);
            fscanf(myInputF,"%ld", &matrix2[j][ i]);
        }
    }   
    fclose(myInputF); 
    
    //computes product of matrices [5 x inner ] * [inner x 5] = [5 x 5]
    mult_matrices(inner, matrix1, matrix2, matrix_Prod);
    
    
}

/*
 * writes the values of all three arrays to a file and debugs them for user
 * output_file - writes to this file
 * mode - w+ or a 
 * rows- rows of matrix
 * cols - columns of matrix
 * matrix - to be written to file
 * title - what the matrix will be titled in file
 */
write_to_file(const char *output_file, const char *mode, int rows , int cols, long **matrix, char *title){
    
    //writes to this file
    FILE *myOutputF = fopen( output_file, mode );
    
    //validates mode
    if(mode !="a" && mode != "w+" && mode != "w") kill("enter valid mode to write_to_file: a, w, w+,");
    
    //index array
    int i,j;
    
    fprintf(myOutputF,"\n-----   %s ------\n", title);
    printf("\n-----   %s ------\n", title);
    for(i = 0; i < rows; i++){
        for( j = 0; j < cols; j++){
            
            fprintf(myOutputF,"%ld ", matrix[i][j]);
            printf("%ld ", matrix[i][j]);
        }
        fprintf(myOutputF,"\n"); 
        printf("\n");
    }
   
    fclose(myOutputF);
}

void sort_Matrix( long **matrix, const int max_row,const int max_col){

    //holds a value tempararly 
    long temp;
    
    //max_index for "virtual" id array
     const int max_index = max_row * max_col;
    
    int index,  //used to index "virtual" 1d array
        row_1,row_2,    //points to a row pos
        col_1,col_2,    //points to a col pos
        count, count_2; //counters
    
    
    
       
    //Bubble sort adjusted for a 2d array
    for(count_2  = 0; count_2 < max_index; count_2++){
           
        for(count = 0; count < (max_index - 1); count++){
                
            row_1 = count / 5;
            row_2 = (count + 1) / 5;
            
            col_1 = count % 5;
            col_2 = (count+1) % 5;
            
            if(matrix[row_1][col_1] < matrix[row_2][col_2]){
                temp = matrix[row_1][col_1];
                matrix[row_1][col_1] = matrix[row_2][col_2];
                matrix[row_2][col_2] = temp;
                
            }
            
                
            
        }
    }
    
    
    
    
}

int main(int argc, char** argv) {
    
    // different modes for writing to file
    const char *write_to = "w+";
    const char *append_to = "a";
    
    //used for indexing matrices
    int i;
    
    //files to read and write to
    const char* input_file = "COSC450_P1_Data.txt";
    const char* output_file = "COSC450_P1_Output.txt";
    
    
    //gets the "inner" value for matrix mult
    int inner = get_num_of_ints(input_file);
    printf(" number of entries: %d\n",inner);
    inner /= 10;
 
    //first matrix
    long **matrix1 = malloc(5 * sizeof(*matrix1));
    for(i = 0; i < 5; i++){
        matrix1[i] = malloc(inner * sizeof(*matrix1[i]));
    }
    
    //second matrix
    long **matrix2 = malloc(inner * sizeof(*matrix2));
    for(i = 0; i < inner; i++){
        matrix2[i] = malloc(5 * sizeof(*matrix2[i]));
    }
    
    //product of multiplying the two matrices
    long **matrix_Prod = malloc(5 * sizeof(*matrix_Prod));
    for(i = 0; i < 5; i++){
        matrix_Prod[i] = malloc(5 * sizeof(*matrix_Prod[i]));
    }
    
    //creates two matricies from file, multiplies them to get a third
    create_Matrices(input_file, inner, matrix1, matrix2, matrix_Prod);
    
    //writes matrix to file
    write_to_file(output_file, write_to,5,inner, matrix1, "Matrix_01") ;
    write_to_file(output_file, append_to, inner,5, matrix2, "Matrix_02") ;
    write_to_file(output_file, append_to,5,5, matrix_Prod, "Matrix_Product") ;
    
    //sorts the product matrix then appends it to the file
    sort_Matrix(matrix_Prod,5,5);
    write_to_file(output_file, append_to,5,5, matrix_Prod, "Sorted_Matrix_Product") ;
    
    return (EXIT_SUCCESS);
}




