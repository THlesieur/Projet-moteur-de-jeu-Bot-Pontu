#ifndef __PONTU_H__
#define __PONTU_H__

	#define FAUX 0
	#define VRAI 1
	
	// joueurs au trait 

	#define EGALITE 0
	#define AUCUN 0
	#define ROU 1
	#define BLE 2
	
	// Etats des ponts 
	#define ABSENT 0
	#define PRESENT 1
	
	#define NBPONTS 40
	#define NBILES 25
	#define UNKNOWN 255

	#define COLNAME(c) ((c==ROU) ? "rouges" : "bleus")

	// Pour les exports JSON ////////////////////////////////////////////
	#define STR_FKB "\"premierRoiBloque\""
	#define STR_WINNER "\"vainqueur\""
	#define STR_TURN "\"trait\""
	#define STR_BRIDGES "\"ponts\""
	#define STR_BRIDGE "\"p\""

	#define STR_FEN "\"fenrle\""
	#define STR_NUMDIAG "\"numDiag\""
	#define STR_NOTES "\"notes\""
	#define STR_J1 "\"j1\""
	#define STR_J2 "\"j2\""
	#define STR_PAWNS "\"pions\""
	#define STR_KING "\"roi\""
	
	#define STR_SCORE_B "\"scoreB\""
	#define STR_SCORE_R "\"scoreR\""
	#define STR_SCORE_BONUS_B "\"bonusB\""
	#define STR_SCORE_BONUS_R "\"bonusR\""
	
	#define STR_NUMCOUP "\"numCoup\""

// TODO pour le tournoi
	#define STR_COUPS "\"coups\""
	#define STR_ORIGINE "\"o\""
	#define STR_DESTINATION "\"d\""
	#define STR_B "\"b\""
	#define STR_R "\"r\""
	#define STR_JOUEURS "\"joueurs\""

	#define STR_NOM "\"nom\""
	#define STR_SCORE "\"score\""
	#define STR_RONDES "\"rondes\""
	#define STR_RONDE "\"ronde\""
	#define STR_PARTIES "\"parties\""
	#define STR_RESULTAT "\"resultat\""
	#define STR_STATUT "\"statut\""

	#ifdef __DEBUG__
		#define printf0(p) printf(p)
		#define printf1(p,q) printf(p,q)
		#define printf2(p,q,r) printf(p,q,r)
		#define printf3(p,q,r,s) printf(p,q,r,s)
		#define printf4(p,q,r,s,t) printf(p,q,r,s,t)
		#define whoamid(p) whoami(p)
		#define whopd(p) whop(p)
		#define whojd(p) whoj(p)
		#define whoamjd() whoamj()
	#else
		#define printf0(p)
		#define printf1(p,q)
		#define printf2(p,q,r)
		#define printf3(p,q,r,s)
		#define printf4(p,q,r,s,t)
		#define whoamid(p)
		#define whoamjd()
		#define whopd(p)
		#define whojd(p)
	#endif

	//verif appels systèmes 

	#define CHECK_IF(sts,val,msg) \
	if ((sts) == (val)) {fprintf(stderr,"erreur appel systeme\n");perror(msg); exit(-1);}

	#define CHECK_DIF(sts,val,msg) \
	if ((sts) != (val)) {fprintf(stderr,"erreur appel systeme\n");perror(msg); exit(-1);}

// NB : afficher un octet avec printf : %hhu
	typedef unsigned char octet; 
	
	// score de chaque camp 
	typedef struct {
		octet j1; octet bonus1; 
		octet j2; octet bonus2;
	} T_Score;

	// Un pont relie deux cases
	typedef struct {
		octet ile1; 
		octet ile2;
	} T_Pont; 
	
	// Un pion se situe sur une île 
	// Et peut être isole (ne pas avoir de voisins)
	// NB : un pion non isole peut ne pas être mobile pour autant... 
	typedef struct {
		octet ile;
		octet nbVoisins;
		octet isole; 
	} T_Pion; 
	 
	// Les voisins d'une ile (max 4)
	// NB : on parle des voisins accessibles, peu importe s'ils contiennent un pion ou non
	typedef struct {
		octet nb; 
		octet iles[4]; 
	} T_Voisins; 
	
	// Une position est définie pas les ponts restants
	// et les positions des pions
	// Chaque joueur dispose de 3 pions
	// NB : on considère que le roi est le troisième pion  
	typedef struct { 
		octet numCoup; // commence à 0 pour le placement du premier pion
		octet trait; // 1(ROU) ou 2(BLE) 
		octet premierRoiBloque; // 0 (AUCUN), 1(ROU) ou 2(BLE)  
		octet bitmapPonts[NBPONTS]; 
		T_Pion j1[3]; 
		T_Pion j2[3]; 
	} T_Position;

	// NB: si le joueur ne bouge pas, origine, destination sont égaux à UNKNOWN
	// NB: cette structure est utile pour jouer un coup dans jouerCoup(T_Position p, T_Coup c), dans ce cas elle comporte le déplacement et le pont à supprimer
	// elle est aussi utile dans les listes de coups. Dans ce cas le pont représente quel pont est utilisé pour passer de origine à destination  
	// TODO : verif jouerCoup fonctionne sans déplacement  
	typedef struct {
		octet origine; 
		octet destination; 
		octet pont; 
	} T_Coup;

	// 3 pions peuvent se déplacer, s'ils sont chacun complètement mobiles, ça représente 12 déplacements
	// 40 pions peuvent être supprimés au maximum
	// Il suffit de regarder la position pour avoir la liste des ponts qu'il est possible d'enlever...  
	// On la formalise différemment, sous forme de liste de ponts
	typedef struct {
		octet nbcPions;
		octet nbcPonts; 
		T_Coup cPions[NBILES]; // On stocke aussi des iles ici lors des premiers coups ! Il en faut donc 25 et pas 12 !  
		octet cPonts[NBPONTS]; // 40
	} T_ListesCoups; 

	// le fichier d'entête topologie.h ne doit être inclus que dans un seul code objet
	// Il est offert par la librairie 
	 
	extern T_Pont topologie[NBPONTS]; 
	extern T_Position positionInitiale; 
	extern T_Position positionStandard; 

	// TODO 
	octet nbVoisins(T_Position p, octet numIle); 
	T_Voisins getVoisins(T_Position p, octet numIle); 
	void afficherVoisins(T_Voisins v); 
	T_Position getPositionInitiale();
	T_Position getPositionInitialeStandard();
	void afficherPosition(T_Position pos); 
	octet estValide(T_Position p, T_Coup c);
	T_ListesCoups getCoupsLegaux(T_Position p) ; 
	void afficherListesCoups(T_ListesCoups l);
	void majPosition(T_Position *p) ; 
	T_Position jouerCoup(T_Position p, T_Coup c) ;
	octet partieFinie(T_Position p); 
	T_Score calculerScore(T_Position p);
	void afficherScore(T_Score s);
	
#endif
