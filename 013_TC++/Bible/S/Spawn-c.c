/*                                */
/*    Program Name : spawn-c.c    */
/*                                */
#include <process.h>
#include <stdio.h>

void main( int argc, char *argv[], char *envp[] )
{
   int i;

   /*  Check the numbers of arguments.  */
   if( argc < 3 ) {
      printf( "Not enough arguments!\a\n" );
      abort();
   }

   printf( "\n--------------------\n" );
   printf( "Run Child process!  " );
   printf( "Called by \"%s\" function.\n", argv[1] );
   printf( "There are %d arguments :\n", argc );

   /*  Print the arguments.  */
   for( i = 0; i < argc; i++ )
      printf( "  Argument %1d is \"%s\".\n", i, argv[i] );

   /*  Print the environment of the child process.  */
   printf( "The environment of child process are :\n" );
   for( i = 0; envp[i] != ( char * )NULL; i++ )
      printf( "  %s\n", envp[i] );
}
