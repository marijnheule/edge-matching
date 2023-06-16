#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

//#define BLOCKED
#define MAPPING
#define ATMOSTONE			// turn on for explicit one-on-one mapping
//#define FBCOLORS			// turn on for forbidden color clauses
#define LIMITED
#define HINTS
//#define PATTERN
//#define NOTWO
//#define NOTHREE
//#define NOFOUR			// Don't use three patterns and force?
//#define POSONLY

#define CENTER_PIECE	0
#define BORDER_PIECE	1
#define CORNER_PIECE	2

#define ROWS		16
#define COLUMNS		16

#define NROFTILES	(1 << TILEBITS)

unsigned int color_map[ 30 ];

unsigned int rows    = ROWS;
unsigned int columns = COLUMNS;

unsigned int nrofcolors;
unsigned int nrofbordercolors, nrofcentercolors;

unsigned int nrofvars, nrofdummies, nrofclauses;
unsigned int vars_offset;

int piece    [ 257 ][ 4 ];

int nrofpieces = ROWS * COLUMNS;
int nrofdiamonds;

int var_map[ 257 ];
int dia_map[ 512 ];

int corner_diamond[   4 ][ 2 ];
int border_diamond[  64 ][ 3 ];
int center_diamond[ 257 ][ 4 ];

int nrofhints;
int hint[ 30 ][ 4 ];

int getPieceType (int index) {
  int count = 0;

  for (int rotation = 0; rotation < 4; rotation++)
    if (piece[ index ][ rotation ] == 0)
      count++;

  return count;
}

int getDiamondType (int index) {
  if (index < columns)                    return 1;
  if (index > (nrofdiamonds - columns)  ) return 1;
  if ((index % (2*columns-1)) == columns) return 1;
  if ((index % (2*columns-1)) == 0      ) return 1;

  return 0;
}

