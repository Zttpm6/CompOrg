#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/************************************************************/
/* Parsing the register with identifier and instruction    */
/************************************************************/
uint32_t getRegister( char * ins )
{

  if( strcmp( "$zero", ins ) == 0 )
  {
     return 0x00;
  }
  else if( strcmp( "$at", ins ) == 0 )
  {
    return 0x01;
  }
  else if( strcmp( "$v0", ins ) == 0 )
  {
    return 0x02;
  }
  else if( strcmp( "$v1", ins ) == 0 )
  {
    return 0x03;
  }
  else if( strcmp( "$a0", ins ) == 0 )
  {
    return 0x04;
  }
  else if( strcmp( "$a1", ins ) == 0 )
  {
    return 0x05;
  }
  else if( strcmp( "$a2", ins ) == 0 )
  {
    return 0x06;
  }
  else if( strcmp( "$a3", ins ) == 0 )
  {
    return 0x07;
  }
  else if( strcmp( "$t0", ins ) == 0 )
  {
    return 0x08;
  }
  else if( strcmp( "$t1", ins ) == 0 )
  {
    return 0x09;
  }
  else if( strcmp( "$t2", ins ) == 0 )
  {
    return 0x0A;
  }
  else if( strcmp( "$t3", ins ) == 0 )
  {
    return 0x0B;
  }
  else if( strcmp( "$t4", ins ) == 0 )
  {
    return 0x0C;
  }
  else if( strcmp( "$t5", ins ) == 0 )
  {
    return 0x0D;
  }
  else if( strcmp( "$t6", ins ) == 0 )
  {
    return 0x0E;
  }
  else if( strcmp( "$t7", ins ) == 0 )
  {
    return 0x0F;
  }
  else if( strcmp( "$s0", ins ) == 0 )
  {
    return 0x10;
  }
  else if( strcmp( "$s1", ins ) == 0 )
  {
    return 0x11;
  }
  else if( strcmp( "$s2", ins ) == 0 )
  {
    return 0x12;
  }
  else if( strcmp( "$s3", ins ) == 0 )
  {
    return 0x13;
  }
  else if( strcmp( "$s4", ins ) == 0 )
  {
    return 0x14;
  }
  else if( strcmp( "$s5", ins ) == 0 )
  {
    return 0x15;
  }
  else if( strcmp( "$s6", ins ) == 0 )
  {
    return 0x16;
  }
  else if( strcmp( "$s7", ins ) == 0 )
  {
    return 0x17;
  }
  else if( strcmp( "$t8", ins ) == 0 )
  {
    return 0x18;
  }
  else if( strcmp( "$t9", ins ) == 0 )
  {
    return 0x19;        
  }
  else if( strcmp( "$k0", ins ) == 0 )
  {
    return 0x1A;
  }
  else if( strcmp( "$k1", ins ) == 0 )
  {
    return 0x1B;
  }
  else if( strcmp( "$gp", ins ) == 0 )
  {
    return 0x1C;
  }
  else if( strcmp( "$sp", ins ) == 0 )
  {
    return 0x1D;
  }
  else if( strcmp( "$fp", ins ) == 0 )
  {
    return 0x1E;
  }
  else if( strcmp( "$ra", ins ) == 0 )
  {
    return 0x1F;
  }
  
  return 0;
}

/************************************************************/
/* Parsing the register with identifier and instruction    */
/************************************************************/
void getArg( char * ron, FILE * fp )
{                           
    fscanf( fp, "%s", ron );
    if( ron[ strlen(ron) - 1] == ',' )
    {
        ron[ strlen(ron) - 1] = '\0';
    }        
    return;                	
}     
