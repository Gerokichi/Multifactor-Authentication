#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Eat(char *sosud, size_t sosud_size);
unsigned int AuraXHash(const char* pass);
bool Digest(const char* name, const char* pass, const char* key);

void SystemResponse(bool error);
void Years_In_Dagestan(unsigned int boychik, char *man, int length);


int main() {
    char username[26];
    char password[51];
    char key[5];

    char *mode[] = {"meno", "heslo", "overovaci kluc", NULL};
    char *sosud[] = {username, password, key};
    size_t sosud_size[] = {sizeof(username), sizeof(password), sizeof(key)};

    for (int i = 0; mode[i] != NULL; i++) {
        printf("%s:", mode[i]);
        Eat(sosud[i], sosud_size[i]);
    }

    // printf("Name: %s\n", username);
    // printf("Pass: %s\n", password);
    // printf("Key: %s\n", key);

    unsigned int pass_faberge = AuraXHash(password);
    // printf("Hash pass: %u\n", pass_faberge);

    int len = snprintf(NULL, 0, "%u", pass_faberge);
    char *real_pass = malloc(len + 1);
    Years_In_Dagestan(pass_faberge, real_pass, len);

    SystemResponse(!Digest(username, real_pass, key));

    free(real_pass);
    return 0;
}


void Eat(char *sosud, size_t sosud_size) {
    if (fgets(sosud, (int)sosud_size, stdin) != NULL) {
        size_t len = strlen(sosud);
        if (len > 0 && sosud[len - 1] == '\n') {
            sosud[len - 1] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

unsigned int AuraXHash(const char* pass) {
    unsigned int hash = 0x41757261;
    size_t pass_len = strlen(pass);

    for (size_t i = 0; i < pass_len; i++) {
        unsigned char key = (unsigned char)(pass[i % pass_len] + i);
        hash ^= (unsigned char)pass[i];
        hash = hash << 7 | hash >> 25;
        hash = hash * 31 + key + (unsigned int)i;
    }

    return hash;
}

bool Digest(const char* name, const char* pass, const char* key) {
    FILE *user_data = fopen("hesla.csv", "r");
    if (user_data == NULL) return false;

    FILE *new_user_data = fopen("temp_hesla.csv", "w");
    if (new_user_data == NULL) {
        fclose(user_data);
        return false;
    }

    char user[256];
    bool permit = false;

    while (fgets(user, sizeof(user), user_data) != NULL) {
        user[strcspn(user, "\r\n")] = 0;

        char copy_user[strlen(user) + 1];
        strcpy(copy_user, user);

        char* username = strtok(user, ":");
        char* userpass = strtok(NULL, ":");

        if (username && userpass && strcmp(name, username) == 0 && strcmp(pass, userpass) == 0) {
            fprintf(new_user_data, "%s:%s:", username, userpass);

            char* userkey = strtok(NULL, ",");

            bool first_key = true;
            while (userkey != NULL) {
                if (strcmp(key, userkey) == 0) {
                    permit = true;
                } else {
                    if (!first_key) fprintf(new_user_data, ",");
                    fprintf(new_user_data, "%s", userkey);
                    first_key = false;
                }
                userkey = strtok(NULL, ",");
            }

            fprintf(new_user_data, "\n");
        } else {
            fprintf(new_user_data, "%s\n", copy_user);
        }
    }

    fclose(new_user_data);
    fclose(user_data);
    return permit;
}


void SystemResponse(const bool error) {
    if (error) {
        printf("chyba\n");
    }
    else {
        printf("ok\n");
    }
}

void Years_In_Dagestan(unsigned int boychik, char *man, int length) {
    for (int i = length - 1; i >= 0; i--) {
        man[i] = boychik % 10 + '0';
        boychik /= 10;
    }
}