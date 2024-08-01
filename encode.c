/*Name : MURARISETTY VISHNU
Date : 24 - 04 -2024
Title: Steganography
 */

// including the required header files
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width 
    fread(&width, sizeof(int), 1, fptr_image);


    // Read the height
    fread(&height, sizeof(int), 1, fptr_image);


    // Return image capacity
    return width * height * 3; //3 for rgb
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO : Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }
    printf("INFO : Opened beautiful.bmp\n");
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");

    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }
    printf("INFO : Opened secret.txt\n");

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }
    printf("INFO : Opened stego.bmp\n");
    // No failure return e_success
    return e_success;
}

//writing the function to validating the command line arguments data and storing into the structutre
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{

    //validating source file is bmp file or not
    if(strstr(argv[2],".bmp")!= NULL)
    {
        encInfo -> src_image_fname = argv[2];
    }
    else
    {
        //printing error message if the source image file is not a bmp file and returning that the function is failure
        printf("Error: Source Image is not a bmp file, Please pass bmp file.\n");
        return e_failure;
    }
    //storing the secter file name to the structure
    encInfo -> secret_fname = argv[3];
    //seperating the secret file name extention and storing to the structure
    strcpy(encInfo -> extn_secret_file, strchr(argv[3],'.'));
    //check wether the output file name is passed or not
    if(argv[4] != NULL)
    {
        //check wether the output file is .bmp file or not
        if(strstr(argv[4],".bmp") != NULL)
        {
            //storing the name to the structure
            encInfo -> stego_image_fname = argv[4];
        }
        else
        {
            //printing error message if the entered output file is not a bmp file and returning function is failure
            printf("Error: Entered file extension is not valid\n");
            return e_failure;
        }
    }
    else
    {
        //storing the output file name to a default name
        printf("INFO : Output File not mentioned. Creating stego.bmp as default\n");	
        encInfo -> stego_image_fname = "stego.bmp";
    }
    return e_success;
}

//function definition for do encoding 
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)    // calling open files function and checking opened or not
    {
        printf("INFO : Done\n");         //printing prompt
        printf("INFO : ## Encoding Procedure Started ##\n");
        if(check_capacity(encInfo) == e_success)       // calling check capacity function and checking 
        {
            printf("INFO : Done. Found OK\n");    // printing prompt
            if(copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_success)    //calling copy header function and checking 
            {
                printf("INFO : Done\n");       // opening files
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)    // calling encode magic string fuction and checking
                {
                    printf("INFO : Done\n");    // printing prompt 
                    if(encode_secret_file_extn_size(encInfo) == e_success)    // calling encode secret file exxtension size function and checking 
                    {
                        printf("INFO : Done\n");   // printing prompt
                        if(encode_secret_file_extn(encInfo -> extn_secret_file,encInfo) == e_success)  //calling encode secret file extension function and checking
                        {
                            printf("INFO : Done\n");  
                            if(encode_secret_file_size(encInfo -> size_secret_file,encInfo) == e_success)  //calling encode secret file size function and checking 
                            {
                                printf("INFO : Done\n");   //printing prompt
                                if(encode_secret_file_data(encInfo) == e_success)   //calling encode secret file data function and cheking
                                {
                                    printf("INFO : Done\n"); // printing prompt
                                    if(copy_remaining_img_data(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_success)  // calling remaininng data and checking
                                    {
                                        printf("INFO : Done\n"); // printing prompt
                                        return e_success;   // returning success
                                    }
                                    else
                                    {
                                        printf("INFO : Failed\n"); // printing prompt
                                        return e_failure; // returning failure 
                                    }
                                }
                                else
                                {
                                    printf("INFO : Failed\n"); // printing prompt
                                    return e_failure; // returning failure
                                }
                            }
                            else
                            {
                                printf("INFO : Failed\n"); // printing prompt
                                return e_failure; // returning failure
                            }
                        }
                        else
                        {
                            printf("INFO : Failed\n"); // printing prompt
                            return e_failure; // returning failure
                        }
                    }
                    else
                    {
                        printf("INFO : Failed\n"); // printing prompt
                        return	e_failure; // returning failure
                    }

                }
                else
                {
                    printf("INFO : Failed\n"); // printing prompt
                    return e_failure; // returning failure
                }

            }
            else
            {
                printf("INFO : Failed\n"); // printing prompt
                return e_failure; // returning failure
            }

        }
        else
        {
            printf("INFO : Failed\n"); // printing prompt
            return e_failure; // returning failure
        }

    }
    else
    {
        printf("INFO : Failed\n"); // printing prompt
        return e_failure; // returning failure
    }
}

//function definition for chekinng capacity
Status check_capacity(EncodeInfo *encInfo)
{
    printf("INFO : Checking for beautiful.bmp capacity to handle secret.txt\n");
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);  // getting image capacity by calling get image size for bmp funtion and storiing in a structure member
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret); // getting secret file size and storing in a structure member
                                                                         // finding the encoding data size and comparing with image capacity 
    if(((strlen(MAGIC_STRING) * 8) + 32 + (strlen(encInfo -> extn_secret_file) * 8) + 32 + (encInfo -> size_secret_file * 8)) < encInfo -> image_capacity)
        return e_success; // returning success
    else
        return e_failure; // returning failure
}

// get file size function definition
uint get_file_size(FILE *sefptr)
{
    //move file pointer to the  EOF
    fseek(sefptr,0,EOF);
    //returning the cursor size to the beginning
    return ftell(sefptr);
}

