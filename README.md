# Projet moteur de jeu : Bot Pontu
La partie du projet qui concerne ce que j'ai développé pour le bot. C'est donc toute la partie théorique du jeu pontu, que nous avons nous-même élaboré, où au début du jeu on peut directement donner un coup à jouer en fonction de la position.

---

## Le Jeu Pontu (Règles & Plateau)

**Le Pontu** est un jeu de plateau abstrait d'affrontement opposant deux équipes : **Les Rouges** (qui commencent) et **Les Bleus**.

### Composition du plateau
* **25 îles** disposées en un carré de 5x5, numérotées de `0` à `24` (en partant du coin supérieur gauche, de gauche à droite et de haut en bas).
* **40 ponts** reliant les îles adjacentes, numérotés de `0` à `39`.

<img width="621" height="609" alt="Capture d’écran du 2026-03-19 09-57-58" src="https://github.com/user-attachments/assets/b55b4344-ecd4-47dc-ba50-fc8a711efe3e" />

---

### Déroulement d'une partie (Variante Royale)
Chaque joueur possède **2 pions** et **1 Roi** (le Roi apporte une valeur supérieure pour le décompte des points).

1. **Phase de placement (Coups 0 à 5) :** Tour à tour, chaque équipe place ses 3 pièces sur des îles libres (les pions d'abord, le Roi en dernier). Cette phase est cruciale pour contrôler le centre et verrouiller le territoire adverse.
2. **Phase de jeu (À partir du coup 6) :** Chaque coup se déroule en deux temps :
   * Déplacer un de ses pions/roi vers une île voisine libre (reliée par un pont).
   * Retirer définitivement un pont du plateau.
3. **Condition de victoire :** Le but est d'isoler les pièces adverses. Dans la variante royale, isoler le Roi adverse en premier rapporte un point bonus décisif pour éviter les matchs nuls.

---

## Algorithme & Approche Théorique (`joueurs/parcoeur.c`)

Le fichier `parcoeur.c` gère la phase de placement et le premier déplacement/retrait de pont en combinant une base de données d'ouvertures et une réduction géométrique des possibilités.

### Fonctionnement du bot

| Étape | Action de l'algorithme |
| :--- | :--- |
| **1. Identification** | Évalue le numéro du coup courant pour déterminer l'équipe (Rouge/Bleu) et l'état du plateau. |
| **2. Chargement** | Charge dynamiquement depuis `joueurs/ouvertures.txt` uniquement les positions théoriques pertinentes pour ce coup. |
| **3. Analyse Géométrique** | Le plateau étant carré, le programme teste jusqu'à **8 transformations géométriques** (4 rotations et leurs symétries) pour faire correspondre le plateau actuel à une position théorique connue. |
| **4. Exécution** | Une fois l'équivalence trouvée, le coup théorique calculé subit la transformation inverse (rotation/symétrie) pour jouer la réponse parfaite. |

---

## Structure des Fichiers

* `joueurs/parcoeur.c` : Moteur de recherche et de réduction par symétrie/rotation.
* `joueurs/ouvertures.txt` : Base de données contenant les positions théoriques et leurs réponses associées.
