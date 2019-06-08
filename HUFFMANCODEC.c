
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Huffman tree structure
typedef struct hufftree
{
    struct hufftree *left;
    struct hufftree *right;
    int symbol;
    int freq;
} hufftree;


int Compare(const void *a, const void *b)
{
    const hufftree **x = (const hufftree **) a, **y = (const hufftree **) b;
    if((*x)->freq == (*y)->freq) 
	return 0;
    else if ((*x)->freq < (*y)->freq) 
	return 1 ;
    else return -1;
}

// build and return a Huffman tree 
hufftree *Build_tree(int frequency[])
{
    int i=0, length = 0;
    hufftree *queue[257];
    
   while(i<257)
    {
        if(frequency[i])
        {
            hufftree *node = (hufftree *)calloc(1, sizeof(hufftree));
            node->symbol = i;
            node->freq = frequency[i];

            queue[length++] = node;
        }
	i++;
    }
    
    while(length > 1)
    {
        hufftree *node = (hufftree *)malloc(sizeof(hufftree));
        
        // sort 
        qsort(queue, length, sizeof(hufftree *), Compare);
        
       
        node->left = queue[--length];
        node->right = queue[--length];
	//add nodes with least frequency 
        node->freq = node->left->freq + node->right->freq;
        
        queue[length++] = node; 
    }
    
    return queue[0]; 
}

void Free_tree(hufftree *tree)
{
    if(tree)
    {
        Free_tree(tree->left);
        Free_tree(tree->right);
        free(tree);
    }
}
char *Concat(char *prefix, char symbol)
{
    char *result = malloc(strlen(prefix) + 2);
    sprintf(result, "%s%c", prefix, symbol);
    return result;
}


// traverse the Huffman tree 
void Traverse_tree(hufftree *tree, char **table, char *prefix)
{
if(tree->left ==0)
{
if(tree->right==0)
{
 table[tree->symbol] = prefix;
}}
else
{
if(tree->left)
Traverse_tree(tree->left, table, Concat(prefix, '0'));
if(tree-> right) 
Traverse_tree(tree->right, table, Concat(prefix, '1'));
free(prefix);
 }
}

// build a table 
char **Build_table(int frequency[])
{
    static char *table[257];
    char *prefix = (char *)calloc(1, sizeof(char));
    hufftree *tree = Build_tree(frequency);
    Traverse_tree(tree, table, prefix);
    Free_tree(tree);
    
    return table;
}
void Write_header(FILE *fpt2, int frequency[])
{
    int i=0, count = 0;
while(i<257) 
{
if(frequency[i]) 
{
count++;
}
i++;
}
 fprintf(fpt2, "%d\n", count);
i=0;
while(i<257)
{
        if(frequency[i])
{
 fprintf(fpt2, "%d %d\n", i, frequency[i]);
}
i++;
}
}

int *Read_header(FILE *fpt1)
{
    static int frequency[257];
    int i, count, symbol, freq,last;
    
    fscanf(fpt1, "%d", &count);
    
    for(i = 0; i < count; i++)
    {
      fscanf(fpt1, "%d ", &symbol);
      fscanf(fpt1, "%d ", &freq) ;
      frequency[symbol] = freq;
    }
   fscanf(fpt1, "%d", &last);
    return frequency;
}

// write bit 
void Write_bit(const char *code, FILE *fpt2)
{
    static int bit= 0, bitcount = 0;
    
    while(*code)
    {
        
        bit = bit * 2 + *code - '0';
        bitcount++;
   
        if(bitcount == 8)
        {
            fputc(bit, fpt2);
            bit = 0,bitcount=0;
        }
        code++;
    }
}

// read a  bit
int get_bit(FILE *fpt1)
{
    
    static int bit = 0, bitcount = 0;
    int nextbit;
    
    if(bitcount == 0)
    {
        bit = fgetc(fpt1);
        bitcount = (1 << (8 - 1));
    }
    
    nextbit = bit/ bitcount;
    bit %= bitcount;
    bitcount /= 2;
    
    return nextbit;
}

// decode a character
 
int Decode_char(FILE *fpt1, hufftree *tree)
{
    while(tree->left || tree->right)
    {
        if(get_bit(fpt1)) tree = tree->right;
        else tree = tree->left;  
    }
    return tree->symbol;
}

void Decode(FILE *fpt1, FILE *fpt2)
{
    int *frequency, c;
    hufftree *tree;
    
    frequency= Read_header(fpt1);
    tree = Build_tree(frequency);
    
    while((c = Decode_char(fpt1, tree)) != 256)
        fputc(c, fpt2); 
    Free_tree(tree);
}

void Encode(FILE *fpt1, FILE *fpt2)
{
    int c,i, frequency[257] = { 0 };
    char **table;
    
    while((c = fgetc(fpt1)) !=EOF) 
frequency[c]++;
    
    frequency[256] = 1;
    rewind(fpt1);
    
    table = Build_table(frequency);
    Write_header(fpt2, frequency);
    
    while((c = fgetc(fpt1)) != EOF)
        Write_bit(table[c],fpt2);
 Write_bit(table[256], fpt2);
 Write_bit("0000000", fpt2);    
 for(i = 0; i < 257; i++) 
if(table[i]) 
free(table[i]);

}
int main(int argc, char *argv[])
{
    FILE *fpt1, *fpt2;
    
if (argc != 4)
{
printf("Invalid no of arguments");
exit(0);
}
if ((fpt1=fopen(argv[2],"rb"))==NULL)
{ 
printf("unable to open %s for reading \n",argv[2]);
exit(0);
}
else if((fpt2 = fopen(argv[3], "wb"))==NULL)
{
printf("unable to open %s for reading \n",argv[3]);
exit(0);
}
if(strcmp(argv[1], "-c")==0)
Encode(fpt1, fpt2);
else Decode(fpt1,fpt2);
    fclose(fpt1);
    fclose(fpt2);    
    return 0;
} 
