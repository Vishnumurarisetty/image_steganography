/*Name : MURARISETTY VISHNU
Date : 24 - 04 -2024
Title: Steganography
 */
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    //Cheking the command line arguments passsed or not
    if(argc ==1)
    {
        //printing error message and usage if it is not passed
        printf("Error : Enter the choice and input files\n");
        printf("Usage : ./a.out -e/-d beautiful.bmp secret.txt\n");
        return e_failure;
    }
    //checking the option is encoding or not
    int ret = check_operation_type(argv);
    if(ret == e_encode)
    {
        //checking that the required header files are passed or not
        if(argc >= 4)
        {
            //checking the entered files are valid or not
            if(read_and_validate_encode_args(argv,&encInfo) == e_success)
            {
                //calling do encoding funtion if all the files are valid
                if(do_encoding(&encInfo) == e_success)
                {
                    //printing the prompt that encoding is successful
                    printf("INFO : ## Encoding Done Successfully ##\n");
                }

            }
            else
            {
                //printing that the entered files are not valid
                printf("Error: Enter the valid files\n");
                return 0;
            }
        }
        else
        {
            //printing error message and usage message to enter valid number of arguments 
            printf("Error : Invaid Number of arguments\n");
            printf("Usage : ./a.out -e/-d beautiful.bmp secret.txt\n");
        }
    }
    //checking for the entered option is decoding or not
    else if(ret == e_decode)
    {
        //checking the number of command line aruments entered are valid for decoding or not
        if(argc >= 3)
        {
            //validating the entered files for decoding 
            if(read_and_validate_decode_args(argv,&decInfo) == e_success)
            {
                //printing the error message if the files are not valid
                printf("INFO : Files are vaild to decode\n");  
                //calling do decoding function 
                if(do_decoding(&decInfo) == e_success)
                {
                    //printing the promt that the decoding is successfull
                    printf("INFO : ## Decoding Done Successfully\n");
                }
            }
            else
            {
                //printing the error meesage to enter the valid files
                printf("ERROR : Enter valid files\n");
                return 0;
            }
        }
        else
        {
            //printing error message and usage message if the entered arguments are lessthan required arguments
            printf("Error : Invalid Number of arguments\n");
            printf("Usage : ./a.out -d stego.bmp\n");
        }
    }
    else
    {
        //printing error message to the user to select encoding or decoding 
        printf("Error: Enter between encoding -e (or) decoding -d\n");
        return 0;
        //successfu termination
    }


    return 0;
}

//function definition for checking which operation we need to do
OperationType check_operation_type(char *argv[])
{
    //checking the operation is encoding or not
    if(!strcmp(argv[1],"-e"))
        return e_encode;
    //checking the operation is decoding or not
    else if(!strcmp(argv[1],"-d"))
        return e_decode;
    else
        return e_unsupported;
}
