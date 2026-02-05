#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include<stdio.h>
/* 
 * Structure to store information required for
 * Decoding  stego Image to decodebfile
 * Info about intermediate data and output is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE2 1000000
#define MAX_IMAGE_BUF_SIZE2 (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX2 10

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    
    /* Decode File Info */
    char *decode_fname;
    FILE *fptr_decode;
    char extn_secret_file[MAX_FILE_SUFFIX2];
    char secret_data[MAX_SECRET_BUF_SIZE2];
    long size_secret_file;



} DecodeInfo;
/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);
/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);
/* Decode secret file size */
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo);
/* Decode secret file extenstion */
Status decode_secret_file_extn(int extn_size,DecodeInfo *decInfo);

//Decode the secret file extension size
Status decode_size(FILE *fptr_stego_img,int *size);
//Decode char from lsb bit
Status decode_char_from_lsb(char *image_buffer, char *str);
//Decode size from lsb bit
Status decode_size_from_lsb(char *image_buffer, int *size);
/* Decode secret file size */
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo);
/* Decode secret file data*/
Status decode_secret_file_data(long file_size,DecodeInfo *decInfo);


#endif
