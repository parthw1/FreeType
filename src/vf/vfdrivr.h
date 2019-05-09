/****************************************************************************
 *
 * vfdrivr.h
 *
 *   FreeType font driver for TeX's VF FONT files.
 *
 * Copyright 1996-2018 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ifndef VFDRIVR_H_
#define VFDRIVR_H_

#include <ft2build.h>
#include FT_INTERNAL_DRIVER_H

#include "vf.h"


FT_BEGIN_HEADER

  typedef struct VF_BitmapRec_
  {
    FT_UInt              bbx_width, bbx_height;
    FT_UInt              off_x, off_y;
    FT_UInt              mv_x,  mv_y;
    FT_Byte              *bitmap;
    FT_UInt              raster;

  } VF_BitmapRec, *VF_Bitmap;

  typedef struct  VF_FaceRec_
  {
    FT_FaceRec        root;
    /* TO-DO */
  } VF_FaceRec, *VF_Face;


  FT_EXPORT_VAR( const FT_Driver_ClassRec )  vf_driver_class;


FT_END_HEADER


#endif /* VFDRIVR_H_ */


/* END */
