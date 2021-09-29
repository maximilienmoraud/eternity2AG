/* TODO
 * 1) Définir la taille t (largueur d'un pièce de puzzle) en fonction de la taille de la fenetre (et centrer le tout si ça ne tombe pas rond)
 * 2) Mettre les couleurs de fonds ressemblant aux couleurs d'origine ou utiliser des images qui qui ressemble aux pièces d'éternity 
 * 3) Utiliser l'un ou l'autre des solutions
 */

BufferedReader reader;                //Permet de lire un fichier ligne par ligne sous forme de string  
String line;                          //Pour contenir la ligne lue dans le fichier 
int[] colors = new int[23];           //Tableau des couleurs utilisées pour représenter les picès du jeu
PImage[] boite = new PImage[23];      //Tableau des pièces utilisées pour réprésenter le puzzle

String inputFile = "lastpuzzlebuild.txt";   //Stocker le nom du fichier dans inputFile
boolean afficherCorrect = true;
boolean afficherErreur = true;


int t = 40;                            //Taille de largueur d'un triangle, afin d'ajuster à la taille de l'écran
int solution = 2;                      //Mettre 1 pour l'affichage en couleur et 2 pour l'affichage en motif 
int afficher_vraifaux = 0;              //Mettre 1 pour afficher les bulles rouge et verte et 0 pour ne pas les afficher 
int contour = 1;                       //Mettre 1 pour afficher le contours des pièces et metre 0 sinon (avec la solution 1 ne change rien)
int contour_triangle = 1;               //Mettre 1 pour afficher le contours des triangles et metre 0 sinon (avec la solution ne change rien)


void setup() {
 
  size(650, 650);                      //Définition de la taille de l'affichage (de la fenêtre)
    
  colors[0]=#B8B8B8;                   // Définition des couleurs 
  colors[1]=#FF5757;
  colors[2]=#FF7ACC;
  colors[3]=#E7A1FA;
  colors[4]=#9560FF;
  colors[5]=#89FAFF;
  colors[6]=#6BC0C4;
  colors[7]=#1EADFF;
  colors[8]=#68A8FF;
  colors[9]=#A9E88E;
  colors[10]=#FFDC4D;
  colors[11]=#FF9452;
  colors[12]=#FF2424;
  colors[13]=#FF810C;
  colors[14]=#B1FF31;
  colors[15]=#00B81E;
  colors[16]=#0019FF;
  colors[17]=#9403F8;
  colors[18]=#FF3974;
  colors[19]=#C67E50;
  colors[20]=#8FAC6A;
  colors[21]=#BB00BB;
  colors[22]=#0369BF; 
  
  boite[0]=loadImage("1.png"); boite[0].resize(t,t);       //initialiser le tableau des images pour représenter les pièces
  boite[1]=loadImage("2.png"); boite[1].resize(t,t);
  boite[2]=loadImage("3.png"); boite[2].resize(t,t);
  boite[3]=loadImage("4.png"); boite[3].resize(t,t);
  boite[4]=loadImage("5.png"); boite[4].resize(t,t);
  boite[5]=loadImage("6.png"); boite[5].resize(t,t);
  boite[6]=loadImage("7.png"); boite[6].resize(t,t);
  boite[7]=loadImage("8.png"); boite[7].resize(t,t);
  boite[8]=loadImage("9.png"); boite[8].resize(t,t);
  boite[9]=loadImage("10.png"); boite[9].resize(t,t);
  boite[10]=loadImage("11.png"); boite[10].resize(t,t);
  boite[11]=loadImage("12.png"); boite[11].resize(t,t);
  boite[12]=loadImage("13.png"); boite[12].resize(t,t);
  boite[13]=loadImage("14.png"); boite[13].resize(t,t);
  boite[14]=loadImage("15.png"); boite[14].resize(t,t);
  boite[15]=loadImage("16.png"); boite[15].resize(t,t);
  boite[16]=loadImage("17.png"); boite[16].resize(t,t);
  boite[17]=loadImage("18.png"); boite[17].resize(t,t);
  boite[18]=loadImage("19.png"); boite[18].resize(t,t);
  boite[19]=loadImage("20.png"); boite[19].resize(t,t);
  boite[20]=loadImage("21.png"); boite[20].resize(t,t);
  boite[21]=loadImage("22.png"); boite[21].resize(t,t);
  boite[22]=loadImage("23.png"); boite[22].resize(t,t);
   
  background(255);                    // Créer un fond blanc ( le 255 teinte du blanc pour le fond sur procesig)

}



