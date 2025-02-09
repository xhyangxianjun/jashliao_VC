/* PDFlib GmbH cvsid:
 * $Id: tif_strip.c,v 1.5.2.2 2004/11/10 15:55:13 tm Exp $ */

/*
 * Copyright (c) 1991-1997 Sam Leffler
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
 *
 * Strip-organized Image Support Routines.
 */
#include "tiffiop.h"

static uint32
summarize(TIFF* tif, size_t summand1, size_t summand2, const char* where)
{
	uint32	bytes = summand1 + summand2;

	if (bytes - summand1 != summand2) {
		TIFFError(tif->tif_name, "Integer overflow in %s", where);
		bytes = 0;
	}

	return (bytes);
}

static uint32
multiply(TIFF* tif, size_t nmemb, size_t elem_size, const char* where)
{
	uint32	bytes = nmemb * elem_size;

	if (elem_size && bytes / elem_size != nmemb) {
		TIFFError(tif->tif_name, "Integer overflow in %s", where);
		bytes = 0;
	}

	return (bytes);
}

/*
 * Compute which strip a (row,sample) value is in.
 */
tstrip_t
TIFFComputeStrip(TIFF* tif, uint32 row, tsample_t sample)
{
	TIFFDirectory *td = &tif->tif_dir;
	tstrip_t strip;

	strip = row / td->td_rowsperstrip;
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE) {
		if (sample >= td->td_samplesperpixel) {
			TIFFError(tif->tif_name,
			    "%u: Sample out of range, max %u",
			    sample, td->td_samplesperpixel);
			return ((tstrip_t) 0);
		}
		strip += sample*td->td_stripsperimage;
	}
	return (strip);
}

/*
 * Compute how many strips are in an image.
 */
tstrip_t
TIFFNumberOfStrips(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	tstrip_t nstrips;

	nstrips = (td->td_rowsperstrip == (uint32) -1 ?
	     (td->td_imagelength != 0 ? 1 : 0) :
	     TIFFhowmany(td->td_imagelength, td->td_rowsperstrip));
	if (td->td_planarconfig == PLANARCONFIG_SEPARATE)
		nstrips = multiply(tif, nstrips, td->td_samplesperpixel,
				   "TIFFNumberOfStrips");
	return (nstrips);
}

/*
 * Compute the # bytes in a variable height, row-aligned strip.
 */
tsize_t
TIFFVStripSize(TIFF* tif, uint32 nrows)
{
	TIFFDirectory *td = &tif->tif_dir;

	if (nrows == (uint32) -1)
		nrows = td->td_imagelength;
#ifdef YCBCR_SUPPORT
	if (td->td_planarconfig == PLANARCONFIG_CONTIG &&
	    td->td_photometric == PHOTOMETRIC_YCBCR &&
	    !isUpSampled(tif)) {
		/*
		 * Packed YCbCr data contain one Cb+Cr for every
		 * HorizontalSampling*VerticalSampling Y values.
		 * Must also roundup width and height when calculating
		 * since images that are not a multiple of the
		 * horizontal/vertical subsampling area include
		 * YCbCr data for the extended image.
		 */
                uint16 ycbcrsubsampling[2];
                tsize_t w, scanline, samplingarea;

                TIFFGetField( tif, TIFFTAG_YCBCRSUBSAMPLING, 
                              ycbcrsubsampling + 0, 
                              ycbcrsubsampling + 1 );
		/* make sure we dont get division by 0 due to bad tiffs */
		if (!ycbcrsubsampling[0]) ycbcrsubsampling[0] = 1;
		if (!ycbcrsubsampling[1]) ycbcrsubsampling[1] = 1;

		w = TIFFroundup(td->td_imagewidth, ycbcrsubsampling[0]);
		scanline = TIFFhowmany8(multiply(tif, w, td->td_bitspersample,
						 "TIFFVStripSize"));
		samplingarea = ycbcrsubsampling[0]*ycbcrsubsampling[1];
		nrows = TIFFroundup(nrows, ycbcrsubsampling[1]);
		/* NB: don't need TIFFhowmany here 'cuz everything is rounded */
		scanline = multiply(tif, nrows, scanline, "TIFFVStripSize");
		return ((tsize_t)
		    summarize(tif, scanline,
			      multiply(tif, 2, scanline / samplingarea,
				       "TIFFVStripSize"), "TIFFVStripSize"));
	} else
#endif
		return ((tsize_t) multiply(tif, nrows, TIFFScanlineSize(tif),
					   "TIFFVStripSize"));
}