void blockFourPattern( ) {
	int i, j, k, l, r, p, size;

	size = (rows-2) * (columns-2);

	for( i = 1; i <= nrofcentercolors; i++ )
	    for( j = 1; j <= nrofcentercolors; j++ )
	    {
		for( p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != CENTER_PIECE ) continue;
		   if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 1 ] == j) ) goto center_A;
		   if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) ) goto center_A;
		   if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 3 ] == j) ) goto center_A;
		   if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 0 ] == j) ) goto center_A;
		} continue; center_A:;

	        for( k = 1; k <= nrofcentercolors; k++ )
		{
		    for( p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != CENTER_PIECE ) continue;
		        if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 2 ] == k) ) goto center_B;
		        if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 3 ] == k) ) goto center_B;
		        if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 0 ] == k) ) goto center_B;
		        if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 1 ] == k) ) goto center_B;
		    }   continue; center_B:;

		    for( p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != CENTER_PIECE ) continue;
		        if( (piece[ p ][ 0 ] == j) && (piece[ p ][ 1 ] == k) ) goto center_C;
		        if( (piece[ p ][ 1 ] == j) && (piece[ p ][ 2 ] == k) ) goto center_C;
		        if( (piece[ p ][ 2 ] == j) && (piece[ p ][ 3 ] == k) ) goto center_C;
		        if( (piece[ p ][ 3 ] == j) && (piece[ p ][ 0 ] == k) ) goto center_C;
		    }   continue; center_C:;

		    for( p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != CENTER_PIECE ) continue;
		        if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 1 ] == j) && (piece[ p ][ 2 ] == k) ) goto center_D;
		        if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) && (piece[ p ][ 3 ] == k) ) goto center_D;
		        if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 3 ] == j) && (piece[ p ][ 0 ] == k) ) goto center_D;
		        if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 0 ] == j) && (piece[ p ][ 1 ] == k) ) goto center_D;
		    }   continue; center_D:;

	            for( l = 1; l <= nrofcentercolors; l++ )
		    {
		        for( p = 1; p <= nrofpieces; p++ )
		        {
		            if( getPieceType( p ) != CENTER_PIECE ) continue;
		            if( (piece[ p ][ 0 ] == k) && (piece[ p ][ 1 ] == l) ) goto center_E;
		            if( (piece[ p ][ 1 ] == k) && (piece[ p ][ 2 ] == l) ) goto center_E;
		            if( (piece[ p ][ 2 ] == k) && (piece[ p ][ 3 ] == l) ) goto center_E;
		            if( (piece[ p ][ 3 ] == k) && (piece[ p ][ 0 ] == l) ) goto center_E;
		        }   continue; center_E:;

		        for( p = 1; p <= nrofpieces; p++ )
		        {
		            if( getPieceType( p ) != CENTER_PIECE ) continue;
		            if( (piece[ p ][ 0 ] == l) && (piece[ p ][ 1 ] == i) ) goto center_F;
		            if( (piece[ p ][ 1 ] == l) && (piece[ p ][ 2 ] == i) ) goto center_F;
		            if( (piece[ p ][ 2 ] == l) && (piece[ p ][ 3 ] == i) ) goto center_F;
		            if( (piece[ p ][ 3 ] == l) && (piece[ p ][ 0 ] == i) ) goto center_F;
		        }   continue; center_F:;

		        for( p = 1; p <= nrofpieces; p++ )
		        {
		            if( getPieceType( p ) != CENTER_PIECE ) continue;
		            if( (piece[ p ][ 0 ] == j) && (piece[ p ][ 2 ] == l) ) goto center_K;
		            if( (piece[ p ][ 1 ] == j) && (piece[ p ][ 3 ] == l) ) goto center_K;
		            if( (piece[ p ][ 2 ] == j) && (piece[ p ][ 0 ] == l) ) goto center_K;
		            if( (piece[ p ][ 3 ] == j) && (piece[ p ][ 1 ] == l) ) goto center_K;
		        }   continue; center_K:;

		        for( p = 1; p <= nrofpieces; p++ )
		        {
		           if( getPieceType( p ) != CENTER_PIECE ) continue;
		           if( (piece[ p ][ 0 ] == j) && (piece[ p ][ 1 ] == k) && (piece[ p ][ 2 ] == l) ) goto center_G;
		           if( (piece[ p ][ 1 ] == j) && (piece[ p ][ 2 ] == k) && (piece[ p ][ 3 ] == l) ) goto center_G;
		           if( (piece[ p ][ 2 ] == j) && (piece[ p ][ 3 ] == k) && (piece[ p ][ 0 ] == l) ) goto center_G;
		           if( (piece[ p ][ 3 ] == j) && (piece[ p ][ 0 ] == k) && (piece[ p ][ 1 ] == l) ) goto center_G;
		        }  continue; center_G:;

		        for( p = 1; p <= nrofpieces; p++ )
		        {
		           if( getPieceType( p ) != CENTER_PIECE ) continue;
		           if( (piece[ p ][ 0 ] == k) && (piece[ p ][ 1 ] == l) && (piece[ p ][ 2 ] == i) ) goto center_H;
		           if( (piece[ p ][ 1 ] == k) && (piece[ p ][ 2 ] == l) && (piece[ p ][ 3 ] == i) ) goto center_H;
		           if( (piece[ p ][ 2 ] == k) && (piece[ p ][ 3 ] == l) && (piece[ p ][ 0 ] == i) ) goto center_H;
		           if( (piece[ p ][ 3 ] == k) && (piece[ p ][ 0 ] == l) && (piece[ p ][ 1 ] == i) ) goto center_H;
		        }  continue; center_H:;

		        for( p = 1; p <= nrofpieces; p++ )
		        {
		           if( getPieceType( p ) != CENTER_PIECE ) continue;
		           if( (piece[ p ][ 0 ] == l) && (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) ) goto center_I;
		           if( (piece[ p ][ 1 ] == l) && (piece[ p ][ 2 ] == i) && (piece[ p ][ 3 ] == j) ) goto center_I;
		           if( (piece[ p ][ 2 ] == l) && (piece[ p ][ 3 ] == i) && (piece[ p ][ 0 ] == j) ) goto center_I;
		           if( (piece[ p ][ 3 ] == l) && (piece[ p ][ 0 ] == i) && (piece[ p ][ 1 ] == j) ) goto center_I;
		        }  continue; center_I:;

		        for( p = 1; p <= nrofpieces; p++ )
		        {
		           if( getPieceType( p ) != CENTER_PIECE ) continue;
		           if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 1 ] == j) &&
			       (piece[ p ][ 2 ] == k) && (piece[ p ][ 3 ] == l) ) goto center_end;
		           if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) &&
			       (piece[ p ][ 3 ] == k) && (piece[ p ][ 0 ] == l) ) goto center_end;
		           if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 3 ] == j) &&
			       (piece[ p ][ 0 ] == k) && (piece[ p ][ 1 ] == l) ) goto center_end;
		           if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 0 ] == j) &&
			       (piece[ p ][ 1 ] == k) && (piece[ p ][ 2 ] == l) ) goto center_end;
		        }

		        for( p = 0; p < size; p++ )
		        {
			    printf("-%i -%i -%i -%i 0\n", center_diamond[ p ][ 0 ] + i, center_diamond[ p ][ 1 ] + j,
			                                  center_diamond[ p ][ 2 ] + k, center_diamond[ p ][ 3 ] + l );
//		 	    printf("-%i -%i -%i -%i 0\n", center_diamond[ p ][ 1 ] + i, center_diamond[ p ][ 2 ] + j,
//			                              center_diamond[ p ][ 3 ] + k, center_diamond[ p ][ 0 ] + l );
///			    printf("-%i -%i -%i -%i 0\n", center_diamond[ p ][ 2 ] + i, center_diamond[ p ][ 3 ] + j,
//			                              center_diamond[ p ][ 0 ] + k, center_diamond[ p ][ 1 ] + l );
//			    printf("-%i -%i -%i -%i 0\n", center_diamond[ p ][ 3 ] + i, center_diamond[ p ][ 0 ] + j,
//			                              center_diamond[ p ][ 1 ] + k, center_diamond[ p ][ 2 ] + l );
		        }

		       printf("c found full center block %i %i %i %i\n", i, j, k, l );

		       center_end:;
		    }
		}
	    }


}
void blockThreePattern( )
{
	int size = 2*rows + 2*columns - 8;

	for(int i = 1; i <= nrofbordercolors; i++ )
	    for(int j = 1; j <= nrofcentercolors; j++ )
	    {
		for(int p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != BORDER_PIECE ) continue;
                   int r;
		   for(r = 0; r < 4; r++ ) if( piece[ p ][ r ] == 0 ) break;
		   if( (piece[ p ][ (r+1)%4 ] == i) && (piece[ p ][ (r+2)%4 ] == j) ) goto border_first;
		}

		continue;

		border_first:;

	        for(int k = 1; k <= nrofbordercolors; k++ )
		{
		    for(int p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != BORDER_PIECE ) continue;
                        int r;
		        for(r = 0; r < 4; r++ ) if( piece[ p ][ r ] == 0 ) break;
		        if( (piece[ p ][ (r+1)%4 ] == i) && (piece[ p ][ (r+3)%4 ] == k) ) goto border_second;
		    }

		    continue;

		    border_second:;

		    for(int p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != BORDER_PIECE ) continue;
                        int r;
		        for(r = 0; r < 4; r++ ) if( piece[ p ][ r ] == 0 ) break;
		        if( (piece[ p ][ (r+2)%4 ] == j) && (piece[ p ][ (r+3)%4 ] == k) ) goto border_third;
		    }

		    continue;

		    border_third:;

		    for(int p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != BORDER_PIECE ) continue;
                        int r;
		        for(r = 0; r < 4; r++ ) if( piece[ p ][ r ] == 0 ) break;
		        if( (piece[ p ][ (r+1)%4 ] == i) &&
			    (piece[ p ][ (r+2)%4 ] == j) &&
		            (piece[ p ][ (r+3)%4 ] == k) ) goto border_skip;
		    }

		    for(int p = 0; p < size; p++ ) printf("-%i -%i -%i 0\n",
			border_diamond[ p ][ 0 ] + i, border_diamond[ p ][ 1 ] + j, border_diamond[ p ][ 2 ]+ k );

		    printf("c found border block %i %i %i\n", i, j, k );

		    border_skip:;
		}
	    }

	size = (rows-2) * (columns-2);

	for(int i = 1; i <= nrofcentercolors; i++ )
	    for(int j = 1; j <= nrofcentercolors; j++ )
	    {
		for(int p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != CENTER_PIECE ) continue;
		   if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 1 ] == j) ) goto center_first;
		   if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) ) goto center_first;
		   if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 3 ] == j) ) goto center_first;
		   if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 0 ] == j) ) goto center_first;
		}  continue; center_first:;

	        for(int k = 1; k <= nrofcentercolors; k++ )
		{
		    for(int p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != CENTER_PIECE ) continue;
		        if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 2 ] == k) ) goto center_second;
		        if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 3 ] == k) ) goto center_second;
		        if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 0 ] == k) ) goto center_second;
		        if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 1 ] == k) ) goto center_second;
		    }   continue;  center_second:;

		    for(int p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != CENTER_PIECE ) continue;
		        if( (piece[ p ][ 0 ] == j) && (piece[ p ][ 1 ] == k) ) goto center_third;
		        if( (piece[ p ][ 1 ] == j) && (piece[ p ][ 2 ] == k) ) goto center_third;
		        if( (piece[ p ][ 2 ] == j) && (piece[ p ][ 3 ] == k) ) goto center_third;
		        if( (piece[ p ][ 3 ] == j) && (piece[ p ][ 0 ] == k) ) goto center_third;
		    }   continue; center_third:;

		    for(int p = 1; p <= nrofpieces; p++ )
		    {
		        if( getPieceType( p ) != CENTER_PIECE ) continue;
		        if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 1 ] == j) && (piece[ p ][ 2 ] == k) ) goto center_skip;
		        if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) && (piece[ p ][ 3 ] == k) ) goto center_skip;
		        if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 3 ] == j) && (piece[ p ][ 0 ] == k) ) goto center_skip;
		        if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 0 ] == j) && (piece[ p ][ 1 ] == k) ) goto center_skip;
		    }

		    for(int p = 0; p < size; p++ )
		    {
			printf("-%i -%i -%i 0\n", center_diamond[ p ][ 0 ] + i,
			  center_diamond[ p ][ 1 ] + j, center_diamond[ p ][ 2 ]+ k );
			printf("-%i -%i -%i 0\n", center_diamond[ p ][ 1 ] + i,
			  center_diamond[ p ][ 2 ] + j, center_diamond[ p ][ 3 ]+ k );
			printf("-%i -%i -%i 0\n", center_diamond[ p ][ 2 ] + i,
			  center_diamond[ p ][ 3 ] + j, center_diamond[ p ][ 0 ]+ k );
			printf("-%i -%i -%i 0\n", center_diamond[ p ][ 3 ] + i,
			  center_diamond[ p ][ 0 ] + j, center_diamond[ p ][ 1 ]+ k );
		    }

		    printf("c found center block %i %i %i\n", i, j, k );

		    center_skip:;
		}
	    }
}

