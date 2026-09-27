#include <stdio.h>
#include <stdlib.h>

#include "../include/pontu.h"
#include "../include/topologie.h"
#include "../include/moteur.h"

#define Max_tab 100
#define PONT_CAMPAGNE 255

typedef struct{
    int x;
    int y;
} T_coordonnees;

#define Abs(n) ((n)<0 ? (-n) : (n))
#define pion_campagne(p) (Abs(p.x) == 2 || Abs(p.y) == 2)
#define pion_absent(p)  (Abs(p.x)==UNKNOWN && Abs(p.y)==UNKNOWN)

typedef struct{
    T_coordonnees position_bleu[3];
    T_coordonnees position_rouge[3];
    int numCoup;
} T_Position_Coordonnees;

typedef struct{
    T_Position_Coordonnees position;
    int indexCoupTheorique;
} T_Position_Coup;

typedef struct{
    T_coordonnees origine;
    T_coordonnees destination;
    int pont;
} T_Coup_Coordonnees;

T_Position_Coup ListePosTheoriques[Max_tab];
T_Coup_Coordonnees ListeCoupsTheoriques[50];

// Prototypes (Mis à jour en camelCase)
void rotationPos(T_Position_Coordonnees *, int, int);
void assignerCoupPosition(int position[], int *i);
void chargerCoupsTheoriques(int nb, int numCoup);
int positionsEgales(T_Position_Coordonnees P1, T_Position_Coordonnees P2);
void assignerCoupTheorique(int dest, int org, int pont);
int trouverPontEq(int pont, int nb_rot, int sym);

/**
 * @brief Charge rapidement une position théorique et ses coordonnées.
 */
void chargerPosCoup(T_Position_Coordonnees *pos, int xr0, int yr0, int xr1, int yr1, int xr2, int yr2, int xb0, int yb0, int xb1, int yb1, int xb2, int yb2){
    pos->position_bleu[0].x = xb0; pos->position_bleu[0].y = yb0;
    pos->position_bleu[1].x = xb1; pos->position_bleu[1].y = yb1;
    pos->position_bleu[2].x = xb2; pos->position_bleu[2].y = yb2;

    pos->position_rouge[0].x = xr0; pos->position_rouge[0].y = yr0;
    pos->position_rouge[1].x = xr1; pos->position_rouge[1].y = yr1;
    pos->position_rouge[2].x = xr2; pos->position_rouge[2].y = yr2;
}

/**
 * @brief Enregistre les détails d'un coup théorique à jouer.
 */
void chargerCoup(T_Coup_Coordonnees *coup, int dest_x, int dest_y, int org_x, int org_y, int pont){
    coup->destination.x = dest_x;
    coup->destination.y = dest_y;
    coup->origine.x = org_x;
    coup->origine.y = org_y;
    coup->pont = pont;
}

/**
 * @brief Charge les ouvertures théoriques depuis un fichier externe.
 * 
 * Remplace l'ancien switch/case par une lecture dynamique depuis "ouvertures.txt".
 * Cela sépare les données de la logique du code et évite la recompilation en cas d'ajout de coups.
 * 
 * @param nb Nombre d'adversaires en campagne
 * @param numCoup Numéro du coup actuel (pair = rouge, impair = bleu)
 */
