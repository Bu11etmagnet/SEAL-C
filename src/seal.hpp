/************************************************
 SEAL: implemented in C
 See LICENSE

 Functions for handling the parameters data structure.

 C doesn't have dynamic variables, so use these instead.
 For languages with local/global dynamic variables for
 hashes (named arrays; e.g., PHP, JavaScript)
 then these are just a named array indexes.
 ************************************************/
#ifndef SEAL_HPP
#define SEAL_HPP

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

// Revise the version if there is any significant change
#define SEAL_VERSION "0.0.1-alpha"

extern int Verbose;

// Common data types
typedef unsigned char byte;
struct sealfield
  {
  /*****
   Data type for debugging
   Set by the SealSet* functions.
   'c' for char
   'b' for binary
   '4' for uint32
   '8' for uint64
   'I' for integer (size_t)
   *****/
  char Type;

  /*****
   Data structure for storing field=value sets.
   *****/
  char *Field;
  byte *Value;
  /*****
   Why uint32_t for lengths?
   Most fields are expected to be smaller than 256.
   Signatures could be 1024-2048 if the algorithm is big enough.
   Comments and custom fields? Those could be huge...
   *****/
  size_t FieldLen; // length of field. e.g., "b" would be 1
  size_t ValueLen; // length of field. e.g., "-s,s-" would be 5
  struct sealfield *Next;
  };
typedef struct sealfield sealfield;

// Macros and code for debugging
#define WHERESTR  "DEBUG[%s:%d]"
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__)
#define DEBUGPRINT(_fmt, ...)  DEBUGPRINT2(WHERESTR ": " _fmt "\n", WHEREARG, __VA_ARGS__)
#define DEBUGWHERE()  DEBUGPRINT2(WHERESTR "\n", WHEREARG)
#define DEBUGWALK(x,y) { DEBUGPRINT2(WHERESTR ": WALK: %s\n", WHEREARG, x); SealWalk(y); }
void	DEBUGhexdump	(size_t DataLen, const byte *Data);

// Common macros
#define Min(x,y)  ( ((x) < (y)) ? (x) : (y) )

// SEAL structure functions
sealfield *	SealClone	(sealfield *src);

void	SealFree	(sealfield *vf);
void	SealWalk	(sealfield *vf);
int	SealCmp	(sealfield *vfhead, const char *Field1, const char *Field2);
int	SealCmp2	(sealfield *vfhead1, const char *Field1, sealfield *vfhead2, const char *Field2);
sealfield *	SealDel		(sealfield *vf, const char *Field);
sealfield *	SealAlloc	(sealfield *vfhead, const char *Field, size_t Len, const char Type);
sealfield *	SealAllocU32	(sealfield *vfhead, const char *Field, size_t Num);
sealfield *	SealAllocU64	(sealfield *vfhead, const char *Field, size_t Num);
sealfield *	SealAllocI	(sealfield *vfhead, const char *Field, size_t Num);
size_t	SealGetSize	(sealfield *vfhead, const char *Field); // value length in bytes
sealfield *	SealSearch	(sealfield *vf, const char *Field);
sealfield *	SealCopy	(sealfield *vfhead, const char *NewField, const char *OldField);
sealfield *	SealCopy2	(sealfield *vfhead2, const char *Field2, sealfield *vfhead1, const char *Field1);
sealfield *	SealMove	(sealfield *vfhead, const char *NewField, const char *OldField);
sealfield *	SealParmCheck	(sealfield *Args);

// Binary data
byte *	SealGetBin	(sealfield *vfhead, const char *Field);
sealfield *	SealSetBin	(sealfield *vfhead, const char *Field, size_t ValueLen, const byte *Value);
sealfield *	SealAddBin	(sealfield *vfhead, const char *Field, size_t ValueLen, const byte *Value);

// Text data
char *	SealGetText	(sealfield *vfhead, const char *Field);
sealfield *	SealSetText	(sealfield *vfhead, const char *Field, const char *Value);
sealfield *	SealSetTextLen	(sealfield *vfhead, const char *Field, size_t ValueLen, const char *Value);
sealfield *	SealAddText	(sealfield *vfhead, const char *Field, const char *Value);
sealfield *	SealAddTextLen	(sealfield *vfhead, const char *Field, size_t ValueLen, const char *Value);
sealfield *	SealAddTextPad	(sealfield *vfhead, const char *Field, size_t PadLen, const char *Padding);

// Generic index
byte *	SealGetGarray	(sealfield *vfhead, const char *Field);
byte *	SealGetGindex	(sealfield *vfhead, const char *Field, size_t Size, int Index);
sealfield *	SealSetGindex	(sealfield *vfhead, const char *Field, char Type, size_t Size, int Index, const void *Value);

// Cast generic to specific
#define SealGetCarray(a,f)	((char*)SealGetGarray(a,f))
#define SealGetU32array(a,f)	((uint32_t*)SealGetGarray(a,f))
#define SealGetU64array(a,f)	((uint64_t*)SealGetGarray(a,f))
#define SealGetIarray(a,f)	((size_t*)SealGetGarray(a,f))

// Character data
char	SealGetCindex	(sealfield *vfhead, const char *Field, int Index);
sealfield *	SealSetCindex	(sealfield *vfhead, const char *Field, int Index, const char C);
sealfield *	SealAddC	(sealfield *vfhead, const char *Field, const char C);

// Uint32 data (as an array)
uint32_t	SealGetU32index	(sealfield *vfhead, const char *Field, int Index);
sealfield *	SealSetU32index	(sealfield *vfhead, const char *Field, int Index, uint32_t Value);

// Uint64 data (as an array)
uint64_t	SealGetU64index	(sealfield *vfhead, const char *Field, int Index);
sealfield *	SealSetU64index	(sealfield *vfhead, const char *Field, int Index, uint64_t Value);

// size_t data (as an array); this is an int that matches the max memory for the machine
size_t		SealGetIindex	(sealfield *vfhead, const char *Field, int Index);
sealfield *	SealSetIindex	(sealfield *vfhead, const char *Field, int Index, size_t Value);

// Comparison

#endif