void blockTwoPattern( )
{
	int i, j, r, p, size;

	for( i = 1; i <= nrofbordercolors; i++ )
	    for( j = 1; j <= nrofbordercolors; j++ )
	    {
		for( p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != CORNER_PIECE ) continue;
		   if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) ) goto corner_next;
		}

		size = 4;
		for( p = 0; p < size; p++ )
		   printf("-%i -%i 0\n", corner_diamond[ p ][ 0 ] + i, corner_diamond[ p ][ 1 ] + j );

		printf("c found block corder pattern %i %i\n", i, j );

		corner_next:;

		for( p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != BORDER_PIECE ) continue;
		   for( r = 0; r < 4; r++ ) if( piece[ p ][ r ] == 0 ) break;
		   if( (piece[ p ][ (r+1)%4 ] == i) && (piece[ p ][ (r+3)%4 ] == j) ) goto border_cross;
		}

		size = 2*rows + 2*columns - 8;
		for( p = 0; p < size; p++ )
		   printf("-%i -%i 0\n", border_diamond[ p ][ 0 ] + i, border_diamond[ p ][ 2 ] + j );

		printf("c found block border pattern %i X %i\n", i, j );

		border_cross:;
	    }

	size = 2*rows + 2*columns - 8;

	for( i = 1; i <= nrofbordercolors; i++ )
	    for( j = 1; j <= nrofcentercolors; j++ )
	    {
		for( p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != BORDER_PIECE ) continue;
		   for( r = 0; r < 4; r++ ) if( piece[ p ][ r ] == 0 ) break;
		   if( (piece[ p ][ (r+1)%4 ] == i) && (piece[ p ][ (r+2)%4 ] == j) ) goto border_nextA;
		}

		for( p = 0; p < size; p++ )
		   printf("-%i -%i 0\n", border_diamond[ p ][ 0 ] + i, border_diamond[ p ][ 1 ] + j );

		printf("c found block border pattern %i %i X\n", i, j );

		border_nextA:;

		for( p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != BORDER_PIECE ) continue;
		   for( r = 0; r < 4; r++ ) if( piece[ p ][ r ] == 0 ) break;
		   if( (piece[ p ][ (r+3)%4 ] == i) && (piece[ p ][ (r+2)%4 ] == j) ) goto border_nextB;
		}

		for( p = 0; p < size; p++ )
		   printf("-%i -%i 0\n", border_diamond[ p ][ 2 ] + i, border_diamond[ p ][ 1 ] + j );

		printf("c found block border pattern X %i %i \n", j, i );

		border_nextB:;
	    }

	size = (rows-2) * (columns-2);

	for( i = 1; i <= nrofcentercolors; i++ )
	    for( j = 1; j <= nrofcentercolors; j++ )
	    {
		for( p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != CENTER_PIECE ) continue;
		   if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 1 ] == j) ) goto two_next;
		   if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 2 ] == j) ) goto two_next;
		   if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 3 ] == j) ) goto two_next;
		   if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 0 ] == j) ) goto two_next;
		}

		for( p = 0; p < size; p++ )
		{
		   printf("-%i -%i 0\n", center_diamond[ p ][ 0 ] + i, center_diamond[ p ][ 1 ] + j );
		   printf("-%i -%i 0\n", center_diamond[ p ][ 1 ] + i, center_diamond[ p ][ 2 ] + j );
		   printf("-%i -%i 0\n", center_diamond[ p ][ 2 ] + i, center_diamond[ p ][ 3 ] + j );
		   printf("-%i -%i 0\n", center_diamond[ p ][ 3 ] + i, center_diamond[ p ][ 0 ] + j );
		}

		printf("c found block pattern %i %i X X\n", i, j );

		two_next:;

		if( j < i ) continue;

		for( p = 1; p <= nrofpieces; p++ )
		{
		   if( getPieceType( p ) != CENTER_PIECE ) continue;
		   if( (piece[ p ][ 0 ] == i) && (piece[ p ][ 2 ] == j) ) goto two_cross;
		   if( (piece[ p ][ 1 ] == i) && (piece[ p ][ 3 ] == j) ) goto two_cross;
		   if( (piece[ p ][ 2 ] == i) && (piece[ p ][ 0 ] == j) ) goto two_cross;
		   if( (piece[ p ][ 3 ] == i) && (piece[ p ][ 1 ] == j) ) goto two_cross;
		}

		for( p = 0; p < size; p++ )
		{
		   printf("-%i -%i 0\n", center_diamond[ p ][ 0 ] + i, center_diamond[ p ][ 2 ] + j );
		   printf("-%i -%i 0\n", center_diamond[ p ][ 1 ] + i, center_diamond[ p ][ 3 ] + j );
		   printf("-%i -%i 0\n", center_diamond[ p ][ 2 ] + i, center_diamond[ p ][ 0 ] + j );
		   printf("-%i -%i 0\n", center_diamond[ p ][ 3 ] + i, center_diamond[ p ][ 1 ] + j );
		}

		printf("c found block pattern %i X %i X\n", i, j );

		two_cross:;
	    }
}


