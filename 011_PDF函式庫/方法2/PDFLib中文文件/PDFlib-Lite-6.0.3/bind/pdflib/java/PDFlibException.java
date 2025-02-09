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

/* $Id: PDFlibException.java,v 1.20.2.26 2006/02/27 17:16:58 rjs Exp $
 *
 * PDFlibException Java class
 */

package com.pdflib;

/** PDFlib - A library for generating PDF on the fly

    Exception handling for PDFlib.
    @author Rainer Schaaf
    @version 6.0.3
*/

public class PDFlibException extends Exception {
    public PDFlibException() {
        super();
    }

    public PDFlibException(String msg) {
        super(msg);
    }

    public PDFlibException(String msg, int errnum, String apiname) {
        super(msg);
        pdf_errnum = errnum;
        pdf_apiname = apiname;
    }

    /** Map standard getMessage method to get_errmsg. */
    public String get_errmsg() {
        return super.getMessage();
    }

    /** Get the error number of the exception. */
    public int get_errnum() {
        return pdf_errnum;
    }

    /** Get the name of the API function which caused the exception. */
    public String get_apiname() {
        return pdf_apiname;
    }

    private int pdf_errnum;
    private String pdf_apiname;
}
