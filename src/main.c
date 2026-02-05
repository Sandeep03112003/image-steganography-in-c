#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[])
{
      if(check_operation_type(argv)==e_encode)
      {
        EncodeInfo encInfo;
        printf("Selected Encoding\n");
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf("Read and Validate encode arguments is sucess\n");
            if(do_encoding(&encInfo) == e_success)
            {
                printf("Completed Encoding\n");
            }
            else
            {
                printf("Failed to do the encoding\n");
            }
        }
        else
        {
           printf("Failed to read and Validate encode arguments\n");
        }
      }
      else if(check_operation_type(argv)==e_decode)
      {
           DecodeInfo decInfo;
           printf("Selected Decoding\n");
           if(read_and_validate_decode_args(argv,&decInfo) == e_success)
           {
             printf("Read and Validate decode arguments is sucess\n");
             if(do_decoding(&decInfo) == e_success)
             {
               
                printf("Completed Decoding\n");
             }
             else
             {
                printf("Failed to do the decoding\n");
                return e_failure;
             }
            
           }
           else 
           {
              printf("Failed to Read and Validate encode arguments\n");
           }

      }
      else
      {
          printf("Invalid Arguments\n");
          printf("******************************************************\n");
          printf("Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
          printf("Decoding ./a.out -d stego.bmp decode.txt\n");
      }
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if((strcmp(argv[1],"-e"))==0)
    {
        return e_encode;
    }
    else if((strcmp(argv[1],"-d"))==0)
    {
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}