void printAtMostOne (int *list, int size) {
#ifdef BLOCKED
	if( size <= 5 )
#endif
	{
	    for (int i = 0; i < size; i++)
	        for (int j = i + 1; j < size; j++)
	    	    printf("%i %i 0\n", -list[ i ], -list[ j ] );
	    return;
	}

        int i = 1;
	int l = -list[ 0 ];

	do
	{
            int d;
	    int m = -list[ i++ ];
	    int r = -list[ i++ ];

	    printf("%i %i 0\n", l, m ); printf("%i %i 0\n", l, r ); printf("%i %i 0\n", m, r );

	    if( i < size )
	    {
		if( i == size - 1 )   d = -list[ i++ ];
		else d = -(++vars_offset);

	 	printf("%i %i 0\n", l, d ); printf("%i %i 0\n", m, d ); printf("%i %i 0\n", r, d );
	    }
	    l = -d;
	}
	while (i < size);
}

void printDiamonds( )
{
	int offset = dia_map[ 0 ], list[ 257 ];

	for(int i = 1; i <= nrofdiamonds; i++ )
	{
	    dia_map[ i ] = offset;
	    if( getDiamondType( i ) )
	    {
		for(int j = 1; j <= nrofbordercolors; j++ ) { printf("%i ", offset + j ); list[ j - 1 ] = offset + j; }
		printf("0\n");

		printAtMostOne( list, nrofbordercolors );

		offset += nrofbordercolors;
	    }
	    else
	    {
		for(int j = 1; j <= nrofcentercolors; j++ ) { printf("%i ", offset + j ); list[ j - 1] = offset+ j; }
		printf("0\n");

		printAtMostOne( list, nrofcentercolors );

		offset += nrofcentercolors;
	    }
	}
}

int getVariable( int index, int row, int column )
{
	int result = var_map[ index ];

	if( (row ==       0 ) && (column ==          0 ) ) { assert(getPieceType(index) == CORNER_PIECE); return result + 1; }
	if( (row ==       0 ) && (column == (columns-1)) ) { assert(getPieceType(index) == CORNER_PIECE); return result + 2; }
	if( (row == (rows-1)) && (column ==          0 ) ) { assert(getPieceType(index) == CORNER_PIECE); return result + 3; }
	if( (row == (rows-1)) && (column == (columns-1)) ) { assert(getPieceType(index) == CORNER_PIECE); return result + 4; }

	if (row    ==             0) { assert(getPieceType(index) == BORDER_PIECE ); return result + column; }
	if (column ==             0) { assert(getPieceType(index) == BORDER_PIECE ); return result + columns - 2 + row; }
	if (column == (columns - 1)) { assert(getPieceType(index) == BORDER_PIECE ); return result + columns + rows - 4 + row; }
	if (row    ==    (rows - 1)) { assert(getPieceType(index) == BORDER_PIECE ); return result + columns + 2 * rows - 6 + column; }

	assert(getPieceType(index) == CENTER_PIECE );
        return result + column + (row - 1)*(columns - 2);
}