void chargerCoupsTheoriques(int nb, int numCoup){
    FILE *fichier = fopen("ouvertures.txt", "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier ouvertures.txt\n");
        return; // Gérer l'erreur proprement
    }

    int f_numCoup, f_nb;
    int xr0, yr0, xr1, yr1, xr2, yr2, xb0, yb0, xb1, yb1, xb2, yb2;
    int dest_x, dest_y, org_x, org_y, pont;
    int posIndex = 0, coupIndex = 0;

    // Lecture du fichier ligne par ligne
    while (fscanf(fichier, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
                  &f_numCoup, &f_nb, &xr0, &yr0, &xr1, &yr1, &xr2, &yr2, 
                  &xb0, &yb0, &xb1, &yb1, &xb2, &yb2, 
                  &dest_x, &dest_y, &org_x, &org_y, &pont) == 19) {
        
        // On ne charge en mémoire que ce qui concerne notre situation actuelle
        if (f_numCoup == numCoup && f_nb == nb) {
            chargerPosCoup(&(ListePosTheoriques[posIndex].position), xr0, yr0, xr1, yr1, xr2, yr2, xb0, yb0, xb1, yb1, xb2, yb2);
            ListePosTheoriques[posIndex].indexCoupTheorique = coupIndex;
            chargerCoup(&(ListeCoupsTheoriques[coupIndex]), dest_x, dest_y, org_x, org_y, pont);
            
            posIndex++;
            coupIndex++;
        }
    }
    fclose(fichier);
}

/**
 * @brief Applique des rotations et symétries pour trouver le pont équivalent.
 * 
 * @param pont L'identifiant original du pont
 * @param nb_rot Nombre de rotations (0 à 3)
 * @param sym Symétrie (1 = oui, 0 = non)
 * @return int L'identifiant du nouveau pont après transformation
 */
int trouverPontEq(int pont, int nb_rot, int sym){
    int x = pont, y = 0, resultat;
    nb_rot = nb_rot % 4;
    
    if(pont > 8) {
        while(x > 8){
            x -= 9;
            y++;
        }
    }
    
    if(sym == 1){
        switch(x){
            case 0: x = 3; break;
            case 1: x = 2; break;
            case 2: x = 1; break;
            case 3: x = 0; break;
            case 4: x = 8; break;
            case 5: x = 7; break;
            case 6: x = 6; break;
            case 7: x = 5; break;
            case 8: x = 4; break;
        }
        pont = y * 9 + x;
        sym = 0;
    }
    
    if(nb_rot == 0) return pont;
    
    if (x >= 0 && x <= 3) {
        resultat = pont + (x + 1) * 8 - y * 10;
    } else {
        x -= 4;
        resultat = pont + (1 + x) * 8 - (y + 1) * 10 + 1;
    }
    
    if(nb_rot > 1) return trouverPontEq(resultat, nb_rot - 1, sym);
    return resultat;
}

void ecrireCoup(T_Coup coup, T_ListesCoups liste_coups){
    int i, j;
    for(i = 0; i < liste_coups.nbcPions; i++){
        if(coup.origine == liste_coups.cPions[i].origine && coup.destination == liste_coups.cPions[i].destination){
            break;
        }
    }

    for(j = 0; j < liste_coups.nbcPonts; j++){
        if(coup.pont == liste_coups.cPonts[j]){
            ecrireIndexCoup(i, j);
            return;
        }
    }

    fprintf(stderr, "Erreur : Coup non trouvé dans la liste des coups légaux\n");
}

T_coordonnees transformerIleCoordonnees(int ile){
    T_coordonnees coor = {0, 0};
    if(ile == UNKNOWN){
        coor.x = UNKNOWN;
        coor.y = UNKNOWN;
        return coor;
    }
    
    if(ile < 12) {
        while(ile < 10){
            ile += 5;
            coor.y++;
        }
    } else {
        while(ile > 14){
            ile -= 5;
            coor.y--;
        }
    }
    coor.x = ile - 12;
    return coor;
}

T_Position_Coordonnees transformerPositionCoordonnees(T_Position pos){
    T_Position_Coordonnees position;
    position.numCoup = pos.numCoup;
    for(int i = 0; i < 6; i++){
        if(i >= 3) position.position_bleu[i-3] = transformerIleCoordonnees(pos.j2[i-3].ile);
        else position.position_rouge[i] = transformerIleCoordonnees(pos.j1[i].ile);
    }
    return position;
}

int sontIdentiques(T_coordonnees c1, T_coordonnees c2) {
    if ((pion_absent(c1) && pion_absent(c2)) || (pion_campagne(c1) && pion_campagne(c2))) return 1;
    return (c1.x == c2.x && c1.y == c2.y);
}

