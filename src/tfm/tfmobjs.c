/****************************************************************************
 *
 * tfmobjs.c
 *
 *   FreeType auxiliary TFM module.
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


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_INTERNAL_STREAM_H
#include FT_INTERNAL_DEBUG_H
#include FT_INTERNAL_TFM_H

#include "tfmobjs.h"
#include "tfmmod.h"
#include "tfmerr.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  tfmobjs


 /**************************************************************************
   *
   * TFM font utility functions.
   *
   */

  FT_Long   tfm_read_intn  ( FT_Stream, FT_Int );
  FT_ULong  tfm_read_uintn ( FT_Stream, FT_Int );

#define READ_UINT2( stream )    (FT_Byte) tfm_read_uintn( stream, 2)
#define READ_UINT4( stream )    (FT_Byte) tfm_read_uintn( stream, 4)
#define READ_INT4( stream )     (FT_Long) tfm_read_intn ( stream, 4)

/*
 * Reading a Number from file
 */

  FT_ULong
  tfm_read_uintn( FT_Stream stream,
		  FT_Int    size )
  {
    FT_ULong  v,k;
    FT_Error  error = FT_Err_Ok;
    FT_Byte   tp;
    v = 0L;
    while (size >= 1)
    {
      if ( FT_READ_BYTE(tp) )
        return 0; /* To be changed */
      k =(FT_ULong) tp;
      v = v*256L + k;
      --size;
    }
    return v;
  }

  FT_Long
  tfm_read_intn( FT_Stream stream,
		 FT_Int    size)
  {
    FT_Long  v;
    FT_Byte  tp;
    FT_Error error= FT_Err_Ok;
    FT_ULong z ;
    if ( FT_READ_BYTE(tp) )
        return 0; /* To be changed */
    z= (FT_ULong) tp;
    v = (FT_Long)z & 0xffL;
    if (v & 0x80L)
      v = v - 256L;
    --size;
    while (size >= 1)
    {
      if ( FT_READ_BYTE(tp) )
        return 0; /* To be changed */
      z= (FT_ULong) tp;
      v = v*256L + z;
      --size;
		}
    return v;
  }

  /**************************************************************************
   *
   * API.
   *
   */

  FT_LOCAL_DEF( FT_Error )
  tfm_init( TFM_Parser  parser,
            FT_Memory   memory,
            FT_Stream   stream )
  {
    FT_Error    error;

    parser->memory    = memory;
    parser->stream    = stream;
    parser->FontInfo  = NULL;
    parser->user_data = NULL;

    return FT_Err_Ok;
  }


  FT_LOCAL( void )
  tfm_close( TFM_Parser  parser )
  {
    FT_Memory  memory = parser->memory;

    FT_FREE( parser->stream );
  }


  FT_LOCAL_DEF( FT_Error )
  tfm_parse_metrics( TFM_Parser  parser )
  {
    FT_Memory     memory = parser->memory;
    TFM_FontInfo  fi     = parser->FontInfo;
    FT_Error      error  = FT_ERR( Syntax_Error );

    FT_ULong      lf, lh, nc, nci, err;
    FT_ULong      offset_header, offset_char_info, offset_param;
    FT_ULong      nw,  nh,  nd,  ni, nl, nk, neng, np, dir;

    FT_Long       *w,  *h,  *d;
    FT_ULong      *ci, v;

    FT_ULong      i;
    FT_Long       bbxw, bbxh, xoff, yoff;

    if ( !fi )
      return FT_THROW( Invalid_Argument );

    fi->width  = NULL;
    fi->height = NULL;
    fi->depth  = NULL;

    fi->font_bbx_w = 0.0;
    fi->font_bbx_h = 0.0;
    fi->font_bbx_xoff = 0.0;
    fi->font_bbx_yoff = 0.0;

    err = 0;

    if( FT_STREAM_SEEK( 0 ) )
      return error;

    lf = (FT_ULong)READ_UINT2( stream );
    
    fi->fs = 4*lf - 1 ;
 
    /* Traditional TeX Metric File */
    lh               = (int)READ_UINT2( stream );
    offset_header    = 4*6;
    offset_char_info = 4*(6+lh);

    fi->begin_char  = (FT_Int) READ_UINT2( stream );
    fi->end_char    = (FT_Int) READ_UINT2( stream );

    nw   = (FT_ULong) READ_UINT2( stream );
    nh   = (FT_ULong) READ_UINT2( stream );
    nd   = (FT_ULong) READ_UINT2( stream );
 
    ni   = (FT_ULong) READ_UINT2( stream );
    nl   = (FT_ULong) READ_UINT2( stream );
    nk   = (FT_ULong) READ_UINT2( stream );
    neng = (FT_ULong) READ_UINT2( stream );
    np   = (FT_ULong) READ_UINT2( stream );
 
    if (((signed)(fi->begin_char-1) > (signed)fi->end_char) ||
       (fi->end_char > 255))
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    if (FT_STREAM_SEEK( offset_header ) )
      goto Exit;
    fi->cs          = READ_UINT4( stream );
    fi->ds          = READ_UINT4( stream );
    fi->design_size = (double)(fi->ds)/(double)(1<<20);

    nc  = fi->end_char - fi->begin_char + 1;
    nci = nc;
    
    ci = (FT_ULong*)calloc(nci, sizeof(FT_ULong));
    w  = (FT_Long*)calloc(nw,  sizeof(FT_ULong));
    h  = (FT_Long*)calloc(nh,  sizeof(FT_ULong));
    d  = (FT_Long*)calloc(nd,  sizeof(FT_ULong));
 
    if ((ci == NULL) || (w == NULL) || (h == NULL) || (d == NULL))
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    if( FT_STREAM_SEEK( offset_char_info ) )
      goto Exit;

    for (i = 0; i < nci; i++)
      ci[i] = READ_UINT4( stream );

    offset_param = stream->pos + 4*(nw + nh + nd + ni + nl + nk + neng);

    for (i = 0; i < nw; i++)
      w[i] = READ_INT4( stream );
    for (i = 0; i < nh; i++)
      h[i] = READ_INT4( stream );
    for (i = 0; i < nd; i++)
      d[i] = READ_INT4( stream );

    fi->width  = (FT_Long*)calloc(nc, sizeof(FT_Long));
    fi->height = (FT_Long*)calloc(nc, sizeof(FT_Long));
    fi->depth  = (FT_Long*)calloc(nc, sizeof(FT_Long));
    
    if ((fi->width == NULL) || (fi->height == NULL) || (fi->depth == NULL))
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }
    bbxw = 0;
    bbxh = 0;
    xoff = 0;
    yoff = 0;

    for (i = 0; i < nc; i++)
    {
      v = ci[i] / 0x10000L;
      fi->depth[i]  = d[v & 0xf];  v >>= 4;
      fi->height[i] = h[v & 0xf];  v >>= 4;
      fi->width[i]  = w[v & 0xff];
      
      if (bbxw < fi->width[i])
	      bbxw = fi->width[i];
      
      if (bbxh < (fi->height[i] + fi->depth[i]))
	      bbxh = fi->height[i] + fi->depth[i];
      
      if (yoff > -fi->depth[i])
	      yoff = -fi->depth[i];
    }
    
    fi->font_bbx_w = (FT_ULong)(fi->design_size * ( bbxw / (1<<20)));
    fi->font_bbx_h = (FT_ULong)(fi->design_size * ( bbxh / (1<<20)));
    fi->font_bbx_xoff = (FT_ULong)(fi->design_size * ( xoff / (1<<20)));
    fi->font_bbx_yoff = (FT_ULong)(fi->design_size * ( yoff / (1<<20)));

    #if 0
    if (tfm->type == METRIC_TYPE_JFM)
    {
      fseek(fp, 4*(7+lh), SEEK_SET);
      tfm->ct_kcode = (unsigned int*)calloc(tfm->nt+1, sizeof(unsigned int));
      tfm->ct_ctype = (unsigned int*)calloc(tfm->nt+1, sizeof(unsigned int));
      if ((tfm->ct_kcode == NULL) || (tfm->ct_ctype == NULL))
      {
        error = FT_THROW( Invalid_Argument );
        goto Exit;
      }
      for (i = 0; i < tfm->nt; i++)
      {
        v = READ_UINT4(fp);
        tfm->ct_kcode[i] = v/0x10000L;
        tfm->ct_ctype[i] = v%0x10000L;
      }
      tfm->ct_kcode[tfm->nt] = 0; /* sentinel */
      tfm->ct_ctype[tfm->nt] = 0;
    }
    #endif

    /* fseek(fp, offset_param, SEEK_SET); */
    if( FT_STREAM_SEEK( offset_param ) )
      return error; /* To be changed */
    
    if (FT_READ_ULONG(fi->slant) )
      return error;
    
    fi->slant = (FT_ULong)(fi->slant/(1<<20));
 
  Exit:
    if( !ci || !w || !h || !d )
    {
      FT_FREE(ci);
      FT_FREE(w);
      FT_FREE(h);
      FT_FREE(d);
    }
    return error;
  }


/* END */
