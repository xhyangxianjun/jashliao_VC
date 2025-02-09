/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2005 Thomas Merz and PDFlib GmbH. All rights reserved. |
 +---------------------------------------------------------------------------+
 |                                                                           |
 |    This software is subject to the PDFlib license. It is NOT in the       |
 |    public domain. Extended versions and commercial licenses are           |
 |    available, please check http://www.pdflib.com.                         |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* $Id: p_type3.c,v 1.80.2.7 2006/01/26 18:09:01 kurt Exp $
 *
 * Routines for Type 3 (user-defined) fonts
 *
 */

#include "p_intern.h"
#include "p_font.h"
#include "p_image.h"

int
pdf_get_t3colorized(PDF *p)
{
    return p->t3font->colorized;
}

static void
pdf_init_t3font_struct(PDF *p, pdc_t3font *t3font, int glyph_capacity)
{
    static char fn[] = "pdf_init_t3font_struct";
    int i;

    /* statement order is critical for cleanup!
    */
    t3font->fontname = NULL;
    t3font->next_glyph = 0;
    t3font->capacity = glyph_capacity;
    t3font->glyphs = (pdc_t3glyph *) 0;
    t3font->glyphs = (pdc_t3glyph *)
        pdc_malloc(p->pdc, t3font->capacity * sizeof (pdc_t3glyph), fn);

    for (i = 0; i < t3font->capacity; i++)
        t3font->glyphs[i].name = NULL;

    t3font->charprocs_id = PDC_BAD_ID;

    t3font->bbox.llx = 0;
    t3font->bbox.lly = 0;
    t3font->bbox.urx = 0;
    t3font->bbox.ury = 0;
}

static void
pdf_type3_protocol(PDF *p, pdc_font *font, pdc_encodingvector *ev)
{
    /* trace protocol */
    if (pdc_trace_protocol_is_enabled(p->pdc, 2, trc_font))
    {
        char *glyphname;
        int i, code, width;

        for (i = 0; i < font->t3font->next_glyph; ++i)
        {
            glyphname = font->t3font->glyphs[i].name;
            width = font->widths[i];

            for (code = 0; code < 256; code++)
                if (glyphname && ev->chars[code] &&
                    !strcmp(glyphname, ev->chars[code]))
                    break;

            pdc_trace(p->pdc,
                "\t\tGlyph%5d: Code=%3d  U+%04X  Width=%4d  \"%s\"\n",
                i, code, ev->codes[code], width, glyphname);
        }
    }
}

static int
name2width(pdc_t3font *t3font, const char *name)
{
    int i;

    for (i = 0; i < t3font->next_glyph; ++i) {
        if (t3font->glyphs[i].name && name &&
            !strcmp(t3font->glyphs[i].name, name))
                return (int) (t3font->glyphs[i].width + 0.5);
    }

    return 0;
}

/*
 * We found a Type 3 font in the cache, but its encoding doesn't match.
 * Make a copy of the font in a new slot, and attach the new encoding.
 */

