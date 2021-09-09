#ifndef PIECE_H_
#define PIECE_H_

struct Piece
{
  unsigned int id;
  int position;
  int rotation;
  unsigned int couleur[4];
  unsigned int nbBord;
  bool fixe;
};


#endif
