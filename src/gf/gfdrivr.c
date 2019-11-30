/****************************************************************************
 *
 * gfdrivr.c
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
#include <ft2build.h>

#include FT_INTERNAL_DEBUG_H
#include FT_INTERNAL_STREAM_H
#include FT_INTERNAL_OBJECTS_H

#include "gf.h"
#include "gfdrivr.h"

#include "gferror.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  gfdriver


  static const FT_Frame_Field  gf_postamble_fields[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  GF_PostambleRec

    FT_FRAME_START( 36 ),
      FT_FRAME_LONG ( ptr_p ),
      FT_FRAME_LONG ( ds ),
      FT_FRAME_LONG ( cs ),
      FT_FRAME_LONG ( hppp ),
      FT_FRAME_LONG ( vppp ),
      FT_FRAME_LONG ( min_m ),
      FT_FRAME_LONG ( max_m ),
      FT_FRAME_LONG ( min_n ),
      FT_FRAME_LONG ( max_n ),
    FT_FRAME_END
  };


  static const FT_Frame_Field  gf_bocrec_fields[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  GF_BocRec

    FT_FRAME_START( 24 ),
      FT_FRAME_LONG ( char_code ),
      FT_FRAME_LONG ( prev_char ),
      FT_FRAME_LONG ( min_m ),
      FT_FRAME_LONG ( max_m ),
      FT_FRAME_LONG ( min_n ),
      FT_FRAME_LONG ( max_n ),
    FT_FRAME_END
  };


  static const FT_Frame_Field  gf_boc1rec_fields[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  GF_Boc1Rec

    FT_FRAME_START( 5 ),
      FT_FRAME_BYTE  ( char_code ),
      FT_FRAME_BYTE  ( del_m ),
      FT_FRAME_BYTE  ( max_m ),
      FT_FRAME_BYTE  ( del_n ),
      FT_FRAME_BYTE  ( max_n ),
    FT_FRAME_END
  };


  static const FT_Frame_Field  gf_charloc_fields[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  GF_CharLocRec

    FT_FRAME_START( 17 ),
      FT_FRAME_BYTE  ( char_residue ),
      FT_FRAME_LONG  ( dx ),
      FT_FRAME_LONG  ( dy ),
      FT_FRAME_LONG  ( w ),
      FT_FRAME_LONG  ( ptr),
    FT_FRAME_END
  };


  static const FT_Frame_Field  gf_charloc0_fields[] =
  {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  GF_CharLoc0Rec

    FT_FRAME_START( 10 ),
      FT_FRAME_BYTE  ( char_residue ),
      FT_FRAME_BYTE  ( dm ),
      FT_FRAME_LONG  ( w ),
      FT_FRAME_LONG  ( ptr ),
    FT_FRAME_END
  };

  typedef struct  GF_CMapRec_
  {
    FT_CMapRec  cmap;
    /*TO-DO*/

  } GF_CMapRec, *GF_CMap;


  FT_CALLBACK_DEF( FT_Error )
  gf_cmap_init  ( FT_CMap     gcmap,
                 FT_Pointer  init_data )
  {
    FT_Error error = FT_Err_Ok;
    /* TO-DO */

    return error;
  }


  FT_CALLBACK_DEF( void )
  gf_cmap_done( FT_CMap  gfcmap )
  {
    /* TO-DO */

    return;
  }


  FT_CALLBACK_DEF( void )
  GF_Face_Done( FT_Face  gfface )              /* GF_Face */
  {
    FT_Error error = FT_Err_Ok;
    /* TO-DO */

    return ;
  }


  FT_CALLBACK_DEF( FT_Error )
  GF_Face_Init( FT_Stream       stream,
                 FT_Face        gfface,        /* GF_Face */
                 FT_Int         face_index,
                 FT_Int         num_params,
                 FT_Parameter*  params )
  {
    FT_Error       error  = FT_Err_Ok;
    GF_Face        face   = (GF_Face)gfface;
    FT_Memory      memory = FT_FACE_MEMORY( face );

    FT_UNUSED( num_params );
    FT_UNUSED( params );

    FT_TRACE2(( "GF driver\n" ));

    return error;
  }


  FT_CALLBACK_DEF( FT_Error )
  GF_Size_Select(  FT_Size   size,
                   FT_ULong  strike_index )
  {
    FT_Error       error  = FT_Err_Ok;
    /* TO-DO*/

    return error;

  }

  FT_CALLBACK_DEF( FT_Error )
  GF_Size_Request( FT_Size          size,
                    FT_Size_Request  req )
  {
    FT_Error error = FT_Err_Ok;
    /* TO-DO */

    return error;
  }



  FT_CALLBACK_DEF( FT_Error )
  GF_Glyph_Load( FT_GlyphSlot  slot,
                  FT_Size       size,
                  FT_UInt       glyph_index,
                  FT_Int32      load_flags )
  {
    FT_Error error = FT_Err_Ok;
    /* TO-DO */

    return error;
  }


  FT_CALLBACK_TABLE_DEF
  const FT_Driver_ClassRec  gf_driver_class =
  {
    {
      FT_MODULE_FONT_DRIVER         |
      FT_MODULE_DRIVER_NO_OUTLINES,
      sizeof ( FT_DriverRec ),

      "gf",
      0x10000L,
      0x20000L,

      NULL,    									/* module-specific interface */

      NULL,                     /* FT_Module_Constructor  module_init   */
      NULL,                     /* FT_Module_Destructor   module_done   */
      NULL      								/* FT_Module_Requester    get_interface */
    },

    sizeof ( GF_FaceRec ),
    sizeof ( FT_SizeRec ),
    sizeof ( FT_GlyphSlotRec ),

    GF_Face_Init,               /* FT_Face_InitFunc  init_face */
    GF_Face_Done,               /* FT_Face_DoneFunc  done_face */
    NULL,                       /* FT_Size_InitFunc  init_size */
    NULL,                       /* FT_Size_DoneFunc  done_size */
    NULL,                       /* FT_Slot_InitFunc  init_slot */
    NULL,                       /* FT_Slot_DoneFunc  done_slot */

    GF_Glyph_Load,              /* FT_Slot_LoadFunc  load_glyph */

    NULL,                       /* FT_Face_GetKerningFunc   get_kerning  */
    NULL,                       /* FT_Face_AttachFunc       attach_file  */
    NULL,                       /* FT_Face_GetAdvancesFunc  get_advances */

    GF_Size_Request,           /* FT_Size_RequestFunc  request_size */
    GF_Size_Select             /* FT_Size_SelectFunc   select_size  */
  };


/* END */