int
pdf_handle_t3font(PDF *p, const char *fontname, pdc_encoding enc, int oldslot)
{
    static const char fn[] = "pdf_handle_t3font";
    char *fname;
    int slot, i;
    unsigned int namlen;
    pdc_font *oldfont, *newfont;
    pdc_encodingvector *ev = pdf_get_encoding_vector(p, enc);

    oldfont = &p->fonts[oldslot];

    /* slot of new font struct - initialized in pdf__load_font */
    slot = p->fonts_number;
    newfont = &p->fonts[slot];

    /* font name incl. encoding name */
    namlen = strlen(fontname) + (int) strlen(ev->apiname) + 2;
    fname = (char *) pdc_malloc(p->pdc, namlen, fn);
    pdc_sprintf(p->pdc, pdc_false, fname, "%s.%s", fontname, ev->apiname);

    pdc_trace_protocol(p->pdc, 1, trc_font,
        "\n\tType3 font \"%s\" with %d glyphs\n",
        fontname, oldfont->t3font->next_glyph);

    /*
     * This is a fresh font which has never been used.
     * Enter the new encoding, and we're done.
     */
    if (oldfont->encoding == pdc_invalidenc) {

        oldfont->encoding = enc;

        for (i = 0; i < oldfont->numOfCodes; i++) {
            oldfont->widths[i] =
                name2width(oldfont->t3font, ev->chars[i]);
            if (newfont->monospace && oldfont->widths[i])
                oldfont->widths[i] = newfont->monospace;
        }
        ev->flags |= PDC_ENC_USED;

        pdf_font_issemantic(p, oldfont);

        oldfont->name = fname;

        pdf_type3_protocol(p, oldfont, ev);

        return oldslot;
    }

    p->fonts_number++;

    /* copy the complete font structure */
    memcpy(newfont, oldfont, sizeof(pdc_font));

    /* make copies of relevant strings to ensure correct cleanup */
    newfont->name = fname;
    newfont->apiname = pdc_strdup(p->pdc, fontname);

    /* modify relevant entries */
    newfont->used_on_current_page       = pdc_false;
    newfont->encoding                   = enc;
    newfont->obj_id                     = pdc_alloc_id(p->out);

    newfont->t3font = (pdc_t3font*)pdc_malloc(p->pdc, sizeof(pdc_t3font), fn);
    pdf_init_t3font_struct(p, newfont->t3font, oldfont->t3font->capacity);

    /* copy relevant entries of Type 3 structure */
    newfont->t3font->charprocs_id = oldfont->t3font->charprocs_id;
    newfont->t3font->res_id = oldfont->t3font->res_id;
    newfont->t3font->colorized = oldfont->t3font->colorized;
    memcpy(&newfont->t3font->matrix, &oldfont->t3font->matrix,
        sizeof(pdc_matrix));
    memcpy(&newfont->t3font->bbox, &oldfont->t3font->bbox,
        sizeof(pdc_rectangle));

    /* copy the glyph/width list from oldfont to newfont */
    newfont->t3font->next_glyph = oldfont->t3font->next_glyph;
    for (i = 0; i < newfont->t3font->next_glyph; ++i)
    {
        newfont->t3font->glyphs[i].width =
            oldfont->t3font->glyphs[i].width;
        newfont->t3font->glyphs[i].charproc_id =
            oldfont->t3font->glyphs[i].charproc_id;
        newfont->t3font->glyphs[i].name =
            pdc_strdup(p->pdc, oldfont->t3font->glyphs[i].name);
    }

    newfont->widths = (int *) pdc_calloc(p->pdc,
                                 newfont->numOfCodes * sizeof(int), fn);
    for (i = 0; i < newfont->numOfCodes; i++)
    {
        newfont->widths[i] =
            name2width(newfont->t3font, ev->chars[i]);
        if (newfont->monospace && oldfont->widths[i])
            newfont->widths[i] = newfont->monospace;
    }
    ev->flags |= PDC_ENC_USED;

    pdf_font_issemantic(p, newfont);

    pdf_type3_protocol(p, newfont, ev);

    return slot;
}

/* definitions of begin font options */
static const pdc_defopt pdf_begin_font_options[] =
{
    {"colorized", pdc_booleanlist, 0, 1, 1, 0.0, 0.0, NULL},

    PDC_OPT_TERMINATE
};

