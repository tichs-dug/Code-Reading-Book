/*
 * SsiFlastmod.java
 * $Header: /home/cvspublic/jakarta-tomcat-4.0/catalina/src/share/org/apache/catalina/util/ssi/SsiFlastmod.java,v 1.3 2001/07/22 20:25:15 pier Exp $
 * $Revision: 1.3 $
 * $Date: 2001/07/22 20:25:15 $
 *
 * ====================================================================
 *
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The names "The Jakarta Project", "Tomcat", and "Apache Software
 *    Foundation" must not be used to endorse or promote products derived
 *    from this software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * [Additional notices, if required by prior licensing conditions]
 *
 */

package org.apache.catalina.util.ssi;

import java.util.Date;
import java.io.IOException;
import java.net.URL;
import java.net.MalformedURLException;
import javax.servlet.ServletContext;

/**
 * Get the last modified date for a file, the date is subject
 * of formatting.
 *
 * @author Bip Thelin
 * @version $Revision: 1.3 $, $Date: 2001/07/22 20:25:15 $
 */
public final class SsiFlastmod
    extends SsiMediator implements SsiCommand {

    /**
     * Get the date of a file and format it correctly
     *
     * @param strParamType The type of parameter
     * @param strParam The value of the parameter
     * @return The date of the file
     */
    public final String getStream(String[] strParamType,
                                  String[] strParam) {
        String path = new String();
        long lastModified;
        String sLastModified = null;
        URL url = null;

        if(strParamType[0].equals("file")) {
            path = super.getFilePath(strParam[0]);
        } else if(strParamType[0].equals("virtual")) {
            path = super.getVirtualPath(strParam[0]);
        }

        try {
            url = super.servletContext.getResource(path);
            lastModified = url.openConnection().getLastModified();
        } catch (MalformedURLException e){
            lastModified = 0;
        } catch (IOException e) {
            lastModified = 0;
        } catch (NullPointerException e) {
            lastModified = 0;
        }

        if(lastModified==0)
            sLastModified = (new String(super.getError()));
        else
            sLastModified = super.timefmt(new Date(lastModified));

        return sLastModified;
    }

    /**
     * Get the date of a file and format it correctly
     *
     * @param path The path to the file
     * @return The Date to return
     */
    protected String getDate(String path) {
        long lastModified;
        String sLastModified = null;
        URL url = null;

        path = super.getVirtualPath(path);

        try {
            url = super.servletContext.getResource(path);
            lastModified = url.openConnection().getLastModified();
        } catch (MalformedURLException e){
            lastModified = 0;
        } catch (IOException e) {
            lastModified = 0;
        } catch (NullPointerException e) {
            lastModified = 0;
        }

        if(lastModified==0)
            sLastModified = (new String(super.getError()));
        else
            sLastModified = super.timefmt(new Date(lastModified));

        return sLastModified;
    }

    /**
     * Not used since this SsiCommand return a stream, use
     * <code>getStream()</code> instead.
     *
     * @param strParamType a value of type 'String[]'
     * @param strParam a value of type 'String[]'
     */
    public final void process(String[] strParamType, String[] strParam) {}

    /**
     * Returns <code>true</code> this SsiCommand is always prnitable
     * and should therefore be accsessed through <code>getStream()</code>
     *
     * @return a value of type 'boolean'
     */
    public final boolean isPrintable() { return true; }

    /**
     * Returns <code>false</code>, this SsiCommands is never buffered.
     *
     * @return a value of type 'boolean'
     */
    public final boolean isModified() { return false; }
}
