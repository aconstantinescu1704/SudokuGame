#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp_header.h"
#include "cJSON.c"


#define CONTUR_ALB 1
#define CONTUR_GRI 1
#define CONTUR_NEGRU 1
#define NR_SUDOKU_CELLS 9

#define CODIF1 "0000*0000*0000*0000*0000*"
#define CODIF2 "*****0000*******0000*****"
#define CODIF3 "*****0000******0000******"
#define CODIF4 "*000**000******0000*0000*"
#define CODIF5 "******0000*****0000******"
#define CODIF6 "******0000******000******"
#define CODIF7 "*****0000*0000*0000*0000*"
#define CODIF8 "******000*******000******"
#define CODIF9 "******000******0000******"


char* CovertInputtoString(FILE* finput)
{
    char* sir;
    long long int lung;
    fseek(finput, 0, SEEK_END);
    lung = ftell(finput);
    fseek(finput, 0, SEEK_SET);

    sir = calloc(lung + 1, sizeof(char));
    
    fread(sir, sizeof(char), 1 + lung, finput);

    return sir;
}

char* Name_Output1( char* input_name)
{
    char copie[100];
    strcpy(copie, input_name);
    char* fileName = calloc(60, sizeof(char));
    char *p = strtok(copie, "./");

    strcat(fileName, "output_task1");
    strcat(fileName, "_");

    while(p != NULL )
    {
        if(strstr(p, "board") != NULL)
            strcat(fileName, p);

        p = strtok(NULL, "./");
        
    }

    strcat(fileName, ".bmp");

    return fileName;
}

char* Name_Output2( char* input_name)
{
    char copie[100];
    strcpy(copie, input_name);
    char* fileName = calloc(60, sizeof(char));
    char *p = strtok(copie, "./");

    strcat(fileName, "output_task2");
    strcat(fileName, "_");

    while(p != NULL )
    {
        if(strstr(p, "board") != NULL)
            strcat(fileName, p);

        p = strtok(NULL, "./");
        
    }

    strcat(fileName, ".bmp");

    return fileName;
}

char* Name_Output3(char* input_name)
{
    char copie[100];
    strcpy(copie, input_name); 
    char* fileName = calloc(60, sizeof(char));
    char *p = strtok(copie, "./");

    strcat(fileName, "output_task3");
    strcat(fileName, "_");

    while(p != NULL )
    {
        if(strstr(p, "board") != NULL)
            strcat(fileName, p);

        p = strtok(NULL, "./");
        
    }

    strcat(fileName, ".json");

    return fileName;
}

char* Name_Output4(char* input_name)
{
    char copie[100];
    strcpy(copie, input_name); 
    char* fileName = calloc(60, sizeof(char));
    char *p = strtok(copie, "./");

    strcat(fileName, "output_task4");
    strcat(fileName, "_");

    while(p != NULL )
    {
        if(strstr(p, "board") != NULL)
            strcat(fileName, p);

        p = strtok(NULL, "./");
        
    }

    strcat(fileName, ".bmp");

    return fileName;
}


char* Board_Name(char* input_name)
{
    char copie[100];
    strcpy(copie, input_name); 
    char* fileName = calloc(60, sizeof(char));
    char *p = strtok(copie, "./");

    while(p != NULL )
    {
        if(strstr(p, "board") != NULL)
            strcpy(fileName, p);

        p = strtok(NULL, "./");
        
    }

    strcat(fileName, ".json");

    return fileName;

}

void ReadDataHeader(cJSON *json, bmp_fileheader* bitImage)
{

    cJSON *file_Header = cJSON_GetObjectItem(json, "file_header");

    cJSON *signature = cJSON_GetObjectItem(file_Header, "signature"); 
    bitImage->fileMarker1 = (signature->valuestring)[0];
    bitImage->fileMarker2 = (signature->valuestring)[1];

    cJSON *file_size = cJSON_GetObjectItem(file_Header, "file_size"); 
    bitImage->bfSize = file_size->valueint;

    cJSON *reserved = cJSON_GetObjectItem(file_Header, "reserved"); 
    bitImage->unused1 =  reserved->valueint;
    bitImage->unused2 = reserved->valueint;

    cJSON *offset = cJSON_GetObjectItem(file_Header, "offset"); 
    bitImage->imageDataOffset = offset->valueint;

}