/*
 * Compute the # bytes in a raw strip.
 */
tsize_t
TIFFRawStripSize(TIFF* tif, tstrip_t strip)
{
	TIFFDirectory* td = &tif->tif_dir;
	tsize_t bytecount = td->td_stripbytecount[strip];

	if (bytecount <= 0) {
		TIFFError(tif->tif_name,
			  "%lu: Invalid strip byte count, strip %lu",
			  (tif_long) bytecount, (tif_long) strip);
		bytecount = (tsize_t) -1;
	}

	return bytecount;
}

/*
 * Compute the # bytes in a (row-aligned) strip.
 *
 * Note that if RowsPerStrip is larger than the
 * recorded ImageLength, then the strip size is
 * truncated to reflect the actual space required
 * to hold the strip.
 */
tsize_t
TIFFStripSize(TIFF* tif)
{
	TIFFDirectory* td = &tif->tif_dir;
	uint32 rps = td->td_rowsperstrip;
	if (rps > td->td_imagelength)
		rps = td->td_imagelength;
	return (TIFFVStripSize(tif, rps));
}

/*
 * Compute a default strip size based on the image
 * characteristics and a requested value.  If the
 * request is <1 then we choose a strip size according
 * to certain heuristics.
 */
uint32
TIFFDefaultStripSize(TIFF* tif, uint32 request)
{
	return (*tif->tif_defstripsize)(tif, request);
}

uint32
_TIFFDefaultStripSize(TIFF* tif, uint32 s)
{
	if ((int32) s < 1) {
		/*
		 * If RowsPerStrip is unspecified, try to break the
		 * image up into strips that are approximately 8Kbytes.
		 */
		tsize_t scanline = TIFFScanlineSize(tif);
		s = (uint32)(8*1024) / (scanline == 0 ? 1 : scanline);
		if (s == 0)		/* very wide images */
			s = 1;
	}
	return (s);
}

/*
 * Return the number of bytes to read/write in a call to
 * one of the scanline-oriented i/o routines.  Note that
 * this number may be 1/samples-per-pixel if data is
 * stored as separate planes.
 */
tsize_t
TIFFScanlineSize(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	tsize_t scanline;
	
	scanline = multiply (tif, td->td_bitspersample, td->td_imagewidth,
			     "TIFFScanlineSize");
	if (td->td_planarconfig == PLANARCONFIG_CONTIG)
	    scanline = multiply (tif, scanline, td->td_samplesperpixel,
				     "TIFFScanlineSize");
	return ((tsize_t) TIFFhowmany8(scanline));
}

/*
 * Return the number of bytes required to store a complete
 * decoded and packed raster scanline (as opposed to the
 * I/O size returned by TIFFScanlineSize which may be less
 * if data is store as separate planes).
 */
tsize_t
TIFFRasterScanlineSize(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	tsize_t scanline;
	
	scanline = multiply (tif, td->td_bitspersample, td->td_imagewidth,
			     "TIFFRasterScanlineSize");
	if (td->td_planarconfig == PLANARCONFIG_CONTIG) {
	    scanline = multiply (tif, scanline, td->td_samplesperpixel,
				 "TIFFRasterScanlineSize");
	    return ((tsize_t) TIFFhowmany8(scanline));
	} else
		return ((tsize_t) multiply (tif, TIFFhowmany8(scanline),
				    td->td_samplesperpixel,
				    "TIFFRasterScanlineSize"));
}
