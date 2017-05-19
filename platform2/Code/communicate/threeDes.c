#include "threeDes.h"    
#include <string.h>
#include "global.h"

// const unsigned char key1[] = "12345678";   
// const  unsigned char key2[] = "abcdefgh";   
// //const  unsigned char key3[] = "~!@#$%^&";
// const unsigned char key3[] = "12345678";

const uint8 Table_IP[64] =    
{    
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,    
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,    
    57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,    
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7    
};    
   

const uint8 Table_InverseIP[64] =    
{    
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,    
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,    
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,    
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25    
};    
   

const uint8 Table_E[48] =    
{    
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,    
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,    
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,    
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1    
};    
   

const uint8 Table_PC1[56] = {    
    57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,    
    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,    
    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,    
    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4    
};    
   

const signed char Table_Move[2][16] =    
{    

    {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1},   
   

    {0, -1, -2, -2, -2, -2, -2, -2, -1, -2, -2, -2, -2, -2, -2, -1}   
};    
   

const uint8 Table_PC2[48] =    
{    
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,    
    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,    
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,    
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32    
};    
   

const uint8 Table_SBOX[8][4][16] =    
{    
    // S1     
    14, 4, 13, 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,    
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,    
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,    
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,    
    // S2     
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,    
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,    
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,    
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,    
    // S3     
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,    
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,    
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,    
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,    
    // S4     
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,    
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,    
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,    
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,    
    // S5     
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,    
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,    
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,    
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,    
    // S6     
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,    
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,    
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,    
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,    
    // S7     
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,    
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,    
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,    
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,    
    // S8     
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,    
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,    
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,    
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11    
};    
   

const uint8 Table_P[32] =    
{    
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5, 18, 31, 10,    
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6, 22, 11, 4,  25    
};    
   

void Xor(uint8 * p_buf_1, const uint8 * p_buf_2, uint8 bytes)   
{   
    while(bytes > 0)   
    {   
        bytes--;   
   
        p_buf_1[bytes] ^= p_buf_2[bytes];   
    }   
}   
   

void move_left(uint8 * p_input, uint8 bit_start, uint8 bit_end, uint8 offset)    
{    
    uint8 b_val = 0;   
    uint8 b_tmp1 = 0;   
    uint8 b_tmp2 = 0;   
   

    b_tmp1 = GET_BIT(p_input, bit_start);   
    b_tmp2 = GET_BIT(p_input, bit_start + 1);   
   

    for(; bit_start <= (bit_end - offset); bit_start++)   
    {   
        b_val = GET_BIT(p_input, bit_start + offset);   
        SET_BIT(p_input, bit_start, b_val);   
    }   
   

    if (1 == offset)   
    {   
        SET_BIT(p_input, bit_end, b_tmp1);   
    }   
    else   
    {   
        SET_BIT(p_input, bit_end, b_tmp2);   
        SET_BIT(p_input, bit_end - 1, b_tmp1);   
    }   
}    
   

void move_right(uint8 * p_input, uint8 bit_start, uint8 bit_end, uint8 offset)    
{    
    uint8 b_val = 0;   
    uint8 b_tmp1 = 0;   
    uint8 b_tmp2 = 0;   
   

    b_tmp1 = GET_BIT(p_input, bit_end);   
    b_tmp2 = GET_BIT(p_input, bit_end - 1);   
   

    for(; bit_end >= (bit_start + offset); bit_end--)   
    {   
        b_val = GET_BIT(p_input, bit_end - offset);   
        SET_BIT(p_input, bit_end, b_val);   
    }   
   

    if (1 == offset)   
    {   
        SET_BIT(p_input, bit_start, b_tmp1);   
    }   
    else   
    {   
        SET_BIT(p_input, bit_start, b_tmp2);   
        SET_BIT(p_input, bit_start + 1, b_tmp1);   
    }   
}    
   

void move_bits(uint8 * p_input, uint8 bit_start, uint8 bit_end, char offset)   
{   
    if(0 < offset)
    {   
        move_left(p_input, bit_start, bit_end, offset);    
    }      
    else if(0 > offset)
    {   
        move_right(p_input, bit_start, bit_end, -offset);   
    }   
}   
   
