#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define SIZE	16
#define COLORS	16

int diamonds[ 2 * SIZE + 2 ][ SIZE + 1 ];

int size = SIZE;
int nrofbordercolors = COLORS;
int nrofcentercolors = COLORS;
int seed;

void printPieces () {
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) {
      int north = diamonds[ 2 * i     ][ j     ];
      int east  = diamonds[ 2 * i + 1 ][ j + 1 ];
      int south = diamonds[ 2 * i + 2 ][ j     ];
      int west  = diamonds[ 2 * i + 1 ][ j     ];
      printf("%i %i %i %i\n", north, east, south, west ); }
}

void generatePieces ( ) {
  for (int i = 0; i < 2 * size; i++)
    for (int j = 0; j <  size; j++)
      diamonds[ i ][ j ] = 1 + (rand() % nrofcentercolors);

  for (int i = 0; i <= size; i++) {
    diamonds[ 0        ][ i ] = 0;
    diamonds[ 2 * size ][ i ] = 0; }

  for (int i = 0; i <= size; i++) {
    diamonds[ 2*i + 1 ][  0   ] = 0;
    diamonds[ 2*i + 1 ][ size ] = 0; }

  for (int i = 1; i < size; i++ ) {
    diamonds[ 1            ][ i ] = nrofcentercolors + 1 + (rand() % nrofbordercolors);
    diamonds[ 2 * size - 1 ][ i ] = nrofcentercolors + 1 + (rand() % nrofbordercolors); }

  for (int i = 1; i < size; i++) {
    diamonds[ 2 * i ][        0 ] = nrofcentercolors + 1 + (rand() % nrofbordercolors);
    diamonds[ 2 * i ][ size - 1 ] = nrofcentercolors + 1 + (rand() % nrofbordercolors); }
}

int main (int argc , char ** argv ) {
  if (argc > 1) {
    size             = atoi (argv[1]);
    nrofbordercolors = atoi (argv[2]);
    nrofcentercolors = atoi (argv[3]);
    seed             = atoi (argv[4]);
    srand (seed); }
  else {
    printf("use three parameters 1:size, 2:nrofbordercolors, 3:nrofcentercolors, 4:seed\n");
    return 1; }

  printf("0\n");

  generatePieces();
  printPieces();

  return 0;
}