// copy header function definiton
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    printf("INFO : Copying Image Header \n");
    rewind(fptr_src_image); // takinng the cursor back to the 0th position
    char buffer[55]; // temporary buffer of 55 bytes for copying header
    fread(buffer, 1, 54, fptr_src_image); // reading header of 54 bytes from the source image file 
    fwrite(buffer, 1, 54, fptr_stego_image); // writing the copied header data into then output image file 
    return e_success; // returning success
}

// encode maagic string function definition
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("INFO : Encoding Magic String Signature\n");
    char buffer[8]; // temporary buffer of 8 bytes
    for(int i=0; i< strlen(MAGIC_STRING); i++) //for loop to encode magic string based on the length of the magic string 
    {
        fread(buffer, 1, 8, encInfo -> fptr_src_image);  // getting 8 bytes of data from the source image 
        encode_byte_to_lsb(MAGIC_STRING[i],buffer);      // encoding the character by calling byte to lsb function 
        fwrite(buffer,1, 8, encInfo -> fptr_stego_image); // writing the data into the  output image 
    }
    return e_success; // returning success
}

// encode byte to lsb function definition
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i <= 7; i++)
    {

        image_buffer[i] = (image_buffer[i] & 0xfe) | ((data & (1 << (7-i))) >> (7-i)); // encoding process
    }
    return e_success; // returning success
}

// encode secret file extension size function definition
Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    long int sfe_size = strlen(encInfo -> extn_secret_file);               // finding secret file extension size and storing in a variable 
    printf("INFO : Encoding secret.txt File Extension Size\n");
    char buffer[4]; // temporary buffer of 4 bytes to encode size
     
    for (int i = 31; i >= 0; i--) // for loop to encode size to lsb 
    {
        fread(buffer,1,1,encInfo->fptr_src_image);
        if ((sfe_size & (1 << i)) != 0) // checking the bit is set or not
        {
            encoding_size_from_lsb(1, buffer); // if it is set passing 1 and calling encode size function
        } 
        else 
        {
            encoding_size_from_lsb(0, buffer);  // else passing 0 and calling encode size function 
        }
        if (fwrite(buffer, 1, 1, encInfo->fptr_stego_image) != 1) // writing into output file and error handiling
        {
            printf("Error: Failed to write to stego image\n");
            return e_failure; // returning failure
        }
    }
    return e_success; // returning success
}

// encoding size function definition
Status encoding_size_from_lsb(int data, char *image_buffer)
{

    *image_buffer = (*image_buffer & 0xFE) | (data & 0x01); //encoding process
}

//encode secrect file extension functionn defintion
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    printf("INFO : Encoding secret.txt File Extension\n");
    char buffer[8]; // temporary buffer of 8 bytes
    for(int i=0; i<strlen(file_extn); i++) // for loop to encode secret file extension based on the size
    {
        fread(buffer, 1, 8, encInfo -> fptr_src_image);     // getting 8 bytes from source file
        encode_byte_to_lsb(file_extn[i],buffer);  // calling encode byte to lsb functionn  to encode data 
        fwrite(buffer, 1, 8, encInfo -> fptr_stego_image); // writinng into the output file 
    }
    return e_success;  // returning success
}

// encode secret fiile size function definiton
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    // Assuming sefptr is the file pointer for the secret file
    FILE *sefptr = encInfo->fptr_secret;

    // Get the file size
    fseek(sefptr, 0, SEEK_END); // Move to the end of the file
    encInfo -> size_secret_file = ftell(sefptr);  // Get the current position, which is the size of the file
    fseek(sefptr, 0, SEEK_SET); // Reset the file position to the beginning 
    printf("INFO : Encoding secret.txt File Size\n");
    char buffer[4];
     
    for (int i = 31; i >= 0; i--)   // for loop to encode size
    {
        fread(buffer,1,1,encInfo->fptr_src_image);
        if ((encInfo -> size_secret_file & (1 << i)) != 0)  //cheking the bit  is set or clear 
        {
            encoding_size_from_lsb(1, buffer); // calling encode size function  by passing 1
        } 
        else 
        {
            encoding_size_from_lsb(0, buffer); // calling encode size function by passing 0 
        } 
        if (fwrite(buffer, 1, 1, encInfo->fptr_stego_image) != 1) //writing to the output file 
        {
            printf("Error: Failed to write to stego image\n");
            return e_failure; // returning failure
        }
    }
    return e_success;  // returning success
}

// encode secret file data function definition
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO : Encoding secret.txt File Data\n");
    char buffer[8], sec_data[encInfo -> size_secret_file]; //declaring required arrays
    fread(sec_data,1,encInfo -> size_secret_file,encInfo -> fptr_secret); // reading the secret file data 
    for(int i=0; i< encInfo -> size_secret_file; i++)  // for loop to encode secret file data
    {
        fread(buffer, 1, 8, encInfo -> fptr_src_image);  // reading 8 bytes form the source file 
        encode_byte_to_lsb(sec_data[i],buffer);   // encoding by calling encode byte to lsb function
        fwrite(buffer, 1, 8, encInfo -> fptr_stego_image); // writing data to the output file 
    }
    return e_success;  // returning success
} 

// copy remaining image data function definition
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{

    printf("INFO : Copying Left Over Data\n");
    int ch; //declaring a variable
    while((ch = getc(fptr_src)) != EOF) // getting each byte until end of file 
    {
        putc(ch,fptr_dest);   // putting into the output file 
    }
    return e_success;  // returning success 
}