void printNegative( int index )
{
	int a, b, c, d;
	int i, j, k, rotation, start = var_map[ index ], color[ 4 ];
	int size, type  = getPieceType( index );

	if( type == CORNER_PIECE )
	{
	    assert( piece[ index ][ 0 ] == 0 );
	    assert( piece[ index ][ 3 ] == 0 );

	    for( rotation = 0; rotation < 2; rotation++ )
		color[ rotation ] = piece[ index ][ rotation + 1 ];

	    size   = 4;
	    for( i = 0; i < size; i++ )
	    {	printf("-%i %i 0\n", start + i + 1, corner_diamond[ i ][ 0 ] + color[ 0 ] );
		printf("-%i %i 0\n", start + i + 1, corner_diamond[ i ][ 1 ] + color[ 1 ] ); }
	}
	if( type == BORDER_PIECE )
	{
	    size = 2*rows + 2*columns - 8;

	    for( rotation = 0; rotation < 4; rotation++ )
		if( piece[ index ][ rotation ] == 0 ) break;

	    color[ 0 ] = piece[ index ][ (rotation + 1) % 4 ];
	    color[ 1 ] = piece[ index ][ (rotation + 2) % 4 ];
	    color[ 2 ] = piece[ index ][ (rotation + 3) % 4 ];

	    for( i = 0; i < size; i++ )
	    {	printf("-%i %i 0\n", start + i + 1, border_diamond[ i ][ 0 ] + color[ 0 ] );
		printf("-%i %i 0\n", start + i + 1, border_diamond[ i ][ 1 ] + color[ 1 ] );
		printf("-%i %i 0\n", start + i + 1, border_diamond[ i ][ 2 ] + color[ 2 ] ); }
	}
	if( type == CENTER_PIECE )
	{
	    int fbcolor[ nrofcentercolors ];
	    int fbcolors = 0, piececolors;
	    int piecetype;

	    int lcolor[ 4 ];
	    int lcolors = 4;

	    for( rotation = 0; rotation < 4; rotation++ ) lcolor[ rotation ] = piece[ index ][ rotation ];

	    for( i = 0; i < lcolors; i++ )
	        for( j = i + 1; j < lcolors; j++ )
		    if( lcolor[ i ] == lcolor[ j ] ) { lcolor[ j-- ] = lcolor[ --lcolors ]; }

	    for( i = 0; i < nrofcentercolors; i++ ) fbcolor[ i ] = i + 1;

	    for( rotation = 0; rotation < 4; rotation++ )
	    {
		color[ rotation ] = piece[ index ][ rotation ];
		fbcolor[ color[ rotation ] - 1 ] = 0;
	    }

	    for( i = 0; i < nrofcentercolors; i++ ) if( fbcolor[ i ] ) fbcolor[ fbcolors++ ] = fbcolor[ i ];

	    piececolors = nrofcentercolors - fbcolors;

	    assert( lcolors == piececolors );

	    size = (rows-2) * (columns-2);

	    if( piececolors == 1 ) 		// Type is 1
	    {
	    	for( i = 0; i < size; i++ )
		    for( j = 0; j < 4; j++ )
		        printf("-%i %i 0\n", start + i + 1, center_diamond[ i ][ j ] + color[ 0 ] );
		return;
	    }


	    if( fbcolors )
	    {
	      for( i = 0; i < size; i++ )
		for( j = 0; j < 4; j++ )
		{
#ifdef FBCOLORS
		    for( k = 0; k < fbcolors; k++ )
		        printf("-%i -%i 0\n", start + i + 1, center_diamond[ i ][ j ] + fbcolor[ k ] );
#endif
#ifdef LIMITED
		    printf("-%i ", start + i + 1);
		    for( k = 0; k < lcolors; k++ )
		        printf("%i ", center_diamond[ i ][ j ] + lcolor[ k ] );
		    printf("0\n");
#endif
		}
	    }

	    a = color[ 0 ]; b = color[ 1 ]; c = color[ 2 ]; d = color[ 3 ];

	    if( piececolors == 2 )
	    {
		if( (a==c) && (b==d) )		// Type 3   A -> B -> A -> B -> A
		{
		    assert( color[ 0 ] != color[ 1 ] );

	    	    for( i = 0; i < size; i++ )
		        for( j = 0; j < 4; j++ )
			{
		            printf("-%i %i %i 0\n", start + i + 1, center_diamond[ i ][ j ] + a, center_diamond[ i ][ (j+1)%4 ] + a );
		            printf("-%i %i %i 0\n", start + i + 1, center_diamond[ i ][ j ] + b, center_diamond[ i ][ (j+1)%4 ] + b );
			}
		    return;
		}
		else if( (a==c) || (b==d) )	// Type 2  B <--> AAA
		{
		    if( (a==b) && (a==d) ) { d = c; c = a; }
		    if( (a==c) && (a==d) ) { d = b; b = a; }
		    if( (b==c) && (b==d) ) { d = a; a = b; }

	    	    for( i = 0; i < size; i++ )
		    {
		        printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + d,
			    center_diamond[ i ][ 1 ] + d, center_diamond[ i ][ 2 ] + d, center_diamond[ i ][ 3 ] + d );

		        printf("-%i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + a, center_diamond[ i ][ 2 ] + a );
		        printf("-%i %i %i 0\n", start + i + 1, center_diamond[ i ][ 1 ] + a, center_diamond[ i ][ 3 ] + a );

		        for( j = 0; j < 4; j++ )
		            printf("-%i %i %i 0\n", start + i + 1, center_diamond[ i ][ j ] + a, center_diamond[ i ][ (j + 1) % 4 ] + a );
		    }
		    return;
		}
		else				// Type 4  A <--> B
		{
		    assert( color[ 0 ] != color[ 2 ] );

	    	    for( i = 0; i < size; i++ )
		        for( j = 0; j < 2; j++ )
			{
		            printf("-%i %i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + color[ 0 ], center_diamond[ i ][ (j + 2) % 4 ] + color[ 0 ] );
		            printf("-%i %i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + color[ 2 ], center_diamond[ i ][ (j + 2) % 4 ] + color[ 2 ] );
			}
		    return;
		}
	    }
	    else if( piececolors == 3 )
	    {
		if( (a==c) || (b==d) )		// Type 6   ABAC
		{
		    if( b == d ) { b = c; c = d; d = a; a = c; }

	    	    for( i = 0; i < size; i++ )
		    {
 		        printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + b,
			    center_diamond[ i ][ 1 ] + b, center_diamond[ i ][ 2 ] + b, center_diamond[ i ][ 3 ] + b );
		        printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + d,
			    center_diamond[ i ][ 1 ] + d, center_diamond[ i ][ 2 ] + d, center_diamond[ i ][ 3 ] + d );

		        for( j = 0; j < 4; j++ )
			{
		            printf("-%i %i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + a, center_diamond[ i ][ (j + 1) % 4 ] + a );
#ifndef POSONLY
		            printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + b, center_diamond[ i ][ (j + 2) % 4 ] + d );
		            printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + d, center_diamond[ i ][ (j + 2) % 4 ] + b );
#endif
			}
		    }
		    return;
		}
		else 		// Type 5	AABC
		{
		    if( b == c ) { c = d; d = a; a = b; }
		    if( c == d ) { c = a; a = d; d = b; b = a; }
		    if( d == a ) { d = c; c = b; b = a;}

	    	    for( i = 0; i < size; i++ )
		    {
		        printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + c,
			    center_diamond[ i ][ 1 ] + c, center_diamond[ i ][ 2 ] + c, center_diamond[ i ][ 3 ] + c );
		        printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + d,
			    center_diamond[ i ][ 1 ] + d, center_diamond[ i ][ 2 ] + d, center_diamond[ i ][ 3 ] + d );

			printf("-%i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + a, center_diamond[ i ][ 2 ] + a );
			printf("-%i %i %i 0\n", start + i + 1, center_diamond[ i ][ 1 ] + a, center_diamond[ i ][ 3 ] + a );

		        for( j = 0; j < 4; j++ )
			{
		            printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + d, center_diamond[ i ][ (j + 3) % 4 ] + c );
		            printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + c, center_diamond[ i ][ (j + 1) % 4 ] + d );
#ifndef POSONLY
		            printf("-%i -%i -%i %i 0\n", start + i + 1, center_diamond[ i ][ j ] + a,
				center_diamond[ i ][ (j + 1) % 4 ] + a, center_diamond[ i ][ (j + 2) % 4 ] + c );
#endif
			}
		    }
		    return;
		}
	    }
	    else				// Type 7
	    {
	        for( i = 0; i < size; i++ )
		{
		    printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + a,
			center_diamond[ i ][ 1 ] + a, center_diamond[ i ][ 2 ] + a, center_diamond[ i ][ 3 ] + a );
		    printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + b,
			center_diamond[ i ][ 1 ] + b, center_diamond[ i ][ 2 ] + b, center_diamond[ i ][ 3 ] + b );
		    printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + c,
			center_diamond[ i ][ 1 ] + c, center_diamond[ i ][ 2 ] + c, center_diamond[ i ][ 3 ] + c );
		    printf("-%i %i %i %i %i 0\n", start + i + 1, center_diamond[ i ][ 0 ] + d,
			center_diamond[ i ][ 1 ] + d, center_diamond[ i ][ 2 ] + d, center_diamond[ i ][ 3 ] + d );

		    for( j = 0; j < 4; j++ )
		    {
		     	printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + a, center_diamond[ i ][ (j+1)%4 ] + b );
		     	printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + b, center_diamond[ i ][ (j+1)%4 ] + c );
