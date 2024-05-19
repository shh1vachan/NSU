#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <string.h>

#define NODE struct node


NODE
{
	unsigned char data;
	unsigned freq;
	NODE* left, * right;
};


NODE* newNode(unsigned char data, unsigned freq)
{
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	temp->left = NULL;
	temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	return temp;
}


void prepare(unsigned char alp[], int freq[])
{
	for (int i = 0; i < 256; i++)
	{
		freq[i] = 0;
	}
	for (int i = 0; i < 256; i++)
	{
		alp[i] = i;
	}
}


void saveTree(NODE* node, FILE* file)
{
	if (node == NULL)
    {
		fputc('0', file);
		return;
	}
	fputc('1', file);
	fputc(node->data, file);
	fwrite(&node->freq, sizeof(unsigned), 1, file);
	saveTree(node->left, file);
	saveTree(node->right, file);
}


NODE* loadTree(FILE* file)
{
	int isNode = fgetc(file);
	if (isNode == '0') {
		return NULL;
	}
	NODE* node = newNode(fgetc(file), 0);
	fread(&node->freq, sizeof(unsigned), 1, file);
	node->left = loadTree(file);
	node->right = loadTree(file);
	return node;
}


struct MinHeap
{
	unsigned long long  size;
	unsigned long long  capacity;
	NODE** array;
};


struct MinHeap* createMinHeap(unsigned capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (NODE**)malloc(minHeap->capacity * sizeof(NODE*));
	return minHeap;
}


void swapNode(NODE** a, NODE** b)
{
	NODE* t = *a;
	*a = *b;
	*b = t;
}


void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swapNode(&minHeap->array[smallest], &minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}


NODE* extractMin(struct MinHeap* minHeap)
{
	NODE* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];
	--minHeap->size;
	minHeapify(minHeap, 0);
	return temp;
}


void insertMinHeap(struct MinHeap* minHeap, NODE* minHeapNode)
{
	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	minHeap->array[i] = minHeapNode;
}


void buildMinHeap(struct MinHeap* minHeap)
{
	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}


int isSizeOne(struct MinHeap* minHeap)
{
	return (minHeap->size == 1);
}


struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size)
{
	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i)
		minHeap->array[i] = newNode(data[i], freq[i]);

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}