void ReadDataInfo(cJSON *json, bmp_infoheader* bitImage)
{
    cJSON *info_Header = cJSON_GetObjectItem(json, "info_header");

    cJSON *size = cJSON_GetObjectItem(info_Header, "size");
    bitImage->biSize = size->valueint;

    cJSON *width = cJSON_GetObjectItem(info_Header, "width");
    bitImage->width = width->valueint;

    cJSON *height = cJSON_GetObjectItem(info_Header, "height");
    bitImage->height = height->valueint;

    cJSON *planes = cJSON_GetObjectItem(info_Header, "planes");
    bitImage->planes = planes->valueint;

    cJSON *bit_count = cJSON_GetObjectItem(info_Header, "bit_count");
    bitImage->bitPix = bit_count->valueint;

    cJSON *compression = cJSON_GetObjectItem(info_Header, "compression");
    bitImage->biCompression = compression->valueint;

    cJSON *image_size = cJSON_GetObjectItem(info_Header, "image_size");
    bitImage->biSizeImage = image_size->valueint;

    cJSON *x_pixels_per_meter = cJSON_GetObjectItem(info_Header, "x_pixels_per_meter");
    bitImage->biXPelsPerMeter = x_pixels_per_meter->valueint;

    cJSON *y_pixels_per_meter = cJSON_GetObjectItem(info_Header, "y_pixels_per_meter");
    bitImage->biYPelsPerMeter = y_pixels_per_meter->valueint;

    cJSON *colors_used = cJSON_GetObjectItem(info_Header, "colors_used");
    bitImage->biClrUsed = colors_used->valueint;

    cJSON *colors_important = cJSON_GetObjectItem(info_Header, "colors_important");
    bitImage->biClrImportant = colors_important->valueint;

}

unsigned char** ReadBitMap(cJSON *json, bmp_infoheader *bitImage)
{
    unsigned char **map = calloc(73, sizeof(char *));
    cJSON *bitMap = cJSON_GetObjectItem(json, "bitmap");
    int j, k = 0;
    int padding = 4 - (((bitImage->width) * (bitImage->bitPix)) / 8)%4;
    //formam matricea de pixeli
    for(int i = 0; i < bitImage->height; i++)
    {
        map[i] = calloc(73 * 3 + 73, sizeof(char));
        for( j = 0; j < ((bitImage->width) * (bitImage->bitPix)) / 8 ; j++)
        {
            cJSON *bit_blue = cJSON_GetArrayItem(bitMap, k);
            k++;
            map[i][j] = (char)(bit_blue->valueint);
        }

        for( j = 0; j < padding; j++)
            map[i][j + ((bitImage->width) * (bitImage->bitPix)) / 8] = 0;

    }

    return map;
}

void SwapCell(unsigned char **bitmap, int dim_cell, int index_cell_linie, int index_cell_coloana)
{
    int index_inceput_linie_pixel  = (CONTUR_ALB + CONTUR_NEGRU) + dim_cell * index_cell_linie + (CONTUR_NEGRU + CONTUR_GRI + CONTUR_ALB) * index_cell_linie;
    int index_inceput_coloana_pixel = (CONTUR_ALB + CONTUR_NEGRU) + dim_cell * index_cell_coloana + (CONTUR_NEGRU + CONTUR_GRI + CONTUR_ALB) * index_cell_coloana;

    int index_inceput_linie_byte =  index_inceput_linie_pixel;
    int index_inceput_coloana_byte = 3 * index_inceput_coloana_pixel;


    for(int i = index_inceput_linie_byte; i < index_inceput_linie_byte + dim_cell; i++)
    {
        char aux_r , aux_g , aux_b ;

        // interschimbam primul si ultimul pixel de pe fiecare linie din matricea de 5X5
    
        aux_b = bitmap[i][0 + index_inceput_coloana_byte];
        bitmap[i][0 + index_inceput_coloana_byte] = bitmap[i][(dim_cell - 1) * 3 + index_inceput_coloana_byte];
        bitmap[i][(dim_cell - 1) * 3 + index_inceput_coloana_byte] = aux_b;

        aux_g = bitmap[i][1 + index_inceput_coloana_byte];
        bitmap[i][1 + index_inceput_coloana_byte] = bitmap[i][(dim_cell - 1) * 3 + 1 + index_inceput_coloana_byte];
        bitmap[i][(dim_cell - 1) * 3 + 1 + index_inceput_coloana_byte] = aux_g;

        aux_r = bitmap[i][2 + index_inceput_coloana_byte];
        bitmap[i][2 + index_inceput_coloana_byte] = bitmap[i][(dim_cell - 1) * 3 + 2 + index_inceput_coloana_byte];
        bitmap[i][(dim_cell - 1) * 3 + 2 + index_inceput_coloana_byte] = aux_r;
            
    }
    

}

