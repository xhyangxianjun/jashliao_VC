#!/usr/bin/python
# $Id: pdfclock.py,v 1.12.2.1 2004/07/08 11:37:48 rjs Exp $
#
# PDFlib client: pdfclock example in Python
#

from sys import *
from time import *
from pdflib_py import *

RADIUS = 200.0
MARGIN = 20.0

p = PDF_new()

if PDF_begin_document(p, "pdfclock.pdf", "") == -1:
    print "Error: " + PDF_get_errmsg(p) + "\n"
    exit(2)

# This line is required to avoid problems on Japanese systems
PDF_set_parameter(p, "hypertextencoding", "winansi")

PDF_set_info(p, "Creator", "pdfclock.py")
PDF_set_info(p, "Author", "Thomas Merz")
PDF_set_info(p, "Title", "PDF clock (Python)")

PDF_begin_page(p, 2 * (RADIUS + MARGIN), 2 * (RADIUS + MARGIN))

PDF_translate(p, RADIUS + MARGIN, RADIUS + MARGIN)
PDF_setcolor(p, "fillstroke", "rgb", 0.0, 0.0, 1.0, 0.0)
PDF_save(p)

# minute strokes 
PDF_setlinewidth(p, 2.0)
for alpha in range(0, 360, 6):
    PDF_rotate(p, 6.0)
    PDF_moveto(p, RADIUS, 0.0)
    PDF_lineto(p, RADIUS-MARGIN/3, 0.0)
    PDF_stroke(p)

PDF_restore(p)
PDF_save(p)

# 5 minute strokes
PDF_setlinewidth(p, 3.0)
for alpha in range(0, 360, 30):
    PDF_rotate(p, 30.0)
    PDF_moveto(p, RADIUS, 0.0)
    PDF_lineto(p, RADIUS-MARGIN, 0.0)
    PDF_stroke(p)

(tm_year, tm_month, tm_day,
tm_hour, tm_min, tm_sec, 
tm_weekday, tm_julian, tm_ds) = localtime(time())

# draw hour hand 
PDF_save(p)
PDF_rotate(p, (-((tm_min/60.0) + tm_hour - 3.0) * 30.0))
PDF_moveto(p, -RADIUS/10, -RADIUS/20)
PDF_lineto(p, RADIUS/2, 0.0)
PDF_lineto(p, -RADIUS/10, RADIUS/20)
PDF_closepath(p)
PDF_fill(p)
PDF_restore(p)

# draw minute hand
PDF_save(p)
PDF_rotate(p, (-((tm_sec/60.0) + tm_min - 15.0) * 6.0))
PDF_moveto(p, -RADIUS/10, -RADIUS/20)
PDF_lineto(p, RADIUS * 0.8, 0.0)
PDF_lineto(p, -RADIUS/10, RADIUS/20)
PDF_closepath(p)
PDF_fill(p)
PDF_restore(p)

# draw second hand
PDF_setcolor(p, "fillstroke", "rgb", 1.0, 0.0, 0.0, 0.0)
PDF_setlinewidth(p, 2)
PDF_save(p)
PDF_rotate(p, -((tm_sec - 15.0) * 6.0))
PDF_moveto(p, -RADIUS/5, 0.0)
PDF_lineto(p, RADIUS, 0.0)
PDF_stroke(p)
PDF_restore(p)

# draw little circle at center
PDF_circle(p, 0, 0, RADIUS/30)
PDF_fill(p)

PDF_restore(p)

PDF_end_page_ext(p, "")

PDF_end_document(p, "")

PDF_delete(p)
