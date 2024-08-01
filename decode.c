/*Name : MURARISETTY VISHNU
Date : 24 - 04 -2024
Title: Steganography
 */

//including the required files
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

//function definition for validating the files
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    //checking the file is bmp file or not
    if(strstr(argv[2],".bmp") != NULL)
    {
        //if it is .bmp file storing file name in a structure member
        decInfo -> src_image_fname = argv[2];
    }
    else
    {
        //printing error message if the file is not a .bmp file 
        printf("Error: Source Image must be a .bmp file\n");
        return e_failure; //returning failure if the file is not a .bmp file  
    }
    //checking the output file name is passed or not
    if(argv[3] != NULL)
    {
        //if it is passed just storing its name excluding the extension
        strcpy(decInfo -> output_fname,strtok(argv[3],"."));
    }
    else
    {
        //if it is not passed giving a default name
        strcpy(decInfo -> output_fname, "output");
    }
    return e_success; //returning success if all the files read successfully
}

//calling do encoding function
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO : Decoding started\n");                                  //printing the prompt
    if(open_files_dec(decInfo) == e_success)                              // calling funtion to open the files and checking wether they successfully opened or not
    {
        printf("INFO : Files opened Successfully\n");                     //printing the prompt
        if(decode_magic_string(decInfo) == e_success)                    //calling the function to decode magic string and cheking 
        {
            printf("INFO : Magic string decoding successfull\n");        //printing the prompt
            if(decode_sec_file_extn_size(decInfo) == e_success)          //calling the function to decode secret file extension size and checking  
            {
                printf("INFO : Secret file extension size decoded successfully\n");    //printing the prompt 
                if(decode_sec_file_extn(decInfo) == e_success)                         //calling the function to decode secret file extention and checking 
                {
                    printf("INFO : Secret file extension decoded successfully\n");     //printing the prompt
                    if(decode_output_file_size(decInfo) == e_success)                  //calling the function to decode output file size
                    { 
                        printf("INFO : Output file size decoded Successfully\n");      //printing the prompt
                        if(decode_output_data(decInfo) == e_success)                   //calling the function  to decode output  file  data 
                        {
                            printf("INFO : Decoding Completed\n");                     //printing the prompt  
                            return e_success;                                          //returning the success message 
                        }
                        else
                        {
                            printf("INFO : Decoding output file data failed\n");       
                            return e_failure;                                          
                        }
                    }
                    else
                    {
                        printf("INFO : Output file size decoding failed\n");           //printing output size decoding failed
                        return e_failure;                                              //reurning failure
                    }
                }
                else
                {
                    printf("INFO : Secret file extention decoding failed\n");          //printing secretfile extension decoding failed
                    return e_failure;                                                  //returning failure
                }
            }
            else
            {
                printf("INFO : Secret file extention size decoding failed\n");         //printing secret file extension size decodinng failed 
                return e_failure;                                                      //retunring failure
            }
        }
        else
        {
            printf("INFO : Magic string decoding failed\n");                          //printing magic string decoding failed
            return e_failure;                                                         //returning failure
        }

    }
    else
    {
        printf("INFO : Files opening failed\n");                                      // printing files opening failed
        return e_failure;                                                             // returning failure
    }
}

// open files function definition
Status open_files_dec(DecodeInfo *decInfo)
{
    decInfo -> fptr_src_image = fopen(decInfo -> src_image_fname, "r");       //opening source image file and storing base address
    if(decInfo -> fptr_src_image == NULL)                                     //checking  source image adress is null or not
    {
        perror("fopen");
        fprintf(stderr,"ERROR : Unable to open file %s\n",decInfo -> src_image_fname);
        return e_failure;
    }
    return e_success;
}

// decode magic string function definition
Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("INFO : Decoding magic string..........!\n");
    fseek(decInfo -> fptr_src_image, 54, SEEK_SET);                     //moving cursor to 54th cursor
    char ms[3];      // creating a char array
    int i;
    for(i=0; i<2; i++)  // for loop to decode magic string
    {
        char buffer[8];
        fread(buffer,1,8,decInfo -> fptr_src_image);   // reading bytes from source image 
        ms[i] = decode_byte_from_lsb(buffer);     //storing the char by calling decode byte to lsb function
    }
    ms[i] = '\0'; //assigning the null
    if(!strcmp(ms,MAGIC_STRING))   // comparing decoded magic string with original magic string 
        return e_success;
    else
        return e_failure;	
}

// decode byte from lsb function definnition
Status decode_byte_from_lsb(char *buffer)
{
    int ch = 0;
    for(int i=0; i<=7; i++)                        //for loop to decode one char 
    {
        ch = ch | ((buffer[i] & 1) << (7-i));       // making a char from 8 bytes
    }
    return ch;   //retuning the char
}

// decode secret file extention size function definition
Status decode_sec_file_extn_size(DecodeInfo *decInfo) 
{
    printf("INFO : Decoding secret file extension size\n");
    char buffer[32];      // declaring a char array for buffer
    fread(buffer,1,32,decInfo -> fptr_src_image);      // reading 32 bytes from the source image for decoding size
    decInfo -> extn_size = decode_size(buffer);       // geting size by calling decode size function
    return e_success;
}

// decode size function definition
Status decode_size(char *buffer)
{
    int size = 0;
    for(int i=0; i<32; i++)            // for loop to decode 32 bits from 32 bytes of buffer
    {
        size = size | ((buffer[i] & 1) << (31 - i));  // decoding process
    } 
    return size;   //retuning the size
}

// decode secret file extension function definition
Status decode_sec_file_extn(DecodeInfo *decInfo)
{
    char extn[decInfo -> extn_size + 1];           // creating a char array to store the extension
    int i=0;
    for(i=0; i < decInfo -> extn_size; i++)        // for loop to get extention based on the extension size
    {
        char buffer[8];              // temporary buffer to acces 8 bytes
        fread(buffer,1,8,decInfo -> fptr_src_image);    //reading 8 bytes from the source image to get one char 
        extn[i] = decode_byte_from_lsb(buffer);     // decoding one char by calling decode byte from lsb function
    }
    extn[i] = '\0';   // storing null char at the end 
    strcat(decInfo -> output_fname,extn);            // concatinating the extention to the output file name 
    decInfo -> fptr_out = fopen(decInfo -> output_fname,"w");  // opening the output file in write mode
    return e_success;   
}

// decode output file size function defintion
Status decode_output_file_size(DecodeInfo *decInfo)
{
    printf("INFO : Decoding output file size\n");
    char buffer[32]; // 32 byte buffer 
    fread(buffer,1,32,decInfo -> fptr_src_image);  //reading 32 bytes from the source image 
    decInfo -> out_file_size = decode_size(buffer);  // getting file size by calling decode size function
    return e_success;
}

// decode output data function definition
Status decode_output_data(DecodeInfo *decInfo)
{

    char buffer[8];   // temporary buffer of 8 bytes
    for(int i=0; i < decInfo -> out_file_size; i++)    // for loop to decode output data based on the ouput file size
    {
        char ch;
        fread(buffer,1,8,decInfo -> fptr_src_image);  // reading 8 bytes form source image to decode one char
        ch = decode_byte_from_lsb(buffer);           // decoding one char by calling decode byte from lsb and storing 
        fputc(ch,decInfo -> fptr_out);                
    }
    return e_success;
}
