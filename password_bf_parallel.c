#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <omp.h>
 
#define MAX 10
 
typedef unsigned char byte;
 
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
 
/*
 * Print a digest of MD5 hash.
*/
void print_digest(byte * hash){
	int x;
 
	for(x = 0; x < MD5_DIGEST_LENGTH; x++)
        	printf("%02x", hash[x]);
	printf("\n");
}
 
/*
 * This procedure generate all combinations of possible letters
*/
void iterate(byte * hash1, byte * hash2, char *str, int idx, int len, int *ok) {
	int c;
 
	// 'ok' determines when the algorithm matches.
	if(*ok) return;
 
	//cada thread tem a sua propria str e hash2
	if (idx == 0) {
		#pragma omp parallel for schedule(dynamic) shared(hash1, str, ok, len)
		for (c = 0; c < strlen(letters); ++c) {
			if (*ok) continue;
 
			char str_local[MAX+1];
			byte hash2_local[MD5_DIGEST_LENGTH];
			memcpy(str_local, str, len+1);
			str_local[0] = letters[c];
 
			if (len - 1 > 0) {
				iterate(hash1, hash2_local, str_local, 1, len, ok);
			} else {
				MD5((byte *) str_local, strlen(str_local), hash2_local);
				if(strncmp((char*)hash1, (char*)hash2_local, MD5_DIGEST_LENGTH) == 0){
					#pragma omp critical
					{
						printf("found: %s\n", str_local);
						print_digest(hash2_local);
						*ok = 1;
					}
				}
			}
		}
		return;
	}

	if (idx < (len - 1)) {
		// Iterate for all letter combination.
		for (c = 0; c < strlen(letters) && *ok==0; ++c) {
			str[idx] = letters[c];
			// Recursive call
			iterate(hash1, hash2, str, idx + 1, len, ok);
		}
	} else {
		// Include all last letters and compare the hashes.
		for (c = 0; c < strlen(letters) && *ok==0; ++c) {
			str[idx] = letters[c];
			MD5((byte *) str, strlen(str), hash2);
			if(strncmp((char*)hash1, (char*)hash2, MD5_DIGEST_LENGTH) == 0){
				printf("found: %s\n", str);
				print_digest(hash2);
				*ok = 1;
			}
		}
	}
}
 
/*
 * Convert hexadecimal string to hash byte.
*/
void strHex_to_byte(char * str, byte * hash){
	char * pos = str;
	int i;
 
	for (i = 0; i < MD5_DIGEST_LENGTH/sizeof *hash; i++) {
		sscanf(pos, "%2hhx", &hash[i]);
		pos += 2;
	}
}
 
int main(int argc, char **argv) {
	char str[MAX+1];
	int lenMax = MAX;
	int len;
	int ok = 0, r;
	char hash1_str[2*MD5_DIGEST_LENGTH+1];
	byte hash1[MD5_DIGEST_LENGTH]; // password hash
	byte hash2[MD5_DIGEST_LENGTH]; // string hashes
 
	// Input:
	r = scanf("%s", hash1_str);
 
	// Check input.
	if (r == EOF || r == 0)
	{
		fprintf(stderr, "Error!\n");
		exit(1);
	}
 
	// Convert hexadecimal string to hash byte.
	strHex_to_byte(hash1_str, hash1);
 
	memset(hash2, 0, MD5_DIGEST_LENGTH);
	print_digest(hash1);
 
	// Generate all possible passwords of different sizes.
	for(len = 1; len <= lenMax; len++){
		memset(str, 0, len+1);
		iterate(hash1, hash2, str, 0, len, &ok);
	}
}
 