#ifndef POSONLY
		     	printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + c, center_diamond[ i ][ (j+1)%4 ] + d );
		     	printf("-%i -%i %i 0\n", start + i + 1,
				center_diamond[ i ][ j ] + d, center_diamond[ i ][ (j+1)%4 ] + a );
#endif
		     }
		}
		return;
	    }
	}
}

void atLeastOnePiece( int index )
{
	int start = var_map[ index ];
	int size, type  = getPieceType( index );
	int row, column;
	int list[ 257 ];

	if( type == CORNER_PIECE ) size = 4;
	if( type == BORDER_PIECE ) size = 2*rows + 2*columns - 8;
	if( type == CENTER_PIECE ) size = (rows-2) * (columns-2);

	for(int i = 1; i <= size; i++ ){  printf("%i ", start + i ); list[ i - 1 ] = start + i; }
	printf("0\n");

#ifdef ATMOSTONE
	printAtMostOne( list, size );
#endif
	// printMapping

	for(int i = 0; i < size; i++ )
	{
	    if( type == CORNER_PIECE )
	    {  {if( (i % 2) == 0 ) column = 1; else column = columns; } { if( i < 2 ) row = 1; else row = rows; } }
	    if( type == BORDER_PIECE )
	    { if     ( i < (columns          - 2) ) { row =                      1; column = i + 2 ; }
	      else if( i < (columns +   rows - 4) ) { row = i - columns        + 4; column = 1; }
	      else if( i < (columns + 2*rows - 6) ) { row = i - columns - rows + 6; column = columns; }
	      else                                  { row =               rows    ; column = columns + i - size; }
	    }
	    if( type == CENTER_PIECE )
	    { column = 2 + (i % (columns-2) ); row = 2 + i / (columns - 2); }

	    printf("c %i [%i,%i] %i\n", start + i + 1, row, column, index );
	}
}

void atLeastOneCell( int row, int column, int *list, int size ) {
  for (int i = 0; i < size; i++)
    printf("%i ", getVariable( list[ i ], row, column ) );
  printf("0\n");
}

void printPositive( )
{
	int i, j, map_index = 0;
	int corner[ 4 ], nrofcorners = 0;
	int border[ 2*rows + 2*columns - 8 ], nrofborders = 0;
	int center[ (rows-2) * (columns-2) ], nrofcenters = 0;

	for( i = 1; i <= nrofpieces; i++ )
	{
	    var_map[ i ] = map_index;

	    if( getPieceType( i ) == CORNER_PIECE )
	    {	corner[ nrofcorners++ ] = i; map_index += 4; }
	    if( getPieceType( i ) == BORDER_PIECE )
	    {	border[ nrofborders++ ] = i; map_index += (2*rows + 2* columns - 8); }
	    if( getPieceType( i ) == CENTER_PIECE )
	    {	center[ nrofcenters++ ] = i; map_index += (rows-2) * (columns-2); }
	}

	assert( nrofcorners == 4 );
	assert( nrofborders == (2*rows + 2*columns - 8) );
	assert( nrofcenters == ((rows-2) * (columns -2)) );

	atLeastOneCell(        0,           0, corner, nrofcorners );
	atLeastOneCell(        0, columns - 1, corner, nrofcorners );
	atLeastOneCell( rows - 1,           0, corner, nrofcorners );
	atLeastOneCell( rows - 1, columns - 1, corner, nrofcorners );

	for( i = 1; i < columns - 1; i++ ) atLeastOneCell( 0       ,           i, border, nrofborders );
	for( i = 1; i <    rows - 1; i++ ) atLeastOneCell( i       ,           0, border, nrofborders );
	for( i = 1; i <    rows - 1; i++ ) atLeastOneCell( i       , columns - 1, border, nrofborders );
	for( i = 1; i < columns - 1; i++ ) atLeastOneCell( rows - 1,           i, border, nrofborders );

	for( i = 1; i < rows - 1; i++ )
	   for( j = 1; j < columns - 1; j++ ) atLeastOneCell( i, j, center, nrofcenters );

	vars_offset = nrofvars;

	for( i = 1; i <= nrofpieces; i++ ) atLeastOnePiece( i );  // with mapping

	dia_map[ 0 ] = nrofcorners * nrofcorners + nrofborders * nrofborders + nrofcenters * nrofcenters;

	nrofdiamonds = 2 * columns * rows - columns - rows;

	printDiamonds ();

	corner_diamond[ 0 ][ 0 ] = dia_map[                        1 ]; corner_diamond[ 0 ][ 1 ] = dia_map[              columns   ];
	corner_diamond[ 1 ][ 0 ] = dia_map[              2*columns-1 ]; corner_diamond[ 1 ][ 1 ] = dia_map[              columns-1 ];
	corner_diamond[ 2 ][ 0 ] = dia_map[ nrofdiamonds-2*columns+2 ]; corner_diamond[ 2 ][ 1 ] = dia_map[ nrofdiamonds-columns+2 ];
	corner_diamond[ 3 ][ 0 ] = dia_map[ nrofdiamonds             ]; corner_diamond[ 3 ][ 1 ] = dia_map[ nrofdiamonds-columns+1 ];

	for( i = 1; i < columns - 1; i++ )
	{   border_diamond[ i - 1 ][ 0 ] = dia_map[       1 + i ];
	    border_diamond[ i - 1 ][ 1 ] = dia_map[ columns + i ];
	    border_diamond[ i - 1 ][ 2 ] = dia_map[           i ]; }
	for( i = 1; i <    rows - 1; i++ )
	{   border_diamond[ columns - 3 + i ][ 0 ] = dia_map[ columns + (i-1)*(2*columns-1) ];
	    border_diamond[ columns - 3 + i ][ 1 ] = dia_map[       1 +   i  *(2*columns-1) ];
	    border_diamond[ columns - 3 + i ][ 2 ] = dia_map[ columns +   i  *(2*columns-1) ]; }
	for( i = 1; i <    rows - 1; i++ )
	{   border_diamond[ columns + rows - 5 + i ][ 0 ] = dia_map[           (i+1)*(2*columns-1) ];
	    border_diamond[ columns + rows - 5 + i ][ 1 ] = dia_map[ columns-1 + i  *(2*columns-1) ];
	    border_diamond[ columns + rows - 5 + i ][ 2 ] = dia_map[             i  *(2*columns-1) ]; }
	for( i = 1; i < columns - 1; i++ )
	{   border_diamond[ columns + 2*rows - 7 + i ][ 0 ] = dia_map[ nrofdiamonds -   columns + i + 1 ];
	    border_diamond[ columns + 2*rows - 7 + i ][ 1 ] = dia_map[ nrofdiamonds - 2*columns + i + 2 ];
	    border_diamond[ columns + 2*rows - 7 + i ][ 2 ] = dia_map[ nrofdiamonds -   columns + i + 2 ]; }

	for( i = 1; i < rows - 1; i++ )
	   for( j = 1; j < columns - 1; j++ )
	   {
		int _tmp = i * (2*columns-1) + j;
		center_diamond[ j - 1 + (i-1)*(columns-2) ][ 0 ] = dia_map[ _tmp - columns + 1 ];
		center_diamond[ j - 1 + (i-1)*(columns-2) ][ 1 ] = dia_map[ _tmp +           1 ];
		center_diamond[ j - 1 + (i-1)*(columns-2) ][ 2 ] = dia_map[ _tmp + columns     ];
		center_diamond[ j - 1 + (i-1)*(columns-2) ][ 3 ] = dia_map[ _tmp               ]; }
#ifdef MAPPING
	for( i = 1; i <= nrofpieces; i++ ) printNegative( i );
#endif
}