void MapSwap( unsigned char **bitMap)
{
    int dim_cell = 5; //dimensiunea celulei fara contur 

    for(int i = 0; i < NR_SUDOKU_CELLS; i++)
        for(int j = 0; j < NR_SUDOKU_CELLS; j++)
            SwapCell(bitMap, dim_cell, i, j);

}

int NumberInLine(int number, int index_linie, int **mat_pseudo)
{
    int count = 0;

    for(int j = 0; j < NR_SUDOKU_CELLS; j++)
        if(mat_pseudo[index_linie][j] == number)
            count++;

    return count;
}

int NumberInColumn(int number, int index_column, int **mat_pseudo)
{
    int count = 0;
    
    for(int i = 0; i < NR_SUDOKU_CELLS; i++)
        if(mat_pseudo[i][index_column] == number)
            count++;

    return count;
}

int NumberInBox(int number, int index_cell, int **mat_pseudo)
{
    int count = 0;
    int inceput_coloana_cell = ((index_cell ) % 3) * 3;
    int inceput_linie_cell = ((index_cell ) / 3) * 3;

    for(int i = inceput_linie_cell; i < inceput_linie_cell + 3; i++)
        for(int j = inceput_coloana_cell; j < inceput_coloana_cell + 3; j++)
            if(mat_pseudo[i][j] == number)
                count++;
    
    return count;
}

 void FindNumberCell(unsigned char **bitmap, int dim_cell, int index_cell_linie, int index_cell_coloana, int** mat_pseudo)
{
    int index_inceput_linie_byte = 2 + dim_cell * index_cell_linie + 3 * index_cell_linie; 
    int index_inceput_coloana_byte = 3 * (2 + dim_cell * index_cell_coloana + 3 * index_cell_coloana);

    //fiecare reprezentare a cifrei in formatul dat este unic definita de 5 biti si in functie de configuratiile lor
    //definim cifra corespunzatoare secventei de biti 

    unsigned char bit0 = bitmap[index_inceput_linie_byte][index_inceput_coloana_byte + 1];
    unsigned char bit1 = bitmap[index_inceput_linie_byte + 1][index_inceput_coloana_byte + 1];
    unsigned char bit2 = bitmap[index_inceput_linie_byte + 1][4 * 3 + index_inceput_coloana_byte + 1];
    unsigned char bit3 = bitmap[index_inceput_linie_byte + 3][index_inceput_coloana_byte + 1];
    unsigned char bit5 = bitmap[index_inceput_linie_byte + 4][index_inceput_coloana_byte + 1];



    if(bit0 == 255)
       {
            if(bit2 == 255)
                mat_pseudo[index_cell_linie][index_cell_coloana] = 0;
            else
                mat_pseudo[index_cell_linie][index_cell_coloana] = 1;
       } 
    else
    {
        if(bit1 == 255)
        {
            if(bit3 == 175)
                mat_pseudo[index_cell_linie][index_cell_coloana] = 2;
            else
            {
                if(bit5 == 175)
                    mat_pseudo[index_cell_linie][index_cell_coloana] = 3;
                else
                    mat_pseudo[index_cell_linie][index_cell_coloana] = 7;
            }
        }
        else
        {
            if(bit2 == 255)
            {   
                if(bit3 == 255)
                    mat_pseudo[index_cell_linie][index_cell_coloana] = 5;
                else
                    mat_pseudo[index_cell_linie][index_cell_coloana] = 6;
            }
            else
            {
                if(bit3 == 175) 
                    mat_pseudo[index_cell_linie][index_cell_coloana] = 8;
            
                else
                {
                    if(bit5 == 255)
                        mat_pseudo[index_cell_linie][index_cell_coloana] = 4;
                    else
                        mat_pseudo[index_cell_linie][index_cell_coloana] = 9;
                }
            }
        }
    }
  

}