void draw() {
  //clear();                         --> utilisable uniquement sur des PGraphics objet
  
  int c1=0, c2=0, c3=0, c4=0;        //4 code couleurs de la pièce
  int l=0, h=0;                      //largeur, hauteur
  Piece[] tab = new Piece[0];        //tableau contenant toutes les pièces
  String[] decoupe;                  //Utile pour récuperer les différents éléments de chaque ligne du fichier d'entrée
  int nbColor;                       //Nombre de couleurs
  
  
  reader = createReader(inputFile);   //Ouverture du fichier 
  line="";                            //Première initialisation de la variable line non null
  int cpt=0;                          //Initialisation du compteur à 0
  
  while(line != null){   
    try {  
      line = reader.readLine();       //lecture ligne par ligne tu fichier positions.txt. MAJ de line avec la valeur de la ligne courante
    } catch (IOException e) {
      e.printStackTrace();
      line = null;
    }
    
    if (line != null) {                //Lecture terminée si il y a une erreur ou que la ligne / le fichier est vide 
                                        //noLoop();
      
      if(cpt==0){                                 //Première étape si cpt = 0, on récupère le format du puzzle
        decoupe = split(line, "\t");              //Permet de découpé les valeurs contenues dans line, dans le tableau decoupe
        l = int(decoupe[0]);
        println(l);
        h = int(decoupe[1]);
        println(h);
        tab = new Piece[l*h];                    //Le puzzle contient l*h pièces
        cpt++;
      } else if(cpt==1){                           //Deuxième étape : on recupere le nombre de couleurs du puzzle
        decoupe = split(line, "\t");
        nbColor = int(decoupe[0]);
        cpt++;
      } else {                                     //X étapes suivantes : on recupere les codes couleurs/motifs de chaque piece
        decoupe = split(line, "\t");
        c1 = int(decoupe[0]);
        c2 = int(decoupe[1]);
        c3 = int(decoupe[2]);
        c4 = int(decoupe[3]);
        tab[cpt-2]=new Piece(c1, c2, c3, c4);   //Création d'une nouvelle pièce avec les 4 couleurs récupérées
        cpt++;      
      }}} //fin du while
  
  //Affichage des pièces du puzlle (colonne par colonne) selon la solution souhaitée
   for(int i=0; i<l;i++){                        //Parcours des lignes
       for (int j=0; j<h; j++){                      //Parcous des colonnes
          if(solution==1){
            solution_couleur(i,j,tab,l);
          }
          if (solution == 2){
            solution_image(i,j,tab,l);         
       }}}
  //Affichage des bulles vertes ou rouges pour correpsondance vraie ou fausse    
  for(int i=0; i<l;i++){                        //Parcours des lignes
       for (int j=0; j<h; j++){                 //Parcous des colonnes
              if (afficher_vraifaux == 1){
                correspondances(i,j,tab, l,h);
              }}}
  //Affichage des contours des pièces en carré  
  if(contour==1){
    contour_pieces(l,h);
  }
  //Affichage des contours des triangles
   if(contour_triangle==1){
    contour_triangle(l,h);
  }
 
  
    delay(2000);
  } //fin du draw 
  
  
  
  
 //-----------------------Classe------------------------------------------------------- 

//Classe Piece contenant pour l'instant uniquement les 4 couleurs
  class Piece{
    public int c1, c2, c3, c4;
    Piece(int _c1, int _c2, int _c3, int _c4){
      c1=_c1; //haut
      c2=_c2; //gauche
      c3=_c3; //bas
      c4=_c4; //droite
    }
  }
 //------------------------------------------------------------------------------------  
 //------------------------------------------------------------------------------------ 
// fonction de la solution en couleur
  void solution_couleur(int i, int j,Piece[] tab,int l){
          
      //Parcours des colones
      //haut - rotation 180
      fill(colors[tab[l*j+i].c1]);                   //Prendre en compte la couleur correspondante
      triangle(i*t,j*t,i*t+t,j*t,i*t+t/2,j*t+t/2);   //Triangle tracé selon x1 y1 x2 y2 x3 y3
      
          
      //gauche - rotation  90
      fill(colors[tab[l*j+i].c2]);
      triangle(i*t,j*t+t,i*t,j*t,i*t+t/2,j*t+t/2); 

      //bas  - rotation  0
      fill(colors[tab[l*j+i].c3]);
      triangle(i*t+t,j*t+t,i*t,j*t+t,i*t+t/2,j*t+t/2);
      

      //droite - rotation  270
      fill(colors[tab[l*j+i].c4]);
      triangle(i*t+t,j*t,i*t+t,j*t+t,i*t+t/2,j*t+t/2);     
    
  }
 //------------------------------------------------------------------------------------  