int positionsEgales(T_Position_Coordonnees P1, T_Position_Coordonnees P2) {
    int deja_trouve_R[3] = {0, 0, 0};
    int deja_trouve_B[3] = {0, 0, 0};
    int score = 0;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(!deja_trouve_R[j] && sontIdentiques(P1.position_rouge[i], P2.position_rouge[j])) {
                deja_trouve_R[j] = 1;
                score++;
                break;
            }
        }
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(!deja_trouve_B[j] && sontIdentiques(P1.position_bleu[i], P2.position_bleu[j])) {
                deja_trouve_B[j] = 1;
                score++;
                break;
            }
        }
    }

    return (score == 6);
}

void rotationPion(T_coordonnees *ile, int rot, int sym){  
    if(sym){
        ile->x *= -1;
    }
    rot = rot % 4;
    if(rot != 0){
        int aux = ile->x;
        ile->x = ile->y;
        ile->y = -aux;
        if(rot > 1) rotationPion(ile, rot - 1, 0);
    }
}

void rotationPos(T_Position_Coordonnees *P, int sym, int nb_rotation){
    nb_rotation = nb_rotation % 4;
    for(int i = 0; i < 3; i++){
        rotationPion(&(P->position_rouge[i]), nb_rotation, sym);
        rotationPion(&(P->position_bleu[i]), nb_rotation, sym);
    }       
}

/**
 * @brief Vérifie si deux positions sont équivalentes par rotation ou symétrie.
 * 
 * Compare la position actuelle avec une position théorique de l'arbre
 * des ouvertures en testant les 4 rotations et leurs symétries.
 * 
 * @param P La position actuelle sur le plateau.
 * @param P_theorique La position de référence issue du dictionnaire.
 * @return int 0 si aucune équivalence, sinon l'index de transformation (1 à 8).
 */
int positionsEquivalentes(T_Position_Coordonnees P, T_Position_Coordonnees P_theorique){
    for(int i = 1; i <= 8; i++){
        if(positionsEgales(P, P_theorique)){
            return i;
        }
        rotationPos(&P_theorique, i == 4, 1);
    }
    return 0;
}

int nbAdversairesCampagne(T_Position_Coordonnees pos){
    int n = 0;
    T_coordonnees *joueur = (pos.numCoup % 2 == 0) ? pos.position_bleu : pos.position_rouge;

    for(int i = 0; i < 3; i++){
        if(pion_campagne(joueur[i])) n++;
    }
    return n;
}

/**
 * @brief Identifie et retire le pont approprié en zone de campagne.
 * 
 * Analyse la position des pions en bordure pour calculer mathématiquement
 * le pont qui doit être détruit (inclut la gestion des symétries).
 * 
 * @param pos La position actuelle des coordonnées.
 * @return int L'identifiant du pont à détruire, ou -1 si introuvable.
 */
int trouverPontCampagne(T_Position_Coordonnees pos){
    int ile = -1, x_eq, y_eq, pont_eq, rot = 0, sym = 0;
    T_coordonnees *joueur;
    
    if(pos.numCoup % 2 == 0){
        chargerPosCoup(&(ListePosTheoriques[0].position), -1,0,0,0,0,-1,-1,1,0,1,1,0);
        joueur = pos.position_bleu;
    } else {
        joueur = pos.position_rouge;
    }
    
    for(int i = 0; i < 3; i++){
        if(pion_campagne(joueur[i])){
            if(Abs(joueur[i].x) == 2 && Abs(joueur[i].y) == 2){
                ile = 12 + 5 * joueur[i].y + joueur[i].x;
            } else {
                x_eq = Abs(joueur[i].x);
                y_eq = Abs(joueur[i].y);
                
                if(Abs(y_eq) == 2) pont_eq = 6 + x_eq;
                else pont_eq = 21 - 9 * y_eq;
                
                if(joueur[i].x == -x_eq) sym = !sym;
                if(joueur[i].y == -y_eq){
                    sym = !sym;
                    rot += 2;
                }
                return trouverPontEq(pont_eq, rot, sym);
            }
        }
    }
    
    if(ile != -1){
        switch (ile) {
            case 0: return 0;
            case 4: return 3;
            case 20: return 36;
            case 24: return 39;
        }
    }
    return -1;
}