NODE* buildHuffmanTree(unsigned char data[], int freq[], int size)
{
	NODE* left, * right, * top;

	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

	while (!isSizeOne(minHeap)) {
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	return extractMin(minHeap);
}


void LoadFile(char* fn, unsigned char alp[], int freq[])
{
	prepare(alp, freq);
	FILE* fp = fopen(fn, "rb");
	if (fp == NULL) {
		printf("Ошибка открытия файла\n");
		return;
	}
	int c;

	while (true)
    {
		c = fgetc(fp);
		if (feof(fp))
			break;

		freq[c]++;
	}
	fclose(fp);
}


void preparegetBCode(unsigned char _findsymb, int *resLen, unsigned int* resgetBCode, bool *ex, unsigned char* findsymb) {

	*resLen = 0;
	*resgetBCode = 0;
	*ex = false;
	*findsymb = _findsymb;
}


bool pathGetSymbol(unsigned int findvalue, int *len, NODE* root, unsigned char* ressymb, int pointer)
{
	bool res = true;
	while ((root->left != NULL) || (root->right != NULL))
	{
		unsigned int needbit = pow(2, 31 - *len);
		unsigned int bit = findvalue & needbit;
		if (bit == 0)
			root = root->left;
		else
			root = root->right;
		*len+=1;
		if (pointer - *len == 0) break;
	}
	if (!(root->left) && (!(root->right)))
        *ressymb = root->data;
	else
	{
		*len = 0;
		return false;
	}
	return true;
}


void  getBCode(NODE* root, int arr[], int top, bool *ex, unsigned int* resgetBCode, int* resLen, unsigned char findsymb) {
    if (*ex) return;
    if (root->left) {
        arr[top] = 0;
        getBCode(root->left, arr, top + 1, ex, resgetBCode, resLen, findsymb);
    }

    if (root->right) {
        arr[top] = 1;
        getBCode(root->right, arr, top + 1, ex, resgetBCode, resLen, findsymb);
    }

    if (!(root->left) && !(root->right)) {
        if (findsymb == root->data) {
            *resLen = top;
            *ex = true;
            for (int i = 0; i < top; i++) {
                *resgetBCode += arr[i] * pow(2, top - i - 1);
            }
        }
    }
}


void add(unsigned int* source, int* pointer, unsigned int value, int len)
{
	value = value << (32 - *pointer - len);
	*pointer = *pointer + len;
	*source = *source | value;
}


unsigned long long  freadtotalsize(char* resfn)
{
	FILE* fresfn = fopen(resfn, "rb");
	if (fseek(fresfn, -8, SEEK_END) != 0) {
		perror("Ошибка при перемещении указателя файла");
		fclose(fresfn);
		return 0;
	}
	unsigned long long last_value;

	if (fread(&last_value, sizeof(last_value), 1, fresfn) != 1) {
		perror("Ошибка при чтении данных из файла");
		fclose(fresfn);
		return 0;
	}
	return last_value;
}


void decode(char* fn, char* resfn) {
    unsigned long long TotalSize = freadtotalsize(resfn);
    FILE* fresfn = fopen(resfn, "rb");
    NODE* root = loadTree(fresfn);
    FILE* ffn = fopen(fn, "wb");
    if (ffn == NULL) {
        printf("Ошибка открытия файла\n");
        return;
    }

    unsigned short c;
    unsigned int res = 0;
    unsigned int findvalue = 0;
    unsigned int temp = 0;
    int pointer = 0;
    int len = 0;
    unsigned char ressymb;
    bool finish = false;

    while ((!finish) && (!feof(fresfn))) {
        fread(&c, 2, 1, fresfn);

        temp = c;
        temp = temp << (16 - pointer);
        pointer = pointer + 16;
        findvalue = findvalue | temp;
        while ((!finish) && (pointer >= 1)) {
            if (pathGetSymbol(findvalue, &len, root, &ressymb, pointer)) {
                fwrite(&ressymb, 1, 1, ffn);
                findvalue = findvalue << len;
                pointer -= len;
                TotalSize -= len;
                len = 0;

                if (TotalSize <= 0) {
                    finish = true;
                    break;
                }
            } else {
                break;
            }
        }
    }
    fclose(ffn);
    fclose(fresfn);
}


void encode(NODE* root, char* fn, char* resfn) {
    unsigned long long TotalSize = 0;
    FILE* ffn = fopen(fn, "rb");
    FILE* fresfn = fopen(resfn, "wb");
    saveTree(root, fresfn);
    if (ffn == NULL) {
        printf("Ошибка открытия файла\n");
        return;
    }
    unsigned char c;
    unsigned int res = 0;
    int pointer = 0;
    unsigned int resgetBCode = 0;
    int resLen = 0;
    bool ex = false;

    while (true) {
        c = fgetc(ffn);
        if (feof(ffn))
            break;
        preparegetBCode(c, &resLen, &resgetBCode, &ex, &c);
        int huffCode[100];
        int top = 0;
        getBCode(root, huffCode, top, &ex, &resgetBCode, &resLen, c);
        add(&res, &pointer, resgetBCode, resLen);
        if (pointer >= 16) {
            unsigned short cc = (unsigned short)(res >> 16);
            fwrite(&cc, 2, 1, fresfn);
            TotalSize += 16;
            pointer -= 16;
            res = res << 16;
        }
    }

    TotalSize += pointer;
    unsigned short cc = (unsigned short)(res >> 16);
    fwrite(&cc, 2, 1, fresfn);
    fwrite(&TotalSize, sizeof(TotalSize), 1, fresfn);
    fclose(ffn);
    fclose(fresfn);
}


int main(int argc, char* argv[]) {
    unsigned char alp[256];
    int freq[256];

    char* command = argv[1];
    char* i1 = argv[2];
    char* p2 = argv[3];

    if (strcmp(command, "c") == 0) {
        char* locale = setlocale(LC_ALL, "");

        LoadFile(i1, alp, freq);
        unsigned char alp1[256];
        int freq1[256];
        int ii = 0;

        for (int i = 0; i < 256; i++) {
            if (freq[i] != 0) {
                alp1[ii] = alp[i];
                freq1[ii++] = freq[i];
            }
        }
        NODE* root = buildHuffmanTree(alp1, freq1, ii);
        encode(root, i1, p2);
    } else if (strcmp(command, "d") == 0) {
        NODE* root = NULL;
        decode(p2, i1);
    }
    return 0;
}