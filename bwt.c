
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int my_str_cmp(const void *a, const void *b);
int my_tuple_cmp(const void *a, const void *b);

char *bwt_encode(const char *src);
char *bwt_decode(const char *dst);

int main(int argc, char **argv) {
	char *encoded;
	char *decoded;

	if (argc < 2) {
		printf("usage: ./bin str-to-compress\n");
		return 1;
	}

	// Encode the string
	encoded = bwt_encode(argv[1]);
	printf("encoded string: %s\n", encoded);

	// Decode the encoded string
	decoded = bwt_decode(encoded);
	printf("decoded string: %s\n", decoded);

	free(encoded);
	free(decoded);

	return 0;
}

int my_str_cmp(const void *a, const void *b) {
	return strcmp(*(const char **)a, *(const char **)b);
}

char *bwt_encode(const char *in) {
	int len, i;
	char **cycles;
	char *src;
	char *result;

	len = strlen(in) + 1;
	src = (char *)malloc((len+1) * sizeof(char));
	result = (char *)malloc((len+1) * sizeof(char));
	src[len] = '\0';
	result[len] = '\0';
	strcpy(src, in);
	src[len-1] = '$';

	cycles = (char **)malloc(len * sizeof(char *));
	for (i = 0; i < len; i++) {
		cycles[i] = (char *)malloc((len+1) * sizeof(char));
		cycles[i][len] = '\0';
	}

	// Get the cycles
	printf("\nunsorted:\n");
	for (i = 0; i < len; i++) {
		strncpy(cycles[i], src + i, len - i);
		strncpy(cycles[i] + len - i, src, i);
		printf("%s\n", cycles[i]);
	}

	// Sort the cycles and print
	printf("\nsorted:\n");
	qsort(cycles, len, sizeof(char *), my_str_cmp);
	for (i = 0; i < len; i++) {
		result[i] = cycles[i][len-1];
		printf("%s\n", cycles[i]);
	}

	// Clean up
	free(src);
	for (i = 0; i < len; i++) {
		free(cycles[i]);
	}
	free(cycles);
	
	return result;
}

struct tuple {
	char c;
	int i;
};

int my_tuple_cmp(const void *a, const void *b) {
	const struct tuple *ta = (const struct tuple *)a;
	const struct tuple *tb = (const struct tuple *)b;
	return ta->c - tb->c;
}

char *bwt_decode(const char *C) {
	char *result;
	int len, i, j;
	struct tuple *tuples;
	int *N;

	len = strlen(C);
	result = (char *)malloc((len+1) * sizeof(char));
	for (i = 0; i < len+1; i++) result[i] = '\0';

	// Load array N
	tuples = (struct tuple *)malloc(len * sizeof(struct tuple));
	N = (int *)malloc(len * sizeof(int));
	for (i = 0; i < len; i++) {
		tuples[i].c = C[i];
		tuples[i].i = i;
	}

	qsort(tuples, len, sizeof(struct tuple *), my_tuple_cmp);
	for (i = 0; i < len; i++) {
		N[i] = tuples[i].i;
	}

	// Find the index of '$' in `C`
	for (j = 0; j < len; j++) {
		if (C[j] == '$') break;
	}

	// Print the result
	i = 0;
	do  {
		j = N[j];
		result[i++] = C[j];
	} while (C[j] != '$');

	return result;
}