int parCoeur(T_Position pos, T_ListesCoups listeCoups){
    int pos_ennemi, resultat, nb, rot, sym;
    T_Position_Coordonnees pos_coor;
    T_Coup coup;
    T_Coup_Coordonnees coup_coor;
    
    switch(pos.numCoup){
        case 0:
            // Premier placement des rouges
            coup.destination = 12; // Les valeurs d'îles (12, 11, etc.) sont conservées d'après les règles de Pontu
            coup.origine = 12;
            coup.pont = UNKNOWN;
            ecrireCoup(coup, listeCoups);
            return 1;
            
        case 2:
            // Deuxieme placement des rouges
            pos_ennemi = pos.j2[0].ile;
            switch(pos_ennemi) {
                case 0: case 1: case 2: case 5: case 6: case 7:
                    coup.destination = 11; break;
                case 3: case 4: case 8: case 9: case 13: case 14:
                    coup.destination = 7; break;
                case 17: case 18: case 19: case 22: case 23: case 24:
                    coup.destination = 13; break;
                case 10: case 11: case 15: case 16: case 20: case 21:
                    coup.destination = 17; break;
            }
            coup.origine = coup.destination;
            coup.pont = UNKNOWN;
            ecrireCoup(coup, listeCoups);
            return 1;
            
        case 4:
            // Troisième placement des rouges
            switch(pos.j1[1].ile){
                case 11: coup.destination = (pos.j2[1].ile != 13) ? 13 : 17; break;
                case 7:  coup.destination = (pos.j2[1].ile != 17) ? 17 : 11; break;
                case 13: coup.destination = (pos.j2[1].ile != 11) ? 11 : 7; break;
                case 17: coup.destination = (pos.j2[1].ile != 7)  ? 7  : 13; break;
            }
            coup.origine = coup.destination;
            coup.pont = UNKNOWN;
            ecrireCoup(coup, listeCoups);
            return 1;
            
        case 6:
            // Premier coup des rouges
            pos_coor = transformerPositionCoordonnees(pos);
            nb = nbAdversairesCampagne(pos_coor);
            
            if(nb != 3){
                chargerCoupsTheoriques(nb, pos.numCoup);
                for(int i = 0; i < 20; i++){
                    resultat = positionsEquivalentes(pos_coor, ListePosTheoriques[i].position);
                    if(resultat){
                        coup_coor = ListeCoupsTheoriques[ListePosTheoriques[i].indexCoupTheorique];
                        
                        sym = resultat > 4;
                        rot = (resultat - 1) % 4;
                        if(rot != 0){
                            rotationPion(&(coup_coor.destination), rot, sym);
                            rotationPion(&(coup_coor.origine), rot, sym);                            
                        } 
                        
                        if(coup_coor.pont != PONT_CAMPAGNE){
                            if(rot != 0) trouverPontEq(coup_coor.pont, rot, sym);
                        } else {
                            coup_coor.pont = trouverPontCampagne(pos_coor);
                        }
                        
                        if(coup_coor.pont == -1) return 0; // Sécurité : pont introuvable

                        coup.destination = 12 - coup_coor.destination.y * 5 + coup_coor.destination.x;
                        coup.origine = 12 - coup_coor.origine.y * 5 + coup_coor.origine.x;
                        coup.pont = coup_coor.pont;

                        ecrireCoup(coup, listeCoups);
                        return 1;
                    }
                }
                return 0;
            } else {
                coup.origine = (pos.j1[2].ile == 13 || pos.j1[2].ile == 11) ? 13 : 17;
                coup.destination = 18;
                coup.pont = trouverPontCampagne(pos_coor);
                ecrireCoup(coup, listeCoups);
                return 1;
            }
            
        case 1:
            // Premier placement des bleus
            switch(pos.j1[0].ile){
                case 7: coup.destination = 17; break;
                case 13: coup.destination = 11; break;
                case 17: coup.destination = 7; break;
                case 11: coup.destination = 13; break;
                case 12: coup.destination = 7; break;
                default: coup.destination = 12; break;
            }
            coup.origine = coup.destination;
            coup.pont = UNKNOWN;
            ecrireCoup(coup, listeCoups);
            return 1;
            
        case 3:
            // Deuxieme placement des bleus
            pos_coor = transformerPositionCoordonnees(pos);
            nb = nbAdversairesCampagne(pos_coor);
            if(nb != 2){
                chargerCoupsTheoriques(nb, pos.numCoup);
                for(int i = 0; i < 20; i++){
                    resultat = positionsEquivalentes(pos_coor, ListePosTheoriques[i].position);
                    if(resultat){
                        coup_coor = ListeCoupsTheoriques[ListePosTheoriques[i].indexCoupTheorique];
                        sym = resultat > 4;
                        rot = (resultat - 1) % 4;
                        if(rot != 0){
                            rotationPion(&(coup_coor.destination), rot, sym);
                            rotationPion(&(coup_coor.origine), rot, sym);                            
                        } 
                        coup.destination = 12 - coup_coor.destination.y * 5 + coup_coor.destination.x;
                        coup.origine = 12 - coup_coor.origine.y * 5 + coup_coor.origine.x;
                        coup.pont = UNKNOWN;
                        ecrireCoup(coup, listeCoups);
                        return 1;
                    }
                }
                return 0;
            } else {
                coup.origine = 11;
                coup.destination = 11;
                coup.pont = UNKNOWN;
                ecrireCoup(coup, listeCoups);
                return 1;
            }
            
        case 5:
            pos_coor = transformerPositionCoordonnees(pos);
            nb = nbAdversairesCampagne(pos_coor);
            if(nb <= 1){
                chargerCoupsTheoriques(nb, pos.numCoup);
                for(int i = 0; i < 40; i++){
                    resultat = positionsEquivalentes(pos_coor, ListePosTheoriques[i].position);
                    if(resultat){
                        coup_coor = ListeCoupsTheoriques[ListePosTheoriques[i].indexCoupTheorique];
                        sym = resultat > 4;
                        rot = (resultat - 1) % 4;
                        if(rot != 0){
                            rotationPion(&(coup_coor.destination), rot, sym);
                            rotationPion(&(coup_coor.origine), rot, sym);                            
                        } 
                        coup.destination = 12 - coup_coor.destination.y * 5 + coup_coor.destination.x;
                        coup.origine = 12 - coup_coor.origine.y * 5 + coup_coor.origine.x;
                        coup.pont = UNKNOWN;
                        ecrireCoup(coup, listeCoups);
                        return 1;
                    }
                }
                return 0;
            } else if(nb == 3){
                coup.origine = 11;
                coup.destination = 11;
                coup.pont = UNKNOWN;
                ecrireCoup(coup, listeCoups);
                return 1;
            }
            return 0;
    }
    return 0;
}

/**
 * @brief Sélectionne un coup pour l'IA en se basant sur le dictionnaire d'ouvertures.
 * 
 * Si la position n'est pas connue "par coeur", la fonction doit basculer
 * sur l'algorithme générique de calcul de l'arbre (MinMax / AlphaBeta).
 */
void choisirCoup(T_Position pos, T_ListesCoups coups){
    if(!parCoeur(pos, coups)){
        fprintf(stderr, "Avertissement : Sortie du dictionnaire d'ouvertures. Déclenchement de l'arbre calculatoire.\n");
        
        // TODO: Appeler ici ta fonction MinMax / Alpha-Beta pour calculer dynamiquement le coup
        // ex: calculerMeilleurCoupDynamique(pos, coups);
    }
}
