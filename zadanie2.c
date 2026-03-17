#include <stdio.h>
#include <string.h>

void Eat(char *sosud, size_t sosud_size);
unsigned int AuraXHash(const char* pass);
bool Digest(char user, unsigned int pass, const char* key);

void SystemResponse(bool error);


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

bool Digest(const char user, unsigned int pass, const char* key) {
    FILE *user_date = fopen("hesla.csv", "r");
    if (user_date == NULL) return false;

    char username[26];

    while (fgets(username, sizeof(username), user_date) != NULL) {
        char* name = strtok(username, ":");


    }

    fclose(user_date);
    return true;
}


void SystemResponse(const bool error) {
    if (error) {
        printf("chyba\n");
    }
    else {
        printf("ok\n");
    }
}