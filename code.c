#include <stdio.h>      // printf, sscanf
#include <string.h>     // strlen, strncmp
#include <openssl/md5.h> // MD5, MD5_DIGEST_LENGTH
#include <stdint.h>
typedef unsigned char byte;

void print_digest(byte * hash){ 
 
    int x; 
  
    for(x = 0; x < MD5_DIGEST_LENGTH; x++) 
        printf("%02x", hash[x]); 
 
    printf("\n"); 
}

// This procedure generate all combinations of possible letters
void iterate(byte * hash1, byte * hash2, char *str, int idx, int len, int *ok) { 
 
    int c; 
  
    // 'ok' determines when the algorithm matches. 
 
    if(*ok) return; 
    
    if (idx < (len - 1)) { 
    
    
    // Iterate for all letter combination. 
    
    
    for (c = 0; c < strlen(letters) && *ok==0; ++c) { 
    
        str[idx] = letters[c]; 
    
        // Recursive call 
        iterate(hash1, hash2, str, idx + 1, len, ok);
    } 
 
    } else { 
        
        // Include all last letters and compare the hashes. 
        
        for (c = 0; c < strlen(letters) && *ok==0; ++c) 
        { 
            
            str[idx] = letters[c]; 
            MD5((byte *) str, strlen(str), hash2); 
            
            if(strncmp((char*)hash1, (char*)hash2, MD5_DIGEST_LENGTH) == 0){ 

                printf("found: %s\n", str);  
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
