#ifndef GRID_INCLUDED
#define GRID_INCLUDED 1

#include <SFML/Window.hpp> 
#include <initializer_list>
#include <iostream>
#include <vector>
#include "figure.h"


class grid 
{
   struct cell 
	 {
      bool s0; // Current state.
      bool s1; // Next state.	

      cell( )
	: s0( false ),
	  s1( false ) {}

   };

   unsigned int xsize;
   unsigned int ysize;
   cell* c;

public:



   inline bool isonchar( char c ) { return c == 'X' || c == '#' || 
                                           c == 'O' || c == '0'; }

   inline bool isoffchar( char c ) { return c == '.'; } 


   grid( unsigned int xsize, unsigned int ysize )
	: xsize( xsize ),
	  ysize( ysize ),
      c( new cell [xsize * ysize] )
	{ }

   grid( const grid&  );
   grid( grid&& );
   void operator = ( const grid&  );
   void operator = ( grid&&  );

  ~grid( ) { delete c; }

   cell* operator [] ( unsigned int x ) { return c + x * ysize; }
   const cell* operator [] ( unsigned int x ) const { return c + x * ysize; }

   void plot( ) const;

   void clear( );

	 int neighbours( const int );

   void addfigure( unsigned int x, unsigned int y,
                   std::initializer_list<const char* > p );
 
   void addfigure( unsigned int x, unsigned int y, const figure& f ); 

   void nextgeneration( );

   friend std::ostream& operator << ( std::ostream& stream, const grid& g );
};

std::ostream& operator << ( std::ostream& stream, const grid& g);
std::ostream& operator << ( std::ostream& stream, const grid::wrongchar& w ); 

#endif 

