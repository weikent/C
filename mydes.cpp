#include "mydes.h"

const LL key11=2084160214307458188L;
const LL key21=1650119177278998150L;
const LL key31=2084160214307458188L;

const LL qleft[][3][3]={//[st=0 for 0;1 for 28][off=1/2][res=0 for all;1 for left;2 for right]
0L,0L,0L,
18446744073441116160L,268435454L,1L,
18446744073441116160L,268435452L,3L,
0L,0L,0L,
18374686479940059135L,72057593501057024L,268435456L,
18374686479940059135L,72057592964186112L,805306368L
};
const LL qright[][3][3]={//[st=0 for 0;1 for 28][off=1/2][res=0 for all;1 for left;2 for right]
0L,0L,0L,
18446744073441116160L,134217728L,134217727L,
18446744073441116160L,201326592L,67108863L,
0L,0L,0L,
18374686479940059135L,36028797018963968L,36028796750528512L,
18374686479940059135L,54043195528445952L,18014398241046528L
};
const byte qline[]={
0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,
1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,
0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,
1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,
0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,
1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,
0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,
1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3
};
const byte qrow[]={
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15,
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15,
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15,
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15,
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15,
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15,
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15,
0,0,8,8,4,4,12,12,2,2,10,10,6,6,14,14,
1,1,9,9,5,5,13,13,3,3,11,11,7,7,15,15
};
const byte qrev[]={
0,128,64,192,32,160,96,224,16,144,80,208,48,176,112,240,
8,136,72,200,40,168,104,232,24,152,88,216,56,184,120,248,
4,132,68,196,36,164,100,228,20,148,84,212,52,180,116,244,
12,140,76,204,44,172,108,236,28,156,92,220,60,188,124,252,
2,130,66,194,34,162,98,226,18,146,82,210,50,178,114,242,
10,138,74,202,42,170,106,234,26,154,90,218,58,186,122,250,
6,134,70,198,38,166,102,230,22,150,86,214,54,182,118,246,
14,142,78,206,46,174,110,238,30,158,94,222,62,190,126,254,
1,129,65,193,33,161,97,225,17,145,81,209,49,177,113,241,
9,137,73,201,41,169,105,233,25,153,89,217,57,185,121,249,
5,133,69,197,37,165,101,229,21,149,85,213,53,181,117,245,
13,141,77,205,45,173,109,237,29,157,93,221,61,189,125,253,
3,131,67,195,35,163,99,227,19,147,83,211,51,179,115,243,
11,139,75,203,43,171,107,235,27,155,91,219,59,187,123,251,
7,135,71,199,39,167,103,231,23,151,87,215,55,183,119,247,
15,143,79,207,47,175,111,239,31,159,95,223,63,191,127,255
};
const LL qbytemasknot[]={
18446744073709551360L,18446744073709486335L,18446744073692839935L,18446744069431361535L
,18446742978492891135L,18446463698244468735L,18374967954648334335L,72057594037927935L
};
const LL Table_PC1x[] = {
128L,32768L,8388608L,36028797018963968L,
2251799813685248L,8796093022208L,34359738368L,72057594037927936L,
64L,16384L,4194304L,18014398509481984L,
1125899906842624L,4398046511104L,17179869184L,72057594037927936L,
32L,8192L,2097152L,9007199254740992L,
562949953421312L,2199023255552L,8589934592L,72057594037927936L,
16L,4096L,1048576L,4503599627370496L,
281474976710656L,1099511627776L,4294967296L,72057594037927936L,
8L,2048L,524288L,134217728L,
140737488355328L,549755813888L,2147483648L,72057594037927936L,
4L,1024L,262144L,67108864L,
70368744177664L,274877906944L,1073741824L,72057594037927936L,
2L,512L,131072L,33554432L,
35184372088832L,137438953472L,536870912L,72057594037927936L,
1L,256L,65536L,16777216L,
17592186044416L,68719476736L,268435456L,72057594037927936L
};
const LL Table_IPx[] = {
549755813888L,128L,140737488355328L,32768L,
36028797018963968L,8388608L,9223372036854775808L,2147483648L,
274877906944L,64L,70368744177664L,16384L,
18014398509481984L,4194304L,4611686018427387904L,1073741824L,
137438953472L,32L,35184372088832L,8192L,
9007199254740992L,2097152L,2305843009213693952L,536870912L,
68719476736L,16L,17592186044416L,4096L,
4503599627370496L,1048576L,1152921504606846976L,268435456L,
34359738368L,8L,8796093022208L,2048L,
2251799813685248L,524288L,576460752303423488L,134217728L,
17179869184L,4L,4398046511104L,1024L,
1125899906842624L,262144L,288230376151711744L,67108864L,
8589934592L,2L,2199023255552L,512L,
562949953421312L,131072L,144115188075855872L,33554432L,
4294967296L,1L,1099511627776L,256L,
281474976710656L,65536L,72057594037927936L,16777216L
};
const signed char Table_Move[2][16] = {    
1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1,
0,-1,-2,-2,-2,-2,-2,-2,-1,-2,-2,-2,-2,-2,-2,-1
};
const LL Table_PC2x[] = {
16L,8388608L,64L,32768L,
32L,512L,524288L,131072L,
281474976710656L,2048L,4L,16384L,
4194304L,1L,256L,262144L,
2L,281474976710656L,8192L,2097152L,
1024L,281474976710656L,4096L,8L,
281474976710656L,65536L,1048576L,128L,
70368744177664L,1073741824L,67108864L,140737488355328L,
17179869184L,1099511627776L,281474976710656L,35184372088832L,
134217728L,281474976710656L,274877906944L,2147483648L,
16777216L,8796093022208L,281474976710656L,68719476736L,
8589934592L,4398046511104L,268435456L,34359738368L,
137438953472L,17592186044416L,4294967296L,33554432L,
2199023255552L,281474976710656L,536870912L,549755813888L,
281474976710656L,281474976710656L,281474976710656L,281474976710656L,
281474976710656L,281474976710656L,281474976710656L,281474976710656L
};
const LL Table_Ex[][2] = {
2L,140737488355328L,4L,0L,
8L,0L,16L,64L,
32L,128L,256L,0L,
512L,0L,1024L,4096L,
2048L,8192L,16384L,0L,
32768L,0L,65536L,262144L,
131072L,524288L,1048576L,0L,
2097152L,0L,4194304L,16777216L,
8388608L,33554432L,67108864L,0L,
134217728L,0L,268435456L,1073741824L,
536870912L,2147483648L,4294967296L,0L,
8589934592L,0L,17179869184L,68719476736L,
34359738368L,137438953472L,274877906944L,0L,
549755813888L,0L,1099511627776L,4398046511104L,
2199023255552L,8796093022208L,17592186044416L,0L,
35184372088832L,0L,1L,70368744177664L
};
const byte Table_SBOX[8][4][16] = {        
14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11  
};
const LL Table_Px[] =    
{ 
256L,65536L,4194304L,1073741824L,
4096L,134217728L,2L,131072L,
8388608L,32768L,536870912L,32L,
33554432L,524288L,512L,1L,
128L,8192L,16777216L,4L,
8L,268435456L,1024L,262144L,
2147483648L,2048L,2097152L,64L,
16L,67108864L,16384L,1048576L,
4294967296L,4294967296L,4294967296L,4294967296L,
4294967296L,4294967296L,4294967296L,4294967296L,
4294967296L,4294967296L,4294967296L,4294967296L,
4294967296L,4294967296L,4294967296L,4294967296L,
4294967296L,4294967296L,4294967296L,4294967296L,
4294967296L,4294967296L,4294967296L,4294967296L,
4294967296L,4294967296L,4294967296L,4294967296L,
4294967296L,4294967296L,4294967296L,4294967296L
};
const LL Table_InverseIPx[] =    
{  
144115188075855872L,562949953421312L,2199023255552L,8589934592L,
33554432L,131072L,512L,2L,
576460752303423488L,2251799813685248L,8796093022208L,34359738368L,
134217728L,524288L,2048L,8L,
2305843009213693952L,9007199254740992L,35184372088832L,137438953472L,
536870912L,2097152L,8192L,32L,
9223372036854775808L,36028797018963968L,140737488355328L,549755813888L,
2147483648L,8388608L,32768L,128L,
72057594037927936L,281474976710656L,1099511627776L,4294967296L,
16777216L,65536L,256L,1L,
288230376151711744L,1125899906842624L,4398046511104L,17179869184L,
67108864L,262144L,1024L,4L,
1152921504606846976L,4503599627370496L,17592186044416L,68719476736L,
268435456L,1048576L,4096L,16L,
4611686018427387904L,18014398509481984L,70368744177664L,274877906944L,
1073741824L,4194304L,16384L,64L
};