char* CheckMap( unsigned char **bitMap, int **mat_pseudo)
{
    int dim_cell = 5;
    int ok = 1;
    char* output = calloc(10, sizeof(char)); 

    for(int i = 0; i < NR_SUDOKU_CELLS; i++)
        for(int j = 0; j < NR_SUDOKU_CELLS; j++)
            FindNumberCell(bitMap, dim_cell, i, j, mat_pseudo);


    //pt ca jocul sa fie castigat fiecare nr de la 1 la 9 trebuie sa se gaseasca pe linie, coloana, si cutie o singura data
    for(int nr = 1; nr <= 9; nr++)
    {
        for(int k = 0; k < NR_SUDOKU_CELLS; k++)
        {
            int nr_aparitii_linie = NumberInLine(nr, k, mat_pseudo);
            int nr_aparitii_coloana = NumberInColumn(nr, k, mat_pseudo);
            int nr_aparitii_celula = NumberInBox(nr, k, mat_pseudo);
    
         
            if(nr_aparitii_linie != 1)
                ok = 0;
            if(nr_aparitii_coloana != 1)
                ok = 0;
            if(nr_aparitii_celula != 1)
                ok =  0;  

        }

    }

    if(ok == 1)
        strcpy(output, "Win!");
    else
        strcpy(output, "Loss :(");
    
    return output;

}

void FreeMap(bmp_infoheader *bitImageI, unsigned char **bit_map)
{
    for(int i = 0; i < bitImageI->height; i++)
       free(bit_map[i]);

    free(bit_map);
}

void ReadFromBitmap(FILE *finput, unsigned char **bitmap, bmp_infoheader *bitImageI, bmp_fileheader* bitImageH)
{
    fread(&(bitImageH->fileMarker1), sizeof(unsigned char), 1, finput);
    fread(&(bitImageH->fileMarker2), sizeof(unsigned char), 1, finput);

    fread(&(bitImageH->bfSize), sizeof(unsigned int), 1, finput);

    fread(&(bitImageH->unused1), sizeof(unsigned short), 1, finput);
    fread(&(bitImageH->unused2), sizeof(unsigned short), 1, finput);

    fread(&(bitImageH->imageDataOffset), sizeof(unsigned int), 1, finput);

    fread(&(bitImageI->biSize), sizeof(unsigned int), 1, finput);
    fread(&(bitImageI->width), sizeof(signed int), 1, finput);
    fread(&(bitImageI->height), sizeof(signed int), 1, finput);
    fread(&(bitImageI->planes), sizeof(unsigned short), 1, finput);
    fread(&(bitImageI->bitPix), sizeof(unsigned short), 1, finput);
    fread(&(bitImageI->biCompression), sizeof(unsigned int), 1, finput);
    fread(&(bitImageI->biSizeImage), sizeof(unsigned int), 1, finput);
    fread(&(bitImageI->biXPelsPerMeter), sizeof(int), 1, finput);
    fread(&(bitImageI->biYPelsPerMeter), sizeof(int), 1, finput);
    fread(&(bitImageI->biClrUsed), sizeof(unsigned int), 1, finput);
    fread(&(bitImageI->biClrImportant), sizeof(unsigned int), 1, finput);

    for(int i = bitImageI->height - 1; i >= 0; i--)
        fread(bitmap[i], ((bitImageI->width) * (bitImageI->bitPix)) / 8 + 1, 1, finput);


}