// fonction de la solution en image
  void solution_image(int i,int j,Piece[] tab, int l){
    
    int x,y;                           //Poisitions X et Y de la pièce à placer
    PImage piece;                      //Image de la pièce à placer
      x=i*t;                           //Initialisation des positions x et y coin supérieur droit du carré de la pièce de puzzle
      y=j*t; 
      //haut - rotation 180     
      piece=boite[tab[l*j+i].c1];
      rotation(piece, x, y, 180);
          
      //gauche - rotation  90
      piece=boite[tab[l*j+i].c2];
      rotation(piece, x, y, 90);
          
      //bas  - rotation  0
      piece=boite[tab[l*j+i].c3];
      rotation(piece, x, y, 0);
           
      //droite - rotation  270
      piece=boite[tab[l*j+i].c4];
      rotation(piece, x, y, 270);
        
  }
//------------------------------------------------------------------------------------  
 //Affichage des bonnes et mauvaises correspondances
 void correspondances(int i,int j,Piece[] tab, int l,int h){
      if(i<(l-1)){
          if(tab[(j*l)+i].c4 == tab[(j*l)+i+1].c2){       //correspondance gauche/droite
            if (afficherCorrect){                         //affichage rond vert
              fill(0,255,0);
              circle(i*t+t,j*t+t/2,t/10);
            }
          }
          else{
            if (afficherErreur){                          //affichage rond rouge
              fill(255,0,0);
              circle(i*t+t,j*t+t/2,t/10);
            }
          }    
      }
      if(j<(h-1)){
        if(tab[j*l+i].c3 == tab[(j+1)*l+i].c1){           //correspondance haut/bas
            if (afficherCorrect){                         //affichage rond vert
              fill(0,255,0);
              circle(i*t+t/2,j*t+t,t/10);
            }
          }
          else{
            if (afficherErreur){                          //affichage rond rouge
              fill(255,0,0);
              circle(i*t+t/2,j*t+t,t/10);
            }
          }             
        }         
  } 
//------------------------------------------------------------------------------------  
//fonction permettant d'afficher la pièce selon la retoation souhaitée 
  void rotation(PImage piece, int x, int y, int sens){
    if (sens==0){
      image(piece,x,y);                     //placer la pièce aux coordonnées initiales
    } else {
      pushMatrix();
      if (sens == 90){
        translate (x+t,y);                  //rotation à 90° : translation du plan à x+t,y
      }
      if (sens == 180){
        translate (x+t,y+t);               //rotation à 180° : translation du plan à x+t,y+t
      }
      if (sens == 270){
        translate (x,y+t);                //rotation à 270° : translation du plan à x,y+t
      }
      rotate(radians(sens));              //Tourner le plan selon le sens souhaité
      image(piece, 0, 0);                 //Replacer la figure sur ce nouveau plan
      popMatrix();                       //Retour au plan initial après avoir placé la pièce
    }
  }
//------------------------------------------------------------------------------------ 
//Fonction permettant de faire apparaitre le contour des pièces 
void contour_pieces(int l,int h){
    for(int i=0; i<l+1;i++){                        //Parcours des lignes
    line(i*t,0, i*t, h*t);          
    }
    for (int j=0 ; j<h+1;j++){                      //Parcours de colonnes 
      line (0,j*t,l*t,j*t);
    }
  }
//------------------------------------------------------------------------------------ 
//Fonction permettant de faire apparaitre le contour des triangles
void contour_triangle(int l,int h){
    for(int i=0; i<l;i++){                          //Parcours des 
      for (int j=0 ; j<h;j++){                      //Parcours de colonnes 
        line (i*t,j*t,i*t+t,j*t+t);
        line(i*t,j*t+t,i*t+t,j*t); 
      }}}
//------------------------------------------------------------------------------------ 
  
    
  
  
  //Test d'ajout d'une forme 
  //Test d'une forme libre  
  void forme1(int a, int b, int t){
    fill(100,200,0);
    beginShape();
    
    vertex(a, a+15);
    vertex(a+17, a+32); 
    vertex(a, a+49);
    vertex(a, a+59);
    vertex(a+27, a+32);
    vertex(a, a+5);
    
   /* vertex(a, a+5);
    vertex(a+27, a+32);
    vertex(a, a+59);
    vertex(a, a+49);
    vertex(a+17, a+32);
    vertex(a, a+15);*/
    endShape(CLOSE);
  }
 
  
