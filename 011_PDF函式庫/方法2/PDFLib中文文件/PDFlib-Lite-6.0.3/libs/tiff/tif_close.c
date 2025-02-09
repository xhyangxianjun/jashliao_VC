/* PDFlib GmbH cvsid:
 * $Id: tif_close.c,v 1.9 2004/03/26 18:36:54 rjs Exp $ */

/*
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * TIFF Library.
 */
#include "tiffiop.h"

void
TIFFClose(TIFF* tif)
{
#ifdef PDFLIB_TIFFWRITE_SUPPORT
    if (tif->tif_mode != O_RDONLY)
        /*
         * Flush buffered data and directory (if dirty).
         */
        TIFFFlush(tif);
#endif /* PDFLIB_TIFFWRITE_SUPPORT */
    (*tif->tif_cleanup)(tif);
    TIFFFreeDirectory(tif);

    if (tif->tif_dirlist)
        _TIFFfree(tif, tif->tif_dirlist);
        
    /* Clean up client info links */
    while( tif->tif_clientinfo )
    {
        TIFFClientInfoLink *ilink = tif->tif_clientinfo;

        tif->tif_clientinfo = ilink->next;
        _TIFFfree(tif, ilink->name );
        _TIFFfree(tif, ilink );
    }

    if (tif->tif_rawdata && (tif->tif_flags&TIFF_MYBUFFER))
        _TIFFfree(tif, tif->tif_rawdata);
    if (isMapped(tif))
        TIFFUnmapFileContents(tif, tif->tif_base, tif->tif_size);
    (void) TIFFCloseFile(tif);
    if (tif->tif_nfields > 0) 
    {
        int  i;

        for (i = 0; i < tif->tif_nfields; i++) 
	{
	    TIFFFieldInfo *fld = tif->tif_fieldinfo[i];
 	    if (fld->field_bit == FIELD_CUSTOM && 
		strncmp("Tag ", fld->field_name, 4) == 0) 
	    {
                _TIFFfree(tif, fld->field_name);
                _TIFFfree(tif, fld);
	    }
        }   
      
        _TIFFfree(tif, tif->tif_fieldinfo);
    }

    _TIFFfree(tif, tif);
}
