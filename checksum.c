//I [HUU NGOC CHINH CAO] ([4529434]) affirm that this
//program is entirely my own work and that I have neither developed my code together with any
//another person, nor copied any code from any other person, nor permitted my code to be copied
//or otherwise used by any other person, nor have I copied, modified, or otherwise used programs
//created by others. I acknowledge that any violation of the above terms will be treated as academic
//dishonesty


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


void bit8(char text[], int length);
void bit16(char text[], int length);
void bit32(char text[], int length);
char * stringToHex(char text[], int length);
int hexToDec(char hexVal[]);
char *decToHex(unsigned long number, int length) ;


int main (int argc, char **argv)
{
	int i;
	int check = 0;
	char filename[20];
	char *buffer = 0;
	FILE *f = NULL;
	int length;
	int error = 1;
	char bit[12];
	
	strcpy(filename, argv[1]);
	strcpy(bit, argv[2]);
	
	//Check filename valid
	if ((f = fopen(filename, "r")) == NULL)
	{
		//Error message when wrong filename
		fprintf(stderr, "%s", "Invalid filename\n");
		return 0;
	}
	
	f = fopen (filename, "rb");

	//Take all char from file to buffer
	if (f)
	{
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = malloc (length+4);
		if (buffer)
		{
			fread (buffer, 1, length, f);
		}
		fclose (f);
	}
	
	
	//check if bit vaild
	if (!strcmp(bit, "8") || !strcmp(bit, "16") || !strcmp(bit, "32"))
	{
		printf("\n");
		
		//Newline every 80 char
		for (i = 0; i < length; i++)
		{
			if ((i%80) == 0 && i != 0)
				printf("\n");
		
			printf("%c", buffer[i]);
		}
		
		//Pad with X when check length and bit is 16
		if (!strcmp(bit, "16") && (length % 2) != 0)
			check++;
		
		//Pad with X when check length and bit is 32
		if (!strcmp(bit, "32"))
		{
			check = length%4;
			check = 4 - check;
		}
		
		//Pad X
		for (i = 0; i < check; i++)
			printf("X");
		
		
		printf("\n");
		
		//bit case
		if (!strcmp(bit, "8"))
		{
			bit8(buffer, length);
			error--;
		}
		
		if (!strcmp(bit, "16"))
		{
			bit16(buffer, length);
			error--;
		}
		
		if (!strcmp(bit, "32"))
		{
			bit32(buffer, length);
			error--;
		}
	}
	
	//Error message when wrong bit
	if (error)
		fprintf (stderr, "%s", "Valid checksum sizes are 8, 16, or 32\n");
	
	return 0;
}


//When choice is 8 bits
void bit8(char text[], int length)
{
	int i;
	int total = 0;
	char c[1];
	
	for (i = 0; i < length; i++)
	{
		c[0] = text[i];
		total += hexToDec(stringToHex(c, 1));
	}
		
	printf("%2d bit checksum is %8lx for all %4d chars\n", 8, hexToDec(decToHex(total, 2)), length);
}


//When choice is 16 bits
void bit16(char text[], int length)
{
	int i = 0;
	int a = 0;
	int b = 0;
	int total = 0;
	char buffer[1024][1024];
	int plus = 0;
	
	
	while (i < length)
	{
		for (a = 0; a < 2; a++)
		{
			//Increment i by 1 each time copy to buffer;
			buffer[b][a] = text[i];
			
			//Pad with X when check length
			if ((length % 2) != 0 && i == (length))
			{
				buffer[b][a] = 'X';
				plus++;
			}
				
			i++;
		}
		b++;
	}

	//b is equal to length x of buffer
	for (i = 0; i < b; i++)
		total += hexToDec(stringToHex(buffer[i], 2));
	
	length += plus;
	printf("%2d bit checksum is %8lx for all %4d chars\n", 16, hexToDec(decToHex(total, 4)), length);
}


//When choice is 32 bits
void bit32(char text[], int length)
{
	int j;
	int i = 0;
	int a = 0;
	int b = 0;
	long total = 0;
	char buffer[1024][1024];
	int plus = 0;
	int check = 0;
	check = length%4;
	check = 4 - check;
	
	while (i < length)
	{
		for (a = 0; a < 4; a++)
		{
			//Increment i by 1 each time copy to buffer;
			buffer[b][a] = text[i];
			
			//Pad with X when check length
			if (check != 0 && i == (length))
			{
				for (j = 0; j < check; j++)
				{
					buffer[b][a] = 'X';
					a++;
					plus++;
				}
			}
				
			i++;
		}
		b++;
	}
	

	//b is equal to length x of buffer
	for (i = 0; i < b; i++)
		total += hexToDec(stringToHex(buffer[i], 4));
	
	
	length += plus;
	printf("%2d bit checksum is %8lx for all %4d chars\n", 32, hexToDec(decToHex(total, 8)), length);
}

//A function to convert string to hexadecimal
char * stringToHex(char text[], int length)
{
	char *newHex = malloc(sizeof(char)*length);
	unsigned int newDec;
	int i, j;
	
	//converting text character into Hex and adding into newHex
    for(i = 0, j = 0; i < length; i++, j+=2)
    { 
        sprintf((char*)newHex + j, "%02X", text[i]);
    }
	
    newHex[j]='\0';
	//adding NULL in the end
	
	return newHex;
}


//A function to convert hexadecimal to decimal
int hexToDec(char hexVal[]) 
{    
    int len = strlen(hexVal); 
      
    // Initializing base value to 1
    int base = 1; 
      
    unsigned int dec_val = 0; 
	int i;
      
    // Extracting characters as digits from last character 
    for (i = len-1; i >= 0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i] >= '0' && hexVal[i] <= '9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 
	
//Convert deceimal to hexadecimal function
char *decToHex(unsigned long number, int length) 
{
	int cnt,i, j;
    char *hex = malloc(sizeof(char) * 20);
	char *newHex = malloc(sizeof(char) * length);
 
    cnt = 0; 
	j = 0;

    while(number > 0)
    {
        switch(number % 16)
        {
            case 10:
                hex[cnt]='A'; break;
            case 11:
                hex[cnt]='B'; break;
            case 12:
                hex[cnt]='C'; break;
            case 13:
                hex[cnt]='D'; break;
            case 14:
                hex[cnt]='E'; break;
            case 15:
                hex[cnt]='F'; break;
            default:
                hex[cnt] = (number%16)+0x30;  //converted into char value
        }
        number = number/16;
        cnt++;
    }
 
	//reverse hex
	for (i = (--length); i >= 0; i--)
	{
		newHex[i] = hex[j];
		j++;
	}
    return newHex;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	