void
pdf__begin_font(
    PDF *p,
    const char *fontname, int len,
    pdc_scalar a, pdc_scalar b, pdc_scalar c, pdc_scalar d,
    pdc_scalar e, pdc_scalar f,
    const char *optlist)
{
    static const char fn[] = "pdf__begin_font";
    char *fname;
    pdc_resopt *results;
    pdc_font *font;
    pdc_scalar det;
    int colorized = pdc_false;
    int slot;

    if (fontname == NULL)
        pdc_error(p->pdc, PDC_E_ILLARG_EMPTY, "fontname", 0, 0, 0);

    /* Converting fontname */
    fname = pdf_convert_name(p, fontname, len, pdc_true);
    if (fname == NULL || *fname == '\0')
        pdc_error(p->pdc, PDC_E_ILLARG_EMPTY, "fontname", 0, 0, 0);
    fontname = pdc_errprintf(p->pdc, "%.*s", PDC_ET_MAXSTRLEN,  fname);
    pdc_free(p->pdc, fname);

    pdc_check_number(p->pdc, "a", a);
    pdc_check_number(p->pdc, "b", b);
    pdc_check_number(p->pdc, "c", c);
    pdc_check_number(p->pdc, "d", d);
    pdc_check_number(p->pdc, "e", e);
    pdc_check_number(p->pdc, "f", f);

    pdc_trace_protocol(p->pdc, 1, trc_font,
        "\n\tBegin of Type3 font \"%s\"\n", fontname);

    /* parsing optlist */
    results = pdc_parse_optionlist(p->pdc, optlist, pdf_begin_font_options,
                                   NULL, pdc_true);
    pdc_get_optvalues("colorized", results, &colorized, NULL);
    pdc_cleanup_optionlist(p->pdc, results);

    det = a*d - b*c;

    if (det == 0)
        pdc_error(p->pdc, PDC_E_ILLARG_MATRIX,
            pdc_errprintf(p->pdc, "%f %f %f %f %f %f", a, b, c, d, e, f),
            0, 0, 0);

    /* look for an already existing font */
    for (slot = 0; slot < p->fonts_number; slot++)
    {
        if (!strcmp(p->fonts[slot].apiname, fontname))
        {
            pdc_error(p->pdc, PDF_E_T3_FONTEXISTS, fontname, 0, 0, 0);
        }
    }

    pdf_pg_suspend(p);
    PDF_SET_STATE(p, pdf_state_font);

    /* slot for new font struct */
    slot = pdf_init_newfont(p);
    font = &p->fonts[slot];
    p->fonts_number++;

    /*
     * We store the new font in a font slot marked with "invalidenc" encoding.
     * When the font is used for the first time we modify the encoding.
     * Later uses will make a copy if the encoding is different.
     */

    font->type          = pdc_Type3;
    font->obj_id        = pdc_alloc_id(p->out);

    font->name          = NULL;
    font->apiname       = pdc_strdup(p->pdc, fontname);
    font->encoding      = pdc_invalidenc;  /* initially there is no encoding */
    font->widths        = (int *) pdc_calloc(p->pdc,
                                 font->numOfCodes * sizeof(int), fn);

    pdf_font_set_missvalues(p, font);

    font->t3font = (pdc_t3font*) pdc_malloc(p->pdc, sizeof(pdc_t3font), fn);
    pdf_init_t3font_struct(p, font->t3font, T3GLYPHS_CHUNKSIZE);

    font->t3font->fontname      = pdc_strdup(p->pdc, fontname);
    font->t3font->colorized     = colorized;

    /* the resource id is needed until the font dict is written */
    font->t3font->res_id        = pdc_alloc_id(p->out);

    font->t3font->matrix.a      = a;
    font->t3font->matrix.b      = b;
    font->t3font->matrix.c      = c;
    font->t3font->matrix.d      = d;
    font->t3font->matrix.e      = e;
    font->t3font->matrix.f      = f;

    /* We won't receive glyph bounding box values for colorized true,
     * so we use the font matrix to get an approximation instead.
     *
     * Writing the font matrix should be optional according to the
     * PDF reference, but we must write it in order to work around a
     * display bug in Acrobat.
     *
     * Disabled from PDFlib 6.0.1
     *
    if (font->t3font->colorized == pdc_true) {
        font->t3font->bbox.llx = 0;
        font->t3font->bbox.lly = 0;
        font->t3font->bbox.urx = (d-b)/det;
        font->t3font->bbox.ury = (a-c)/det;
    }
     */


    /*
     * We must store a pointer to the current font because its glyph
     * definitions may use other fonts and we would be unable to find
     * "our" current font again. This pointer lives throughout the
     * font definition, and will be reset in PDF_end_font() below.
     */
    p->t3font = font->t3font;
}

void
pdf__end_font(PDF *p)
{
    int i;
    pdc_t3font *t3font;

    PDF_SET_STATE(p, pdf_state_document);

    t3font = p->t3font;

    t3font->charprocs_id = pdc_alloc_id(p->out);
    pdc_begin_obj(p->out, t3font->charprocs_id);        /* CharProcs dict */
    pdc_begin_dict(p->out);

    for (i = 0; i < t3font->next_glyph; i++) {
        pdf_put_pdfname(p, t3font->glyphs[i].name);
        pdc_objref(p->out, "", t3font->glyphs[i].charproc_id);
    }

    pdc_end_dict(p->out);
    pdc_end_obj(p->out);                        /* CharProcs dict */

    pdc_begin_obj(p->out, t3font->res_id);
    pdc_begin_dict(p->out);                     /* Resource dict */

    pdf_write_page_fonts(p);                    /* Font resources */

    pdf_write_page_colorspaces(p);              /* Color space resources */

    pdf_write_page_pattern(p);                  /* Pattern resources */

    pdf_write_xobjects(p);                      /* XObject resources */

    pdc_end_dict(p->out);                       /* Resource dict */
    pdc_end_obj(p->out);                        /* Resource object */

    pdf_pg_resume(p, -1);

    pdc_trace_protocol(p->pdc, 1, trc_font,
        "\n\tEnd of Type3 font \"%s\"\n", p->t3font->fontname);

    p->t3font = (pdc_t3font *) NULL;

    if (p->flush & pdf_flush_content)
        pdc_flush_stream(p->out);
}

