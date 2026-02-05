# Image Steganography in C

## Problem Statement
Steganography is the technique of hiding secret information within a carrier file such that the presence of the data is not easily detectable. This project implements image steganography using the Least Significant Bit (LSB) technique in C.

## Objective
To encode and decode secret data inside a BMP image using bit-level manipulation while preserving the visual quality of the image.

## Features
- Encode secret text data into a BMP image
- Decode hidden data from a stego image
- Uses LSB technique for data hiding
- Supports encoding magic string, file extension, file size, and data
- Command-line based execution

## Implementation Details
- Implemented LSB-based encoding and decoding logic
- Used magic string to identify stego images
- Encoded metadata such as file extension and size before actual data
- Modular design with separate encode and decode modules

## Challenges Faced
Handling bit-level operations and maintaining synchronization between encoding and decoding was challenging. Careful pointer management and strict encoding order were required to ensure correct data extraction.

## Key Learnings
- Strong understanding of bitwise operations
- Practical experience with file handling in C
- Learned how steganography works at binary level
- Improved modular programming and debugging skills

## How to Compile and Run

### Encoding
gcc src/*.c -o stego
./stego -e input.bmp secret.txt output.bmp