int computeClauses( int size )
{
	int result = size > 0;
#ifdef BLOCKED
	if( size >= 2 )
	{
	   if( size <= 5 ) result += (size * (size - 1)) / 2;
	   else		   result += 3*(size-2);
	}
#else
	if( size >= 2 ) result += (size * (size - 1)) / 2;
#endif
	return result;
}

int computeDummies (int size) {
  if (size <= 5)
    return 0;
  return (int) (size - 3) / 2;
}

void printDuplicates( )
{
	int d, i, j, r, size, type, tmp, startA, startB;

	int duplicate[ nrofpieces + 1 ];
	int dlist[ nrofpieces + 1 ], dsize;

	for( i = 1; i <= nrofpieces; i++ ) duplicate[ i ] = i;

	for( i = 1; i <= nrofpieces; i++ )
	    for( j = i + 1; j <= nrofpieces; j++ )
		for( r = 0; r < 4; r++ )
		   if( (piece[ i ][ 0 ] == piece[ j ][ (r+0)%4 ]) && (piece[ i ][ 1 ] == piece[ j ][ (r+1)%4 ]) &&
		       (piece[ i ][ 2 ] == piece[ j ][ (r+2)%4 ]) && (piece[ i ][ 3 ] == piece[ j ][ (r+3)%4 ]) )
		   {  duplicate[ j ] = duplicate[ i ]; duplicate[ i ] = j; r = 4; j = nrofpieces; }

	for( i = 1; i <= nrofpieces; i++ )
	{
	    if( duplicate[ i ] <= i ) continue;

	    type = getPieceType( i );   // Wellicht een getSize( int index ) maken

	    if( type == CORNER_PIECE ) size = 4;
	    if( type == BORDER_PIECE ) size = 2*rows + 2*columns - 8;
	    if( type == CENTER_PIECE ) size = (rows-2) * (columns-2);

	    startA = var_map[ i ];

	    tmp = i;

	    do
	    {
	        tmp    = duplicate[ tmp ];
		startB = var_map  [ tmp ];

		for( j = 1; j <= size; j++ ) printf("-%i -%i 0\n", startA + j, startB + j );
	    }
	    while( duplicate[ tmp ] > i );
	}
#ifdef PATTERN
	for( i = 1; i <= nrofpieces; i++ )
	{
	    tmp   = i;
	    dsize = 0;

	    do
	    {
		dlist[ dsize++ ] = var_map  [ tmp ] + 1;
	        tmp              = duplicate[ tmp ];
	    }
	    while(  tmp > i );

	    printf("c dsize = %i\n", dsize );

	    if( tmp != i ) continue;

	    printf("c dsize = %i\n", dsize );

	        type = getPieceType( i );   // Wellicht een getSize( int index ) maken

	        if( type == CORNER_PIECE )
		{
		   size = 4;
		   for( j = 0; j < size; j++ )
		   {
		        for( d = 0; d < dsize; d++ ) printf("%i ", dlist[ d ] + j );
			printf("-%i -%i 0\n",
			  corner_diamond[ j ][ 0 ] + piece[ i ][ 1 ],
			  corner_diamond[ j ][ 1 ] + piece[ i ][ 2 ] );
		   }
		}
	        if( type == BORDER_PIECE )
		{
		    size = 2*rows + 2*columns - 8;

		    for( r = 0; r < 4; r++ ) if( piece[ i ][ r ] == 0 ) break;

		    for( j = 0; j < size; j++ )
		    {
		        for( d = 0; d < dsize; d++ ) printf("%i ", dlist[ d ] + j );
			printf("-%i -%i -%i 0\n",
			  border_diamond[ j ][ 0 ] + piece[ i ][ (r+1)%4 ],
			  border_diamond[ j ][ 1 ] + piece[ i ][ (r+2)%4 ],
			  border_diamond[ j ][ 2 ] + piece[ i ][ (r+3)%4 ] );
		    }
		}
	        if( type == CENTER_PIECE )
		{
		   size = (rows-2) * (columns-2);
		   for( j = 0; j < size; j++ )
		      for( r = 0; r < 4; r++ )
		      {
		        for( d = 0; d < dsize; d++ ) printf("%i ", dlist[ d ] + j );
			printf("-%i -%i -%i -%i 0\n",
			  center_diamond[ j ][ 0 ] + piece[ i ][ (r+0)%4 ],
			  center_diamond[ j ][ 1 ] + piece[ i ][ (r+1)%4 ],
			  center_diamond[ j ][ 2 ] + piece[ i ][ (r+2)%4 ],
			  center_diamond[ j ][ 3 ] + piece[ i ][ (r+3)%4 ] );
		      }

	    	}
	}
#endif
}

void printConnections( )
{
	int index, rotation;

	int a = (rows-2) * (columns -2);
	int b = 2*rows + 2 * columns -8;
	int c = 2*nrofbordercolors*(rows+columns-2);
	int d = nrofcentercolors * (2*(rows-1)*(columns-1) - rows - columns + 2);

	nrofvars = 16 + a*a + b*b + c + d;

	nrofclauses = 10 * nrofvars;

	printf("p cnf %i %i\n", nrofvars, nrofclauses );

	printPositive();

	printDuplicates();
#ifdef NOTWO
	blockTwoPattern();
#endif
#ifdef NOTHREE
	blockThreePattern();
#endif
#ifdef NOFOUR
	blockFourPattern();
#endif
}