void
pdf__begin_glyph(
    PDF *p,
    const char *glyphname,
    pdc_scalar wx,
    pdc_scalar llx, pdc_scalar lly, pdc_scalar urx, pdc_scalar ury)
{
    static const char fn[] = "pdf__begin_glyph";
    pdc_t3font *t3font;
    pdc_t3glyph *glyph;
    int i;

    t3font = p->t3font;

    pdc_check_number(p->pdc, "wx", wx);
    pdc_check_number(p->pdc, "llx", llx);
    pdc_check_number(p->pdc, "lly", lly);
    pdc_check_number(p->pdc, "urx", urx);
    pdc_check_number(p->pdc, "ury", ury);

    if (t3font->colorized == pdc_true &&
        (llx != 0 || lly != 0 ||
         urx != 0 || ury != 0))
        pdc_error(p->pdc, PDF_E_T3_BADBBOX, t3font->fontname, 0, 0, 0);

    PDF_SET_STATE(p, pdf_state_glyph);

    for (i = 0; i < t3font->next_glyph; ++i) {
        if (!strcmp(t3font->glyphs[i].name, glyphname)) {
            pdc_error(p->pdc, PDF_E_T3_GLYPH, glyphname,
                      t3font->fontname, 0, 0);
        }
    }

    pdc_trace_protocol(p->pdc, 1, trc_font,
        "\n\tBegin of Type3 font glyph \"%s\"\n", glyphname);

    if (t3font->next_glyph == t3font->capacity) {
        t3font->capacity *= 2;
        t3font->glyphs = (pdc_t3glyph *) pdc_realloc(p->pdc, t3font->glyphs,
            t3font->capacity * sizeof (pdc_t3glyph), fn);
    }

    glyph               = &t3font->glyphs[t3font->next_glyph];
    glyph->charproc_id  = pdc_begin_obj(p->out, PDC_NEW_ID);
    glyph->name         = pdc_strdup(p->pdc, glyphname);

    /* see comment in p_font.c for explanation */
    glyph->width        = 1000 * wx * t3font->matrix.a;

    /* if the strdup above fails, cleanup won't touch this slot. */
    ++t3font->next_glyph;
                                                /* glyph description */
    pdc_begin_dict(p->out);                     /* glyph description dict */

    p->length_id = pdc_alloc_id(p->out);
    pdc_objref(p->out, "/Length", p->length_id);

    if (pdc_get_compresslevel(p->out))
        pdc_puts(p->out, "/Filter/FlateDecode\n");

    pdc_end_dict(p->out);                       /* glyph description dict */

    pdc_begin_pdfstream(p->out);                /* glyph description stream */

    if (t3font->colorized == pdc_true)
        pdc_printf(p->out, "%f 0 d0\n", wx);
    else {
        pdc_printf(p->out, "%f 0 %f %f %f %f d1\n", wx, llx, lly, urx, ury);

        /* adjust the font's bounding box */
        if (llx < t3font->bbox.llx)
            t3font->bbox.llx = llx;
        if (lly < t3font->bbox.lly)
            t3font->bbox.lly = lly;
        if (urx > t3font->bbox.urx)
            t3font->bbox.urx = urx;
        if (ury > t3font->bbox.ury)
            t3font->bbox.ury = ury;
    }
}

void
pdf__end_glyph(PDF *p)
{
    PDF_SET_STATE(p, pdf_state_font);

    /* check whether pdf__save() and pdf__restore() calls are balanced */
    if (p->curr_ppt->sl > 0)
        pdc_error(p->pdc, PDF_E_GSTATE_UNMATCHEDSAVE, 0, 0, 0, 0);

    pdf_end_text(p);
    pdc_end_pdfstream(p->out);                  /* glyph description stream */
    pdc_end_obj(p->out);                        /* glyph description */

    pdc_put_pdfstreamlength(p->out, p->length_id);

    pdc_trace_protocol(p->pdc, 1, trc_font,
        "\n\tEnd of Type3 font glyph \"%s\"\n",
        p->t3font->glyphs[p->t3font->next_glyph-1].name);
}


void
pdf_init_type3(PDF *p)
{
    p->t3font = NULL;
}