void WriteNewBmp(FILE *foutput, unsigned char **bitmap, bmp_infoheader *bitImageI, bmp_fileheader* bitImageH)
{
    fwrite(&(bitImageH->fileMarker1), 1, sizeof(unsigned char), foutput);
    fwrite(&(bitImageH->fileMarker2), 1, sizeof(unsigned char), foutput);

    fwrite(&(bitImageH->bfSize), 1, sizeof(unsigned int), foutput);
    fwrite(&(bitImageH->unused1), 1, sizeof(unsigned short), foutput);
    fwrite(&(bitImageH->unused2), 1, sizeof(unsigned short), foutput);
    fwrite(&(bitImageH->imageDataOffset), 1, sizeof(unsigned int), foutput);

    fwrite(&(bitImageI->biSize), 1, sizeof(unsigned int), foutput);

    fwrite(&(bitImageI->width), 1, sizeof(signed int), foutput);

    fwrite(&(bitImageI->height), 1, sizeof(signed int), foutput);

    fwrite(&(bitImageI->planes), 1, sizeof(unsigned short), foutput);

    fwrite(&(bitImageI->bitPix), 1, sizeof(unsigned short), foutput);

    fwrite(&(bitImageI->biCompression), 1, sizeof(unsigned int), foutput);

    fwrite(&(bitImageI->biSizeImage), 1, sizeof(unsigned int), foutput);

    fwrite(&(bitImageI->biXPelsPerMeter), 1, sizeof(int), foutput);

    fwrite(&(bitImageI->biYPelsPerMeter), 1, sizeof(int), foutput);

    fwrite(&(bitImageI->biClrUsed), 1, sizeof(unsigned int), foutput);

    fwrite(&(bitImageI->biClrImportant), 1, sizeof(unsigned int), foutput);

    for(int i = bitImageI->height - 1; i >= 0; i--)
       fwrite(bitmap[i], 1, ((bitImageI->width) * (bitImageI->bitPix)) / 8 + 1, foutput);
}

void FillCell(int index_cell_linie, int index_cell_coloana, unsigned char **bitmap, int nr, int dim_cell)
{
    int index_inceput_linie_byte = 2 + dim_cell * index_cell_linie + 3 * index_cell_linie; 
    int index_inceput_coloana_byte = 3 * (2 + dim_cell * index_cell_coloana + 3 * index_cell_coloana);

    char* completare = calloc(dim_cell * dim_cell + 1, sizeof(char));


    if(nr == 1)
        strcpy(completare, CODIF1);
    if(nr == 2)
        strcpy(completare, CODIF2);
    if(nr == 3)
        strcpy(completare, CODIF3);
    if(nr == 4)
        strcpy(completare, CODIF4);
    if(nr == 5)
        strcpy(completare, CODIF5);
    if(nr == 6)
        strcpy(completare, CODIF6);
    if(nr == 7)
        strcpy(completare, CODIF7);
    if(nr == 8)
        strcpy(completare, CODIF8);
    if(nr == 9)
        strcpy(completare, CODIF9);

    int k = 0;

    for(int i = index_inceput_linie_byte; i < index_inceput_linie_byte + dim_cell; i++)
        for(int j = index_inceput_coloana_byte; j < index_inceput_coloana_byte + 3 * dim_cell; j = j + 3)
        {
            if(completare[k] == '*')
            {
                bitmap[i][j] = 255;
                bitmap[i][j + 1] = 0;
                bitmap[i][j + 2] = 255;
            }
          k++;
        }
    free(completare);

}


void SolveSudoku( unsigned char **bitMap, int **mat_pseudo)
{
    int dim_cell = 5;
    
    for(int i = 0; i < NR_SUDOKU_CELLS; i++)
        for(int j = 0; j < NR_SUDOKU_CELLS; j++)
            FindNumberCell(bitMap, dim_cell, i, j, mat_pseudo);

    
    for(int i = 0; i < NR_SUDOKU_CELLS;  i++)
        for(int j = 0; j < NR_SUDOKU_CELLS; j++)
        {
            //daca gasim un spatiu liber
            if(mat_pseudo[i][j] == 0)
            {
                for(int nr = 1; nr <= 9; nr++)
                {
                // formula ce transpunem indicii din pseudo matrice in coef celulei
                int coef_celula = 3 * (i/3) + j/3;
                
                int nr_aparitii_linie = NumberInLine(nr, i, mat_pseudo);
                int nr_aparitii_coloana = NumberInColumn(nr, j, mat_pseudo);
                int nr_aparitii_celula = NumberInBox(nr, coef_celula, mat_pseudo);

                if((nr_aparitii_linie + nr_aparitii_coloana + nr_aparitii_celula) == 0)
                {
                        FillCell(i, j, bitMap, nr, dim_cell);
                }
                

                }
            }
            

        }
}

