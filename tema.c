/*
Grupa: 161
Problema: 3 - Divide et Impera: Mini "motor de cautare" (cu relevanta)
Nume: Rusănescu Gabriel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_CALE 256
#define MAX_TERMEN 31

typedef struct {
    char *cale;
    int scor; 
} RezultatDocument;

typedef struct {
    RezultatDocument *date;
    int numar_rezultate;
} RezultatCautare;

void elimina_newline(char *sir) {
    int lungime=strlen(sir);
    while(lungime>0 && (sir[lungime-1]=='\n' || sir[lungime-1]=='\r')) {
        sir[lungime-1]='\0';
        lungime--;
    }
}

int calculeaza_relevanta(const char *nume_fisier, const char *termen) {
    FILE *fisier=fopen(nume_fisier, "r");
    if(!fisier) return 0;
    char linie[MAX_TERMEN];
    int numar_aparitii=0;
    while(fgets(linie, sizeof(linie), fisier)) {
        elimina_newline(linie);
        if(strcmp(linie, termen)==0) numar_aparitii++;
    }
    fclose(fisier);
    return numar_aparitii;
}

RezultatCautare interclaseaza_rezultate(RezultatCautare stanga, RezultatCautare dreapta) {
    RezultatCautare rez;
    rez.numar_rezultate = stanga.numar_rezultate + dreapta.numar_rezultate;
    if(rez.numar_rezultate>0){
    rez.date = malloc(rez.numar_rezultate * sizeof(RezultatDocument));
    } else {
        rez.date = NULL;
    }
    int i=0, j=0, k=0;
    while(i<stanga.numar_rezultate && j<dreapta.numar_rezultate) {
        if(stanga.date[i].scor >= dreapta.date[j].scor) rez.date[k++] = stanga.date[i++];
        else rez.date[k++] = dreapta.date[j++];
    }
    while(i<stanga.numar_rezultate) rez.date[k++]=stanga.date[i++];
    while(j<dreapta.numar_rezultate) rez.date[k++]=dreapta.date[j++];
    if(stanga.date) free(stanga.date); 
    if(dreapta.date) free(dreapta.date);
    return rez;
}

RezultatCautare divide_et_impera(char **cai, int inceput, int sfarsit, const char *termen) {
    if (inceput == sfarsit) {
        RezultatCautare rez = {NULL, 0};
        int scor = calculeaza_relevanta(cai[inceput], termen);
        if(scor>0) {
            rez.numar_rezultate = 1;
            rez.date = malloc(sizeof(RezultatDocument));
            rez.date[0].cale = malloc(strlen(cai[inceput]) + 1);
            strcpy(rez.date[0].cale, cai[inceput]);
            rez.date[0].scor = scor;
        }
        return rez;
    }
    int mijloc=inceput + (sfarsit-inceput) / 2;
    return interclaseaza_rezultate(divide_et_impera(cai, inceput, mijloc, termen), divide_et_impera(cai, mijloc+1, sfarsit, termen));
}

void ruleaza_cautare(const char *termen) {
    FILE *f_lista = fopen("documente.txt", "r");
    if (!f_lista) return;
    char **cai = NULL; char linie[MAX_CALE]; int nr_doc = 0;
    while (fgets(linie, sizeof(linie), f_lista)) {
        elimina_newline(linie);
        if (strlen(linie) > 0) {
            cai = realloc(cai, (nr_doc+1) * sizeof(char*));
            cai[nr_doc] = malloc(strlen(linie) + 1);
            strcpy(cai[nr_doc++], linie);
        }
    }
    fclose(f_lista);
    RezultatCautare final = {NULL, 0};
    if (nr_doc > 0) final = divide_et_impera(cai, 0, nr_doc - 1, termen);
    FILE *f_out = fopen("index.out", "w");
    if (f_out) {
        fprintf(f_out, "%s\n", termen);
        for (int i = 0; i < final.numar_rezultate; i++) {
            fprintf(f_out, "%s\n", final.date[i].cale);
            free(final.date[i].cale);
        }
        fclose(f_out);
    }
    if (final.date) free(final.date);
    for (int i = 0; i < nr_doc; i++) free(cai[i]);
    free(cai);
}

void valideaza_fisier_output(const char *termen_asteptat, const char **cai_asteptate, int nr_cai_asteptate) {
    FILE *f = fopen("index.out", "r");
    assert(f!=NULL);
    char linie[MAX_CALE];
    //Verific prima linie/termenul cautat
    if(fgets(linie, sizeof(linie), f)) {
        elimina_newline(linie);
        assert(strcmp(linie, termen_asteptat) == 0);
    } else {
        assert(0);
    }

    //Verific restul liniilor (caile) intr-un for
    for(int i = 0; i < nr_cai_asteptate; i++) {
        assert(fgets(linie, sizeof(linie), f) != NULL);
        elimina_newline(linie);
        assert(strcmp(linie, cai_asteptate[i]) == 0);
    }

    //Verific sa nu existe linii extra
    assert(fgets(linie, sizeof(linie), f) == NULL);
    fclose(f);
}

int main() {
    FILE *f1 = fopen("doc_a.txt", "w"); fprintf(f1, "masina\nmasina\nroata\n"); fclose(f1);
    FILE *f2 = fopen("doc_b.txt", "w"); fprintf(f2, "masina\n"); fclose(f2);
    FILE *fl = fopen("documente.txt", "w"); fprintf(fl, "doc_a.txt\ndoc_b.txt\n"); fclose(fl);

    //Test1: Cautare "masina" (apare in ambele, doc_a e primul datorita scorului)
    const char *asteptat1[] = {"doc_a.txt", "doc_b.txt"};
    ruleaza_cautare("masina");
    valideaza_fisier_output("masina", asteptat1, 2);
    printf("Assert test 1 (masina) este valid \n");

    //Test2: Cautare "roata" (apare doar in doc_a)
    const char *asteptat2[] = {"doc_a.txt"};
    ruleaza_cautare("roata");
    valideaza_fisier_output("roata", asteptat2, 1);
    printf("Assert test 2 (roata) este valid \n");

    //Test3: Cautare "avion" (nu apare niciunde)
    ruleaza_cautare("avion");
    valideaza_fisier_output("avion", NULL, 0);
    printf("Assert test 3 (avion) este valid \n");

    /*
    remove("doc_a.txt"); 
    remove("doc_b.txt"); 
    remove("documente.txt"); 
    remove("index.out");
    */

    printf("\nToate cele 3 variante de input au fost validate prin assert conform regulamentului.\n");
    return 0;
}
