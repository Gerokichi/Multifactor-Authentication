#include <stdio.h>

void SystemResponse(bool error);

int main() {
    SystemResponse(false);

    return 0;
}



void SystemResponse(const bool error) {
    if (error) {
        printf("chyba\n");
    }
    else {
        printf("ok\n");
    }
}