void move_left(LL& p_input, byte bit_start, byte bit_end, byte offset)    
{    
	LL xleft=p_input&qleft[bit_start][offset][1];
	LL xright=p_input&qleft[bit_start][offset][2];
	p_input&=qleft[bit_start][offset][0];
	p_input|=xleft>>offset;
	p_input|=xright<<(28-offset);
}    
   
void move_right(LL& p_input, byte bit_start, byte bit_end, byte offset)    
{    
	LL xleft=p_input&qright[bit_start][offset][1];
	LL xright=p_input&qright[bit_start][offset][2];
	p_input&=qright[bit_start][offset][0];
	p_input|=xleft>>(28-offset);
	p_input|=xright<<offset;
} 

void move_bits(LL& p_input, byte bit_start, byte bit_end, char offset)   
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

void PermutationE(LL p_input, LL& p_output, const LL Table[][2])    
{      
	p_output=0;
	byte tmp;

	tmp=p_input;
	if(tmp&1){p_output|=Table[0][0];p_output|=Table[0][1];}
	if(tmp&2){p_output|=Table[1][0];p_output|=Table[1][1];}
	if(tmp&4){p_output|=Table[2][0];p_output|=Table[2][1];}
	if(tmp&8){p_output|=Table[3][0];p_output|=Table[3][1];}
	if(tmp&16){p_output|=Table[4][0];p_output|=Table[4][1];}
	if(tmp&32){p_output|=Table[5][0];p_output|=Table[5][1];}
	if(tmp&64){p_output|=Table[6][0];p_output|=Table[6][1];}
	if(tmp&128){p_output|=Table[7][0];p_output|=Table[7][1];}

	tmp=p_input>>8;
	if(tmp&1){p_output|=Table[ 8][0];p_output|=Table[ 8][1];}
	if(tmp&2){p_output|=Table[ 9][0];p_output|=Table[ 9][1];}
	if(tmp&4){p_output|=Table[10][0];p_output|=Table[10][1];}
	if(tmp&8){p_output|=Table[11][0];p_output|=Table[11][1];}
	if(tmp&16){p_output|=Table[12][0];p_output|=Table[12][1];}
	if(tmp&32){p_output|=Table[13][0];p_output|=Table[13][1];}
	if(tmp&64){p_output|=Table[14][0];p_output|=Table[14][1];}
	if(tmp&128){p_output|=Table[15][0];p_output|=Table[15][1];}

	
	tmp=p_input>>16;
	if(tmp&1){p_output|=Table[16][0];p_output|=Table[16][1];}
	if(tmp&2){p_output|=Table[17][0];p_output|=Table[17][1];}
	if(tmp&4){p_output|=Table[18][0];p_output|=Table[18][1];}
	if(tmp&8){p_output|=Table[19][0];p_output|=Table[19][1];}
	if(tmp&16){p_output|=Table[20][0];p_output|=Table[20][1];}
	if(tmp&32){p_output|=Table[21][0];p_output|=Table[21][1];}
	if(tmp&64){p_output|=Table[22][0];p_output|=Table[22][1];}
	if(tmp&128){p_output|=Table[23][0];p_output|=Table[23][1];}

	tmp=p_input>>24;
	if(tmp&1){p_output|=Table[24][0];p_output|=Table[24][1];}
	if(tmp&2){p_output|=Table[25][0];p_output|=Table[25][1];}
	if(tmp&4){p_output|=Table[26][0];p_output|=Table[26][1];}
	if(tmp&8){p_output|=Table[27][0];p_output|=Table[27][1];}
	if(tmp&16){p_output|=Table[28][0];p_output|=Table[28][1];}
	if(tmp&32){p_output|=Table[29][0];p_output|=Table[29][1];}
	if(tmp&64){p_output|=Table[30][0];p_output|=Table[30][1];}
	if(tmp&128){p_output|=Table[31][0];p_output|=Table[31][1];}
} 