int main(int argc, char* argv[])
{

    char *task = argv[2];

    if(argc > 3)
        printf("Se depaseste nr de argumente\n");
    else
    {
        if(strcmp(task, "123") == 0)
        {
        
            FILE* finput = fopen(argv[1], "r");
            char *input_name = argv[1];

        //TASK1

            char *name_output1 = Name_Output1(input_name);
            FILE* foutput1 = fopen(name_output1, "wb");
            
            char *string_json = CovertInputtoString(finput);
            cJSON *json = cJSON_Parse(string_json);
            free(string_json);
            bmp_fileheader* bmpHeader = malloc(sizeof(bmp_fileheader));
            bmp_infoheader* bmpInfo = malloc(sizeof(bmp_infoheader));

            ReadDataHeader(json, bmpHeader);
            ReadDataInfo(json, bmpInfo);
            unsigned char **bitmap = ReadBitMap(json, bmpInfo);
            WriteNewBmp(foutput1, bitmap, bmpInfo, bmpHeader);

        //TASK2
            
            char *name_output2 = Name_Output2(input_name);
            FILE* foutput2 = fopen(name_output2, "wb");

            ReadDataHeader(json, bmpHeader);
            ReadDataInfo(json, bmpInfo);
            MapSwap(bitmap);
            WriteNewBmp(foutput2, bitmap, bmpInfo, bmpHeader);;

            

        //TASK3

            char *name_output3 = Name_Output3(input_name);

            FILE* foutput3 = fopen(name_output3, "w");

            int **mat_pseudo = calloc(NR_SUDOKU_CELLS, sizeof(int *));

            for(int i = 0; i < NR_SUDOKU_CELLS; i++)
                mat_pseudo[i] = calloc(NR_SUDOKU_CELLS, sizeof(int));
            
            char* mesaj = CheckMap( bitmap, mat_pseudo);
            char* board = Board_Name(input_name);

            cJSON *output = cJSON_CreateObject();
            cJSON *input_file = NULL;
            cJSON *game_state = NULL;
            input_file = cJSON_CreateString(board);
            game_state = cJSON_CreateString(mesaj);
            cJSON_AddItemToObject(output, "input_file", input_file);
            cJSON_AddItemToObject(output, "game_state", game_state);

            char* string = cJSON_Print(output);
            fprintf(foutput3, "%s\n", string);


        //Free memory
            FreeMap( bmpInfo, bitmap);
            free(bmpHeader);
            free(bmpInfo);
            free(mesaj);

            for(int i = 0; i < NR_SUDOKU_CELLS; i++)
                free(mat_pseudo[i]);
            
            free(mat_pseudo);
            free(board);
            free(string);

            cJSON_Delete(json);
            cJSON_Delete(output);

            fclose(finput);
            fclose(foutput1);
            fclose(foutput2);
            fclose(foutput3);

            free(name_output1);
            free(name_output2);
            free(name_output3);

        }


        if(strcmp(task, "4") == 0)
        {       
        // TASK4
            FILE* finput = fopen(argv[1], "r");
            char *input_name = argv[1];

            char *name_output4 = Name_Output4(input_name);
            FILE* foutput4 = fopen(name_output4, "wb");

            bmp_fileheader* bmpHeader = malloc(sizeof(bmp_fileheader));
            bmp_infoheader* bmpInfo = malloc(sizeof(bmp_infoheader));

            // alocam matricea de biti
            unsigned char **bitmap = calloc(73, sizeof(unsigned char *));

            for(int i = 0; i < 73; i++)
                bitmap[i] = calloc(73 * 3 + 1, sizeof(unsigned char));

            //alocam pseudo matricea
            int **mat_pseudo = calloc(NR_SUDOKU_CELLS, sizeof(int *));

            for(int i = 0; i < NR_SUDOKU_CELLS; i++)
                mat_pseudo[i] = calloc(NR_SUDOKU_CELLS, sizeof(int));


            ReadFromBitmap(finput, bitmap, bmpInfo, bmpHeader);
            SolveSudoku( bitmap, mat_pseudo);
            WriteNewBmp(foutput4, bitmap, bmpInfo, bmpHeader);
            

        //Free

            for(int i = 0; i < NR_SUDOKU_CELLS; i++)
                free(mat_pseudo[i]);

            free(mat_pseudo);
            FreeMap( bmpInfo, bitmap);
            free(bmpHeader);
            free(bmpInfo);
            fclose(finput);
            fclose(foutput4);
            free(name_output4);


        }
    }


    return 0;
}