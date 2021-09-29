## Introduction

Ce projet permet de donner des méthodes de résolution du puzzle EternityII composé de 256 pièces.
Puzzle étudié pour être extrêmement difficile, au point que son éditeur offre 2 millions de dollars
au premier qui parviendra à placer correctement ses 256 pièces carrées de façon à ce que les côtése chacune
correspondent à ceux de ses 4 voisins.


## Prérequis
Afin de pouvoir exécuter le code sur votre poste, vous devez télécharger :
  * La bibliothèque ParadisEO (paradiseo.gforge.inria.fr/)
  * L'outil processing pour l'affichage du puzzle (https://processing.org/)
  * MGwin (https://www.mingw-w64.org/, Windows uniquement)
  * Cmake (https://cmake.org/)
  * Doxygen (https://www.doxygen.nl/index.html)

### Installation
#### ParadisEO
  1. Télécharger la version selon l'environnement utilisé : http://paradiseo.gforge.inria.fr/index.php?n=Download.Download
  2. Suivre les étapes d'installation en laissant les options par défaut.

### Pour Windows
  1. Ne pas oublier de télécharger les outils (tools package)
  2. Ne pas oublier de définir les variables d'environnement :
     1. Propriétés système
     2. Cliquer sur varaible d'ennvironnement
     3. Dans l'onglet variablie système, trouver la variable "path" et cliquer sur "modifier"
     4. Ajouter une nouvelle ligne (cliquer sur "nouveau") avec le chemin de des dossiers : ParadisEO, Doxygen\bin,
         MinGW\bin et Cmake\bin



## Exécution
Suivre [cette procédure] :

1. Ouvrir une invite de commande
2. Récupérer le projet : ``git clone https://github.com/maximilienmoraud/eternity2AG``
3. Se placer à l'intérieur de ce dossier : ``cd eternity2AG/``

4. Faire le build (générer le code en lenguage processeur) :
   1. Créer un dossier pour générer le build : ``mkdir 'yourbuildingdirectory'``
   2. Se placer dans ce dossier : ``cd 'yourbuildingdirectory'``
   3. Générer le build : ``cmake .. && make``

5. Excuter le code (Run) :
   1. Aller dans le dosser application : `cd application`
   2. Lancer le run (variable détaillées par la suite): ```./hopla [variables]```

6. Visualisation :
   1. Lancer avant l'excution le fichier processing pour visualiser l'évolution du puzzle
               Il se trouve dans le dossier `Processing\sketch_190409a : sketch_190409a.pde`
   2. La console affiche l'évolution du traitement ainsi que le score final maximal obtenu :
                  Le but est d'atteindre un score de -480
   3. Des fichiers de suivis sont également créés dans le dossier `essai` ils sont nommés en fonction des paramètres définis ci-après. 
      1. `final.txt` correspond aux fitness dela dernière population générée
      2. `info.txt` correspond au suivi des différentes opérations de l'algorithme demandé
      3. `log.log` correspond aux valeurs de toutes les meilleurs fitness à chaque étape de chaque AG
      4. `map.txt` correspond au fichier lisible par processing du meilleur élément de la dernière population

## Les paramètres
Pour la commande `./hopla [variables]`, il est possible de renseigner les paramètres suivants :
--> exemple : ```./hopla -D=1 -m=0.5```
````
    -R : Numéro du run, valeur par défaut : 1
    -D : Debug consol, valeur par défaut : false
    -g : Taille de la génération, valeur par défaut : 10
    -L : Active la local search 1, valeur par défaut : false
            Permet d'échanger des pièces de places si cela améliore le score
    -2 : Active la local search 2, valeur par défau : false
            Permet de tourner des pièces si cela améliore le score
    -l : Nombre de local search effectuées, valeur par défaut : 1
    -M : Nobmre maximum d'algorithmes génétiques appliqués, valeur par défaut : 100
    -O : Nombre de l'objectif de gain fitness par Algo génétique, valeur par défaut : 5
    -I : Statégie d'initialisation, valeur par défaut : 3
            valeur 0 : initialisation aléatoire
            valeur 1 : initialisation glouton (pièces cohérentes)
            valeur 2 : initialisation glouton avec un contour sans erreur
            valeur 3 : initalisation en escargot (pièces cohérentes) avec contour sans erreur
    -k : Nombre de lignes affectées par la mutation, valeur par défaut : 3
    -m : taux de cross effectués dans l'algorithme génétique, valeur par défaut : 0,5
    -s : taux de swap effectués dans l'algorithme génétique, valeur par défaut : 0,5
````

Pour Processing, changer les premières lignes du code afin de modifier l'affichage :
```` 
    -t : pour modifier la largeur d'une pièce
     -solution : mettre 1 pour l'affichage en couleur des pièces et 2 pour l'affichage en motif
     -afficher_vraifaux : mettre 1 pour afficher les bulles rouge et verte et 0 pour ne pas les afficher
     -contour : mettre 1 pour afficher le contours des pièces et metre 0 sinon
     -contour_triangle : mettre 1 pour afficher le contours des triangles et metre 0 sinon
````


