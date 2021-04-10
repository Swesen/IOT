/* Enkelt testprogram för att se att se att installationen
av GCC fungerat. I visual studio code kan du använda powershell:
1) Använd kommandot: gcc gcctest.c för att kompilera programmet.
2) Använd kommandot: ./a för att köra programmet.
3) Mata in din output på moodle i testet "Bekräfta att GCC fungerar"
för att komma vidare. */

#include <stdio.h>

int main(){
    printf("%c%c%c", 80-8,101,54*2);
    printf("%c%c", 108,sizeof(char)*111);
    printf(" %c%c%c%c%c", 87,sizeof(char)*111,114,108,10*10);
    
    return 0;
}