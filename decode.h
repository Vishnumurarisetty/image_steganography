#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
//structure
typedef struct _DecodeInfo
{
    /*Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    int extn_size;

    /* Output Image Info */
    char output_fname[100];
    FILE *fptr_out;
    int out_file_size;

} DecodeInfo;

//read and validate function

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

//decoding function 

Status do_decoding(DecodeInfo *decInfo);

//opening of the files

Status open_files_dec(DecodeInfo *decInfo);

//decoding the magic string

Status decode_magic_string(DecodeInfo *decInfo);

//decoding byte from the lsb

Status decode_byte_from_lsb(char *buffer);

//decoding secret file extention size

Status decode_sec_file_extn_size(DecodeInfo *decInfo);

//decoding size

Status decode_size(char *buffer);

//decoding secret file extention

Status decode_sec_file_extn(DecodeInfo *decInfo);

//decoding output file size

Status decode_output_file_size(DecodeInfo *decInfo);

//decoding output data

Status decode_output_data(DecodeInfo *decInfo);


#endif