void Permutation(LL p_input, LL& p_output, const LL* Table)
{    
    p_output=0;
	byte tmp;

	tmp=p_input;
	if(tmp&1)p_output|=Table[0];
	if(tmp&2)p_output|=Table[1];
	if(tmp&4)p_output|=Table[2];
	if(tmp&8)p_output|=Table[3];
	if(tmp&16)p_output|=Table[4];
	if(tmp&32)p_output|=Table[5];
	if(tmp&64)p_output|=Table[6];
	if(tmp&128)p_output|=Table[7];

	tmp=p_input>>8;
	if(tmp&1)p_output|=Table[8];
	if(tmp&2)p_output|=Table[9];
	if(tmp&4)p_output|=Table[10];
	if(tmp&8)p_output|=Table[11];
	if(tmp&16)p_output|=Table[12];
	if(tmp&32)p_output|=Table[13];
	if(tmp&64)p_output|=Table[14];
	if(tmp&128)p_output|=Table[15];

	tmp=p_input>>16;
	if(tmp&1)p_output|=Table[16];
	if(tmp&2)p_output|=Table[17];
	if(tmp&4)p_output|=Table[18];
	if(tmp&8)p_output|=Table[19];
	if(tmp&16)p_output|=Table[20];
	if(tmp&32)p_output|=Table[21];
	if(tmp&64)p_output|=Table[22];
	if(tmp&128)p_output|=Table[23];

	tmp=p_input>>24;
	if(tmp&1)p_output|=Table[24];
	if(tmp&2)p_output|=Table[25];
	if(tmp&4)p_output|=Table[26];
	if(tmp&8)p_output|=Table[27];
	if(tmp&16)p_output|=Table[28];
	if(tmp&32)p_output|=Table[29];
	if(tmp&64)p_output|=Table[30];
	if(tmp&128)p_output|=Table[31];
	
	tmp=p_input>>32;
	if(tmp&1)p_output|=Table[32];
	if(tmp&2)p_output|=Table[33];
	if(tmp&4)p_output|=Table[34];
	if(tmp&8)p_output|=Table[35];
	if(tmp&16)p_output|=Table[36];
	if(tmp&32)p_output|=Table[37];
	if(tmp&64)p_output|=Table[38];
	if(tmp&128)p_output|=Table[39];
	
	tmp=p_input>>40;
	if(tmp&1)p_output|=Table[40];
	if(tmp&2)p_output|=Table[41];
	if(tmp&4)p_output|=Table[42];
	if(tmp&8)p_output|=Table[43];
	if(tmp&16)p_output|=Table[44];
	if(tmp&32)p_output|=Table[45];
	if(tmp&64)p_output|=Table[46];
	if(tmp&128)p_output|=Table[47];

	tmp=p_input>>48;
	if(tmp&1)p_output|=Table[48];
	if(tmp&2)p_output|=Table[49];
	if(tmp&4)p_output|=Table[50];
	if(tmp&8)p_output|=Table[51];
	if(tmp&16)p_output|=Table[52];
	if(tmp&32)p_output|=Table[53];
	if(tmp&64)p_output|=Table[54];
	if(tmp&128)p_output|=Table[55];
	
	tmp=p_input>>56;
	if(tmp&1)p_output|=Table[56];
	if(tmp&2)p_output|=Table[57];
	if(tmp&4)p_output|=Table[58];
	if(tmp&8)p_output|=Table[59];
	if(tmp&16)p_output|=Table[60];
	if(tmp&32)p_output|=Table[61];
	if(tmp&64)p_output|=Table[62];
	if(tmp&128)p_output|=Table[63];
}

