#include "grid.h"

#define GRID_SCALE 2

// constructors
grid::grid( const grid& g )
	: xsize( g. xsize ),
	  ysize( g. ysize ),
    c( new cell [xsize * ysize] )
{
  for(int i = 0; i < xsize * ysize; i++)
	c[i] = g. c[i];	   
}

grid::grid( grid&& g )
  : xsize( std::move(g. xsize) ),
	  ysize( std::move(g. ysize) )
{
  for(int i = 0; i < xsize * ysize; i++)
	{
	  c[i] = g. c[i];	
	}

  g. xsize = 0;
	g. ysize = 0;
	delete g. c;
}

// assignment 

void grid::operator = ( const grid& g )
{
	if( g. c != c )
	{
		delete c;
		xsize = g. xsize;
		ysize = g. ysize;
		c = new cell [xsize * ysize];
	}
}

void grid::operator = ( grid&& g )
{
  xsize = g. xsize;
	ysize = g. ysize;  
  std::swap(c, g. c);
}

// figure methods
 
void grid::addfigure( unsigned int x, unsigned int y, const figure& f )
{
	int line = 0;
	for(auto vectIt = f. repr. begin(); vectIt != f. repr. end(); ++vectIt)
	{
		std::string f = *vectIt;
		int size = f. size();
		int i = (x+line) * xsize + y % ysize;

		for(int j = 0; j < size; j++ )
		{
			c[i+j]. s0 = isonchar(f[j]); 
		}

		line++;
	}
}

void grid::addfigure( unsigned int x, unsigned int y,
                   std::initializer_list< const char* > p )
{
	std::vector< std::string> v;
  v. insert(v. end(), p. begin(), p. end()  );
	figure temp(p);

	addfigure(x,y,temp);
}

// computing events

void grid::nextgeneration( )
{
	for(int i = 0; i < xsize * ysize; i++ )
	{

		int friends = neighbours(i);
		if ( c[i]. s0 && friends < 2 )
			c[i]. s1 = false;
		else if ( c[i]. s0 && friends == 2 || friends == 3 )
			c[i]. s1 = true;
		else if ( c[i]. s0 && friends > 3 )
			c[i]. s1 = false;
		else if ( !c[i]. s0 && friends == 3 )
			c[i]. s1 = true;
	}

	for(int i = 0; i < xsize*ysize; i++)
		c[i]. s0 = c[i]. s1;
}


int grid::neighbours(int i)
{
	int count = 0;
	
	//up 

	if( (i-int(xsize)) >= 0 && c[i - xsize]. s0 ) // need to cast xsize to int 'cause of some g++ errors
		count ++;
	if( (i-int(xsize)-1) >= 0 && c[i - xsize - 1]. s0 )
		count ++;
	if( (i-int(xsize)+1) >= 0 && c[i - xsize + 1]. s0 )
		count ++;

	// mid
	if ( i+1 < xsize*ysize && c[i+1]. s0 )
		count++;
	if ( -1< i-1 && c[i-1]. s0 )
		count++;	
	//down
	if ( i+xsize-1 < xsize*ysize && c[i+xsize-1]. s0 )
		count++;
	if ( i+xsize < xsize*ysize && c[i+xsize]. s0 )
		count++;
	if ( i+xsize+1 < xsize*ysize && c[i+xsize+1]. s0 )
		count++;


	return count; //all 'if' statements using comparison could/should be replaced with some assert

}

// display

std::ostream& operator << ( std::ostream& stream, const grid& g)
{
	for( int i = 0; i <= g. xsize * g. ysize; i++ )
	{
		if(g. c[i]. s0 )
			stream << "0";
		else
			stream << ".";

		if( i % g. xsize == 0 && i != 0 ) 
			stream << "\n"; 	
	}

	return stream;
}
std::ostream& operator << ( std::ostream& stream, const grid::wrongchar& w )
{
	return stream;
}


void grid::plot( ) const
{
  glColor3f( 0.6, 0.7, 1 );
	for( int i = 0; i <= xsize *  ysize; i++ )
	{
		if( c[i]. s0 )
		{
			int row = i / xsize;
			int col = i % ysize;
			glRecti( -(xsize/2)+(GRID_SCALE*col), (ysize/2 - GRID_SCALE )-(GRID_SCALE*row), 
							 -(xsize/2 + GRID_SCALE )+(GRID_SCALE*col), (ysize/2)-(GRID_SCALE*row));
		}


	}

  glColor3f( 0.0, 0.7, 0 );
	for(int i = 0; i < 250; i++ )
	{
  	glBegin(GL_LINES);
			glVertex2i(-(xsize/2)+(i*2),-(ysize/2));
			glVertex2i(-(xsize/2)+(i*2), ysize/2);
		glEnd();	

		glBegin(GL_LINES);
			glVertex2i(-(xsize/2),-(ysize/2)+(i*2));
      glVertex2i((xsize/2),-(ysize/2)+(i*2));
		glEnd();			

	}
	
}

// maintenance

void grid::clear( )
{
	for(int i = 0; i < xsize * ysize; i ++)
		c[i]. s0 = false;
}	

