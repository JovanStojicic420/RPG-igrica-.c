#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   // Za _getch() - omogucava kretanje bez lupanja Entera
#include <string.h>  // Za strcpy() - rad sa tekstom u inventaru

// Konstante za lakse upravljanje dimenzijama
#define MAX_ROWS 10
#define MAX_COLS 20

// --- STRUKTURA IGRACA ---
// Ovde cuvamo sve bitne podatke o liku da ne bismo imali brdo globalnih promenljivih
typedef struct {
    int hp;             // Trenutno zdravlje (Health Points)
    int max_hp;         // Maksimalni HP
    int napad;          // Jacina napada
    int odbrana;        // Jacina odbrane
    char inventar[7][30]; // Niz od 7 stringova za inventar
    int broj_predmeta;   // Brojac popunjenih mesta u inventaru
} Igrac;

// --- GLOBALNE PROMENLJIVE ---
char map[MAX_ROWS][MAX_COLS];
int rows = 0;
int px, py; // Pozicija igraca (x, y)
Igrac heroj = {100, 100, 10, 10, {0}, 0}; // Inicijalizacija po pravilima zadatka

// FUNKCIJA: Ucitava mapu iz fajla u 2D matricu
void ucitaj() {
    FILE *f = fopen("nivo1.txt", "r");
    if (f == NULL) {
        perror("Greska pri otvaranju nivo1.txt");
        exit(1);
    }

    rows = 0;
    // Citamo red po red dok ima mesta u matrici
    while (fgets(map[rows], MAX_COLS, f) && rows < MAX_ROWS) {
        // Ciscenje "invisible" karaktera na kraju reda (enter)
        for(int i=0; i<MAX_COLS; i++) {
            if(map[rows][i] == '\n' || map[rows][i] == '\r') {
                map[rows][i] = '\0'; // Terminiramo string
            }
        }
        rows++;
    }
    fclose(f);

    // Pronalazimo pocetnu poziciju igraca (@)
    for (int y = 0; y < rows; y++) {
        for (int x = 0; map[y][x] != '\0'; x++) {
            if (map[y][x] == '@') {
                px = x; py = y;
            }
        }
    }
}

// FUNKCIJA: Ispisuje inventar na ekran (poziva se na 'i')
void prikaziInventar() {
    system("cls");
    printf("=== MOJ INVENTAR (%d/7) ===\n", heroj.broj_predmeta);
    for(int i = 0; i < heroj.broj_predmeta; i++) {
        printf("%d. %s\n", i + 1, heroj.inventar[i]);
    }
    if(heroj.broj_predmeta == 0) printf("Prazno je...\n");
    printf("\nPritisni bilo koji taster za povratak u igru...");
    _getch();
}

// FUNKCIJA: Logika borbe (pokrece se susretom sa 'E')
void pokreniBorbu() {
    system("cls");
    printf("!!! NAISAO SI NA NEPRIJATELJA !!!\n");
    printf("Statistika: HP: %d | Napad: %d | Odbrana: %d\n", heroj.hp, heroj.napad, heroj.odbrana);

    // OVDE JEDAN KOLEGA DODAJE LOGIKU ZA RUNDE
    printf("\n(Borba u izradi... pritisni taster da ga pobedis magijom)\n");
    _getch();
}

// FUNKCIJA: Glavno crtanje mape i status bara
void crtaj() {
    system("cls");
    // Status bar na vrhu ekrana
    printf("PLAYER STATS -> HP: %d/%d | Napad: %d | Odbrana: %d\n", heroj.hp, heroj.max_hp, heroj.napad, heroj.odbrana);
    printf("----------------------------------\n");

    // Ispis matrice (mape)
    for (int i = 0; i < rows; i++) {
        printf("%s\n", map[i]);
    }
    printf("----------------------------------\n");
    printf("WASD-Kretanje | I-Inventar | Q-Izlaz\n");
}

int main() {
    ucitaj(); // Prvo ucitavanje fajla

    while (1) {
        crtaj(); // Osvezavanje prikaza

        char c = _getch(); // Hvatanje tastera

        // LOGIKA IZLAZA: Obavezna potvrda (d/n)
        if (c == 'q') {
            printf("\nStvarno zelis da izadjes? (d/n): ");
            char provera = _getch();
            if(provera == 'd') break;
            else continue;
        }

        // Otvaranje inventara
        if (c == 'i') {
            prikaziInventar();
            continue;
        }

        // Racunanje potencijalne nove pozicije
        int nx = px, ny = py;
        if (c == 'w') ny--;
        if (c == 's') ny++;
        if (c == 'a') nx--;
        if (c == 'd') nx++;

        // --- PROVERA INTERAKCIJE ---
        char polje = map[ny][nx]; // Gledamo sta je na mestu gde hocemo da stanemo

        if (polje == '#') continue; // Ako je zid, ostajemo gde smo

        if (polje == 'E') {
            pokreniBorbu(); // Pozivamo funkciju za borbu
            // Ovde mozete dodati map[ny][nx] = '.' ako neprijatelj nestane
        }
        else if (polje == '$') {
            // Logika za skupljanje predmeta
            if(heroj.broj_predmeta < 7) {
                strcpy(heroj.inventar[heroj.broj_predmeta], "Obican mac");
                heroj.broj_predmeta++;
                map[ny][nx] = '.'; // Predmet nestaje sa mape
                printf("\nPokupio si predmet!");
            } else {
                printf("\nInventar je pun!");
                _getch();
            }
        }
        else if (polje == '>') {
            printf("\nPRONASAO SI IZLAZ! Prelazak na nivo 2...\n");
            _getch();
            break;
        }

        // --- ZVANICNO POMERANJE ---
        map[py][px] = '.'; // Staro mesto postaje pod
        px = nx; py = ny;  // Koordinate postaju nove
        map[py][px] = '@'; // Na novom mestu crtamo igraca
    }

    return 0;
}
