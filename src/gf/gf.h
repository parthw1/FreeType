/****************************************************************************
 *
 * gf.h
 *
 *   FreeType font driver for TeX's GF FONT files
 *
 * Copyright (C) 1996-2019 by
 * Parth Wazurkar, David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ifndef GF_H_
#define GF_H_


#include <ft2build.h>
#include FT_INTERNAL_OBJECTS_H
#include FT_INTERNAL_STREAM_H


FT_BEGIN_HEADER

  typedef struct  GF_PostambleRec_
  {
    FT_Long   ptr_p;
    FT_Long   ds;    /* design_size */
    FT_Long   cs;    /* check_sum */
    FT_Long   hppp;
    FT_Long   vppp;
    FT_Long   min_m;
    FT_Long   max_m;
    FT_Long   min_n;
    FT_Long   max_n;

  } GF_PostambleRec;


  typedef struct  GF_BocRec_
  {
    FT_Long   char_code;
    FT_Long   prev_char;
    FT_Long   min_m;
    FT_Long   max_m;
    FT_Long   min_n;
    FT_Long   max_n;

  } GF_BocRec;


  typedef struct  GF_Boc1Rec_
  {
    FT_Byte   char_code;
    FT_Byte   del_m;
    FT_Byte   max_m;
    FT_Byte   del_n;
    FT_Byte   max_n;

  } GF_Boc1Rec;


  typedef struct  GF_CharLocRec_
  {
    FT_Byte   char_residue;
    FT_Long   dx;           /* character escapements dx and dy*/
    FT_Long   dy;
    FT_Long   w;            /* character width */
    FT_Long   ptr;          /* ptr to begining of character */

  } GF_CharLocRec;


  typedef struct  GF_CharLoc0Rec_   /* Same as char_loc, except dy=0 and dx=65536*dm */
  {
    FT_Byte   char_residue;
    FT_Byte   dm;                   /* 0 <= dm < 256 */
    FT_Long   w;
    FT_Long   ptr;

  } GF_CharLoc0Rec;


#define  GF_PAINT_0        0
#define  GF_PAINT_1        1
#define  GF_PAINT_63      63
#define  GF_PAINT1        64
#define  GF_PAINT2        65
#define  GF_PAINT3        66
#define  GF_BOC           67
#define  GF_BOC1          68
#define  GF_EOC           69
#define  GF_SKIP0         70
#define  GF_SKIP1         71
#define  GF_SKIP2         72
#define  GF_SKIP3         73
#define  GF_NEW_ROW_0     74
#define  GF_NEW_ROW_164  238 
#define  GF_XXX1         239
#define  GF_XXX2         240
#define  GF_XXX3         241
#define  GF_XXX4         242
#define  GF_YYY          243
#define  GF_NO_OP        244
#define  GF_CHAR_LOC     245
#define  GF_CHAR_LOC0    246
#define  GF_PRE          247
#define  GF_POST         248
#define  GF_POST_POST    249
#define  GF_ID           131

/* Commands 250-255 are undefined at present time (gftype) */


FT_END_HEADER


#endif /* GF_H_ */


/* END */
