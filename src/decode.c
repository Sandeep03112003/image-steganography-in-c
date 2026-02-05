#include <stdio.h>
#include "decode.h"
#include<string.h>
#include "types.h"
#include"common.h"



Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    //validate .bmp file
     if(argv[2] != NULL && strcmp(strstr(argv[2],"."),".bmp")==0)
     {
        decInfo->stego_image_fname = argv[2];
     }
     else
     {
          return e_failure;
     }
     //validate .txt file
     if(argv[3] != NULL)
     {
        decInfo->decode_fname = argv[3];
     }
     else
     {
        decInfo->decode_fname="secret_msg.txt";
     }
     return e_success;
}
Status open_decode_files(DecodeInfo *decInfo)
{

    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname,"r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
     // Secret file
    decInfo->fptr_decode = fopen(decInfo->decode_fname, "w");
    // Do Error handling
    if (decInfo->fptr_decode == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
   
    if(open_decode_files(decInfo) == e_success)
    {
        printf("Successfully opened all required files\n");
        if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
        {
            printf("Successfully decoded the magic string\n");
            // Decode the extension size
             int ext_size = 0;
       if(decode_size(decInfo->fptr_stego_image, &ext_size) == e_success)
         {
                     printf("Decoded extension size: %d\n", ext_size);

                  if(decode_secret_file_extn(ext_size, decInfo) == e_success)
                     {
                    printf("Secret file extension decoded: %s\n", decInfo->extn_secret_file);
                      if(decode_secret_file_size(0,decInfo)== e_success)
                      {
                          printf("Decoded secret file size successfully : %ld bytes\n", decInfo->size_secret_file);
                          if(decode_secret_file_data(decInfo->size_secret_file,decInfo)== e_success)
                          {
                            fwrite(decInfo->secret_data, 1, decInfo->size_secret_file, decInfo->fptr_decode);

                               printf("Successfully decoded the secret file data\n");
                              
                          }
                          else
                          {
                                     printf("Failed to decode secret data\n");
                                     return e_failure;
                          }
                    }
                    else
                    {
                        printf("Failed to decode the file size\n");
                        return e_failure;
                    }
                 }
                   else
                    {
               printf("Failed to decode secret file extension\n");
                     return e_failure;
              }
}

            else
            {
                printf("Failed to decode extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open the files\n");
        return e_failure;
    }
    return e_success;
}
Status decode_magic_string(const char *magic_str, DecodeInfo *decInfo)
{
    int len = strlen(magic_str);
    char decoded_magic_str[len + 1];
    unsigned char image_data[8];

    memset(decoded_magic_str, 0, sizeof(decoded_magic_str));

    // Move file pointer past BMP header
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    for (int i = 0; i < len; i++)
    {
        char ch = 0;

        if (fread(image_data, sizeof(char), 8, decInfo->fptr_stego_image) != 8)
        {
            perror("fread");
            return e_failure;
        }

        for (int j = 0; j < 8; j++)
        {
            ch |= (image_data[j] & 1) << (7 - j);
        }

        decoded_magic_str[i] = ch;
    }

    decoded_magic_str[len] = '\0';

    if (strcmp(decoded_magic_str, magic_str) == 0)
    {
        return e_success;
    }
    else
{
        return e_failure;
    }
}
Status decode_char_from_lsb(char *image_buffer, char *str)
{
    unsigned int value = 0;

    for (int i = 0; i < 8; i++)
    {
        int bit = image_buffer[i] & 1;
        value = (value << 1) | bit;  
    }

    *str = (char)value;   
    return e_success;
}

Status decode_size(FILE *fptr_stego, int *size)
{
    unsigned char buffer[32];

    // Read 32 bytes from stego image
    if (fread(buffer, sizeof(char), 32, fptr_stego) != 32)
    {
        perror("fread");
        return e_failure;
    }

    // Decode from LSB (MSB-first bit order)
    if (decode_size_from_lsb((char *)buffer, size) == e_success)
        return e_success;
    else
        return e_failure;
}

Status decode_size_from_lsb(char *image_buffer, int *size)
{
    unsigned int value = 0;

    for (int i = 0; i < 32; i++)
    {
        int bit = image_buffer[i] & 1;
        value = (value << 1) | bit;  
    }

    *size = (int)value;   
    return e_success;
}
Status decode_secret_file_extn(int extn_size, DecodeInfo *decInfo)
{
    char buffer[8];

    if(extn_size <= 0 || extn_size > MAX_FILE_SUFFIX2)
        return e_failure;

    for(int i = 0; i < extn_size; i++)
    {
        if(fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
            return e_failure;

        decode_char_from_lsb(buffer, &decInfo->extn_secret_file[i]);
    }

    decInfo->extn_secret_file[extn_size] = '\0';
    return e_success;
}
Status decode_secret_file_size(long int file_size, DecodeInfo *decInfo)
{
    unsigned char buffer[32];
    unsigned int value = 0;

    if(fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        perror("fread");
        return e_failure;
    }

    for(int i = 0; i < 32; i++)
    {
        int bit = buffer[i] & 1;    // Take LSB of each byte
        value = (value << 1) | bit; // Build number MSB-first
    }

    decInfo->size_secret_file = (long)value;  // Store in struct
    return e_success;
}
Status decode_secret_file_data(long int file_size, DecodeInfo *decInfo)
{
    char buffer[8];

    if(file_size <= 0 || file_size > MAX_SECRET_BUF_SIZE2)
        return e_failure;

    for(long int i = 0; i < file_size; i++)
    {
        if(fread(buffer, 1, 8, decInfo->fptr_stego_image) != 8)
            return e_failure;

        char ch = 0;
        for(int j = 0; j < 8; j++)
        {
            int bit = buffer[j] & 1;       // Take LSB of each byte
            ch = (ch << 1) | bit;          // Build character MSB-first
        }

        decInfo->secret_data[i] = ch;
    }

    decInfo->secret_data[file_size] = '\0';   // Null-terminate
    return e_success;
}