void des(LL& p_data, LL p_key, LL& p_output, DES_MODE mode)   
{   
	LL mask=0;
	LL tmp=0;
	byte tmp2;

    LL key_tmp=0;//7     
    LL sub_key=0;//6      
   
    unsigned int p_left=0;  
    unsigned int p_right=0; 
   
    LL p_right_ext=0;//8   
    unsigned int p_right_s=0; 

    byte loop = 0; 
    byte s_loop = 0;          
    byte s_line = 0;   
    byte s_row = 0;   
    byte s_bit;
	
    Permutation(p_key, key_tmp, Table_PC1x);   
   
      
    Permutation(p_data, p_output, Table_IPx);   
	
    p_left  = p_output;         
    p_right = p_output>>32;  
	
    for(loop = 0; loop < 16; loop++)   
    {   
         
        move_bits(key_tmp, 0, 27, Table_Move[mode][loop]); 
		//!! 1 for 28
        move_bits(key_tmp, 1, 55, Table_Move[mode][loop]);   
		
        Permutation(key_tmp, sub_key, Table_PC2x);   
		 
        PermutationE(p_right, p_right_ext, Table_Ex);   
		
        p_right_ext^=sub_key;   

        for(s_loop = 0; s_loop < 4; s_loop++)   
        {   
			s_bit = s_loop * 12;   
			tmp2=p_right_ext>>s_bit;
            s_line = qline[tmp2];
            s_row  = qrow[tmp2];   
			
			tmp=Table_SBOX[s_loop * 2][s_line][s_row];
            s_bit += 6;      
            tmp2=p_right_ext>>s_bit;
            s_line = qline[tmp2];
            s_row  = qrow[tmp2];  
             
            tmp <<= 4; 
			tmp|=Table_SBOX[(s_loop * 2) + 1][s_line][s_row];
			tmp=qrev[tmp];
			p_right_s&=qbytemasknot[s_loop];
			p_right_s|= tmp<<(s_loop*8); 
			
        }  
        
        Permutation(p_right_s, p_right_ext, Table_Px);   
   
        p_right_ext^=p_left;    

		p_left=p_right;
		p_right=p_right_ext;
    }   
	p_right_ext=p_left;
	p_right_ext=(p_right_ext<<32)|p_right;
    
    Permutation(p_right_ext, p_output, Table_InverseIPx);           
}

void desEn(LL& data, LL& en_data){
    des(data, key11, en_data, DES_ENCRYPT);   
    des(en_data, key21, data, DES_DECRYPT);   
    des(data, key31, en_data, DES_ENCRYPT);   
}

void desDe(LL& en_data, LL& de_data){
    des(en_data, key31, de_data, DES_DECRYPT);   
    des(de_data, key21, en_data, DES_ENCRYPT);   
    des(en_data, key11, de_data, DES_DECRYPT);   
}

LL key2LL(byte*a)
{
	LL res=0;
	for(int i=7;i>=0;i--)
	{
		res<<=8;
		res|=qrev[a[i]];
	}
	return res;
}

void LL2Key(LL a,byte*ret)
{
	byte v;
	for(int i=0;i<8;i++)
	{
		v=a;
		ret[i]=qrev[v];
		a>>=8;
	}
}