unsigned int map( int color, int center_flag )
{
	if( color == 0 ) return 0;

	if( color_map[ color ] ) return color_map[ color ];

	if( center_flag ) color_map[ color ] = ++nrofcentercolors;
	else              color_map[ color ] = ++nrofbordercolors;

	return color_map[ color ];
}

void initPiece( int index, int north, int east, int south, int west ) {

  piece[ index ][ 0 ] = map( north, east  * west  );
  piece[ index ][ 1 ] = map( east,  north * south ) ;
  piece[ index ][ 2 ] = map( south, east  * west  );
  piece[ index ][ 3 ] = map( west,  north * south );

//	for(int rotation = 0; rotation < 4; rotation++ )
//	    if( piece[ index ][ rotation ] > nrofcolors ) nrofcolors = piece[ index ][ rotation ];

//	if( north && east && south && west )
//	    for(int rotation = 0; rotation < 4; rotation++ )
//	        if( piece[ index ][ rotation ] > nrofcentercolors ) nrofcentercolors = piece[ index ][ rotation ];
}

void turnCorners( )
{
	for (int index = 1; index <= nrofpieces; index++)
	    if( getPieceType( index ) == CORNER_PIECE )
	    {
		if( (piece[ index ][ 0 ] == 0) && (piece[ index ][ 1 ] == 0) )
		{ piece[ index ][ 1 ] = piece[ index ][ 2 ]; piece[ index ][ 2 ] = piece[ index ][ 3 ]; piece[index ][ 3 ] = 0;}
		if( (piece[ index ][ 1 ] == 0) && (piece[ index ][ 2 ] == 0) )
		{ piece[ index ][ 1 ] = piece[ index ][ 3 ]; piece[ index ][ 2 ] = piece[ index ][ 0 ]; piece[index ][ 3 ] = 0; piece[ index ][ 0 ] = 0;}
		if( (piece[ index ][ 2 ] == 0) && (piece[ index ][ 3 ] == 0) )
		{ piece[ index ][ 2 ] = piece[ index ][ 1 ]; piece[ index ][ 1 ] = piece[ index ][ 0 ]; piece[index ][ 0 ] = 0;}
	    }
}

void parse( char * filename  )
{
	int nrofcorners = 0, nrofborders = 0;
	FILE *file;

	file = fopen( filename , "r");

	nrofpieces = 0;
#ifdef HINTS
	fscanf( file, "%i\n", &nrofhints );

	for(int i = 0; i < nrofhints; i++ )
	    fscanf( file, "%i %i %i %i\n", &hint[ i ][ 0 ], & hint[ i ][ 1 ], &hint[ i ][ 2 ], &hint[ i ][ 3 ] );
#endif
	if( file )
	   while( !feof(file) )
	   {
   	        int north, east, south, west;
		fscanf( file, "%i %i %i %i\n", &north, &east, &south, &west );
		initPiece( ++nrofpieces, north, east, south, west );
		int type = getPieceType( nrofpieces );
		if( type == CORNER_PIECE ) nrofcorners++;
		if( type == BORDER_PIECE ) nrofborders++;
	   }

	assert( nrofcorners == 4 );
	assert( (nrofborders % 2) == 0 );

	for (int _rows = 3; _rows <= 16; _rows++ )
	{
	   int _columns = (nrofborders / 2) - _rows + 4;
	   if( (_rows * _columns) == nrofpieces ) {rows = _rows; columns = _columns; break; }
	}

	assert( (rows * columns) == nrofpieces );
}

void shuffle( int seed )
{

	srand( seed );

	// DO NOT SHUFFLE HINT PIECES!!!

	for(int i = 1; i <= nrofpieces; i++ )
	{
	   for(int j = 0; j < nrofhints; j++ ) if( hint[ j ][ 2 ] == i ) goto next_piece;

   	   int tmp;
	   int rot = rand() % 4;

	   if( rot == 1 ) { tmp = piece[ i ][ 0 ]; piece[ i ][ 0 ] = piece[ i ][ 1 ];
	     piece[ i ][ 1 ] = piece[ i ][ 2 ]; piece[ i ][ 2 ] = piece[ i ][ 3 ]; piece[ i ][ 3 ] = tmp; }

	   if( rot == 2 ) { tmp = piece[ i ][ 0 ]; piece[ i ][ 0 ] = piece[ i ][ 2 ]; piece[ i ][ 2 ] = tmp;
	     tmp = piece[ i ][ 1 ]; piece[ i ][ 1 ] = piece[ i ][ 3 ]; piece[ i ][ 3 ] = tmp; }

	   if( rot == 1 ) { tmp = piece[ i ][ 0 ]; piece[ i ][ 0 ] = piece[ i ][ 3 ];
	     piece[ i ][ 3 ] = piece[ i ][ 2 ]; piece[ i ][ 2 ] = piece[ i ][ 1 ]; piece[ i ][ 1 ] = tmp; }

	    next_piece:;
	}
}

void printHints( )
{
#ifdef HINTS
	printf("c file contains %i hints\n", nrofhints );
#endif
	for (int i = 0; i < nrofhints; i++ )
	{
	     int var  = getVariable ( hint[ i ][ 2 ], hint[ i ][ 0 ] - 1, hint[ i ][ 1 ] - 1 );
	     int type = getPieceType( hint[ i ][ 2 ] );

	     printf("%i 0\n", var );

	     if( type == CENTER_PIECE )
	     {
		int index = var - 1 - var_map[ hint[ i ][ 2 ] ];

		for(int j = 0 ; j < 4; j++ )
		    printf("%i 0\n", center_diamond[ index ][ j ] + piece[ hint[ i ][ 2 ] ][ (j + 4 - hint[ i ][ 3 ]) % 4 ] );
//		    printf("%i 0\n", center_diamond[ index ][ j ] + piece[ hint[ i ][ 2 ] ][ (j + hint[ i ][ 3 ]) % 4 ] );
	     }
	}
}

int main( int argc , char ** argv ) {

	nrofvars     = 0;
	nrofdummies  = 0;
	nrofclauses  = 0;
	nrofcolors   = 0;
	nrofbordercolors   = 0;
	nrofcentercolors   = 0;

	for (int i = 0; i < 30; i++) color_map[ i ] = 0;

	if( argc > 1 ) {
	   parse( argv[ 1 ] );
	   if( argc > 2 ) shuffle( atoi(argv[2]) );
	}
	else {
	  printf("No input file given\n");
	  return 0;
	}

//	nrofbordercolors = nrofcolors - nrofcentercolors;

	turnCorners();

	printConnections();

	printHints();
  return 1;
}