void Permutation(uint8 * p_input, uint8 * p_output, const uint8 * Table, uint8 bits)    
{    
    uint8 b_val = FALSE;   
    uint8 bit_index = 0;   
   
    for(bit_index = 0; bit_index < bits; bit_index++)    
    {   
        b_val = GET_BIT(p_input, Table[bit_index] - 1);   
           
        SET_BIT(p_output, bit_index, b_val);   
    }   
}    
   

uint8 S_GetLine(uint8 * p_data_ext, uint8 bit_s)   
{   
    return (GET_BIT(p_data_ext, bit_s + 0) << 1) + GET_BIT(p_data_ext, bit_s + 5);   
}   
   

uint8 S_GetRow(uint8 * p_data_ext, uint8 bit_s)   
{   
    uint8 row;   
   

    row = GET_BIT(p_data_ext, bit_s + 1);   
    row <<= 1;    
    row += GET_BIT(p_data_ext, bit_s + 2);   
    row <<= 1;    
    row += GET_BIT(p_data_ext, bit_s + 3);   
    row <<= 1;    
    row += GET_BIT(p_data_ext, bit_s + 4);   
   
    return row;   
}   
   

void des(const unsigned char * p_data, const char * p_key, const unsigned char * p_output, DES_MODE mode)   
{   
    uint8 loop = 0;         
    uint8 key_tmp[8];      
    uint8 sub_key[6];       
   
    uint8 * p_left;   
    uint8 * p_right;   
   
    uint8 p_right_ext[8];       
    uint8 p_right_s[4];       
   
    uint8 s_loop = 0;          
   
   
    Permutation((uint8*)p_key, key_tmp, Table_PC1, 56);   
   
      
    Permutation((uint8*)p_data, (uint8*)p_output, Table_IP, 64);   
   
    p_left  = (uint8*)p_output;         
    p_right =(uint8*) &p_output[4];  
   
    for(loop = 0; loop < 16; loop++)   
    {   
          
        move_bits(key_tmp, 0, 27, Table_Move[mode][loop]);   
        move_bits(key_tmp, 28, 55, Table_Move[mode][loop]);   
   
        
        Permutation(key_tmp, sub_key, Table_PC2, 48);   
   
          
        Permutation(p_right, p_right_ext, Table_E, 48);   
   
            
        Xor(p_right_ext, sub_key, 6);   
   
        
        for(s_loop = 0; s_loop < 4; s_loop++)   
        {   
            uint8 s_line = 0;   
            uint8 s_row = 0;   
            uint8 s_bit = s_loop * 12;   
   
            s_line = S_GetLine(p_right_ext, s_bit);   
            s_row  = S_GetRow(p_right_ext,  s_bit);   
               
            p_right_s[s_loop] = Table_SBOX[s_loop * 2][s_line][s_row];   
   
            s_bit += 6;   
               
            s_line = S_GetLine(p_right_ext, s_bit);   
            s_row  = S_GetRow(p_right_ext,  s_bit);   
               
            p_right_s[s_loop] <<= 4;   
            p_right_s[s_loop] += Table_SBOX[(s_loop * 2) + 1][s_line][s_row];   
        }   
   
          
        Permutation(p_right_s, p_right_ext, Table_P, 32);   
   
        Xor(p_right_ext, p_left, 4);   
   
        memcpy(p_left, p_right, 4);   
        memcpy(p_right, p_right_ext, 4);   
    }   
   
    memcpy(&p_right_ext[4], p_left, 4);   
    memcpy(p_right_ext, p_right, 4);   
   
    
    Permutation(p_right_ext, (uint8*)p_output, Table_InverseIP, 64);           
}

void desD(const unsigned char * en_data, const unsigned char * en_out_put, const char *key1, const char *key2, const char *key3){
    des(en_data, key1, en_out_put, DES_ENCRYPT);   
    des(en_out_put, key2, en_data, DES_DECRYPT);   
    des(en_data, key3, en_out_put, DES_ENCRYPT);   
	
}
void desE(const unsigned char * en_data, const unsigned char * en_out_put, const char *key1, const char *key2, const char *key3){
    des(en_out_put, key3, en_data, DES_DECRYPT);   
    des(en_data, key2, en_out_put, DES_ENCRYPT);   
    des(en_out_put, key1, en_data, DES_DECRYPT);   
}
