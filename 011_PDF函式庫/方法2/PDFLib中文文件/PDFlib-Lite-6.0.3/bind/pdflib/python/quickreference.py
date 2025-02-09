#!/usr/bin/python
# $Id: quickreference.py,v 1.11.2.1 2004/07/08 11:37:48 rjs Exp $
#
# PDFlib/PDI client: mini imposition demo
#

from sys import *
from pdflib_py import *

maxrow = 2
maxcol = 2
width = 500.0
height = 770.0

infile = "reference.pdf"
searchpath = "../data"

p = PDF_new()

if PDF_begin_document(p, "quickreference.pdf", "") == -1:
    print "Error: " + PDF_get_errmsg(p) + "\n"
    exit(2)

PDF_set_parameter(p, "SearchPath", searchpath)

# This line is required to avoid problems on Japanese systems
PDF_set_parameter(p, "hypertextencoding", "winansi")

PDF_set_info(p, "Creator", "quickreference.py")
PDF_set_info(p, "Author", "Thomas Merz")
PDF_set_info(p, "Title", "mini imposition demo (Python)")

manual = PDF_open_pdi(p, infile, "", 0)
if manual == -1:
    print "Error: " + PDF_get_errmsg(p) + "\n"
    exit(2)

row = 0
col = 0

PDF_set_parameter(p, "topdown", "true")

endpage = int(PDF_get_pdi_value(p, "/Root/Pages/Count", manual, -1, 0))

for pageno in range(1, endpage+1):
    if row == 0 and col == 0:
	PDF_begin_page_ext(p, width, height, "")
	font = PDF_load_font(p, "Helvetica-Bold", "winansi", "")
	PDF_setfont(p, font, 18)
	PDF_set_text_pos(p, 24, 24)
	PDF_show(p, "PDFlib Quick Reference")

    page = PDF_open_pdi_page(p, manual, pageno, "")

    if page == -1:
	print "Error: " + PDF_get_errmsg(p) + "\n"
	exit(2)

    optlist = "scale " + repr(1.0/maxrow)

    PDF_fit_pdi_page(p, page,
	    width/maxcol*col, (row + 1) * height/maxrow, optlist)
    PDF_close_pdi_page(p, page)

    col = col+1
    if col == maxcol:
	col = 0
	row = row+1

    if row == maxrow:
	row = 0
	PDF_end_page_ext(p, "")

# finish the last partial page
if row != 0 or col != 0:
    PDF_end_page_ext(p, "")

PDF_end_document(p, "")
PDF_close_pdi(p, manual)
PDF_delete(p)
