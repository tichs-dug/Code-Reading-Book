/*****************************************************************************
 * Copyright (C) The Apache Software Foundation. All rights reserved.        *
 * ------------------------------------------------------------------------- *
 * This software is published under the terms of the Apache Software License *
 * version 1.1, a copy of which has been included  with this distribution in *
 * the LICENSE file.                                                         *
 *****************************************************************************/
package org.apache.cocoon.generation;

import org.apache.avalon.framework.activity.Initializable;
import org.apache.avalon.framework.activity.Disposable;
import org.apache.avalon.framework.component.ComponentException;
import org.apache.avalon.framework.component.ComponentManager;
import org.apache.avalon.framework.context.Context;
import org.apache.avalon.framework.context.ContextException;
import org.apache.avalon.framework.context.Contextualizable;
import org.apache.avalon.framework.parameters.Parameters;

import org.apache.avalon.excalibur.pool.Recyclable;

import org.apache.cocoon.Constants;
import org.apache.cocoon.ProcessingException;
import org.apache.cocoon.components.search.LuceneCocoonSearcher;
import org.apache.cocoon.components.search.LuceneXMLIndexer;
import org.apache.cocoon.components.search.LuceneCocoonPager;
import org.apache.cocoon.components.search.LuceneCocoonHelper;
import org.apache.cocoon.environment.Request;
import org.apache.cocoon.environment.SourceResolver;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.search.Hits;
import org.apache.lucene.store.Directory;

import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.AttributesImpl;

import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 * Generates an XML representation of a search result.
 *
 * <p>
 *  This generator generates xml content representening an XML search.
 *  The generated xml content contains the search result, 
 *  the search query information, and navigation information about the
 *  search results.
 * </p>
 *
 * <p>
 *  Search xml sample generated by this generator:
 * </p>
 * <pre><tt>
 * &lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;
 * &lt;search:results date=&quot;1008437081064&quot; query-string=&quot;cocoon&quot;
 * start-index=&quot;0&quot; page-length=&quot;10&quot;
 * xmlns:search=&quot;http://apache.org/cocoon/search/1.0&quot;
 * xmlns:xlink=&quot;http://www.w3.org/1999/xlink&quot;&gt;
 * &lt;search:hits total-count=&quot;125&quot; count-of-pages=&quot;13&quot;&gt;
 * &lt;search:hit rank=&quot;0&quot; score=&quot;1.0&quot;
 * uri=&quot;http://localhost:8080/cocoon/documents/hosting.html&quot;/&gt;
 * ...
 * &lt;/search:hits&gt;
 * &lt;search:navigation total-count=&quot;125&quot; count-of-pages=&quot;13&quot;
 * has-next=&quot;true&quot; has-previous=&quot;false&quot; next-index=&quot;10&quot; previous-index=&quot;0&quot;&gt;
 * &lt;search:navigation-page start-index=&quot;0&quot;/&gt;
 * &lt;search:navigation-page start-index=&quot;10&quot;/&gt;
 * ...
 * &lt;search:navigation-page start-index=&quot;120&quot;/&gt;
 * &lt;/search:navigation&gt;
 * &lt;/search:results&gt;
 * </tt></pre>
 *
 * @author     <a href="mailto:berni_huber@a1.net">Bernhard Huber</a>
 * @author     <a href="mailto:vgritsenko@apache.org">Vadim Gritsenko</a>
 * @version    CVS $Id: SearchGenerator.java,v 1.2 2002/01/24 03:20:25 vgritsenko Exp $
 */
public class SearchGenerator extends ComposerGenerator
    implements Recyclable, Contextualizable, Initializable, Disposable
{

    /**
     * The XML namespace for the output document.
     *
     * @since
     */
    protected final static String namespace = "http://apache.org/cocoon/search/1.0";

    /**
     * The XML namespace for xlink
     *
     * @since
     */
    protected final static String xlinkNamespace = "http://www.w3.org/1999/xlink";

    /**
     * Description of the Field
     *
     * @since
     */
    protected final static String CDATA = "CDATA";

    /**
     * Root element of generated xml content, ie <code>results</code>.
     *
     * @since
     */
    protected final static String RESULTS_ELEMENT = "results";

    /**
     * Attribute <code>date</code> of <code>results</code> element.
     * It contains the date a long value, indicating when a search
     * generated this xml content.
     *
     * @since
     */
    protected final static String DATE_ATTRIBUTE = "date";

    /**
     * Attribute <code>query-string</code> of <code>results</code> element.
     * Echos the <code>queryString</code> query parameter.
     *
     * @since
     */
    protected final static String QUERY_STRING_ATTRIBUTE = "query-string";

    /**
     * Attribute <code>start-index</code> of <code>results</code> element.
     * Echos the <code>startIndex</code> query parameter.
     *
     * @since
     */
    protected final static String START_INDEX_ATTRIBUTE = "start-index";

    /**
     * Attribute <code>page-length</code> of <code>results</code> element.
     * Echos the <code>pageLenth</code> query parameter.
     *
     * @since
     */
    protected final static String PAGE_LENGTH_ATTRIBUTE = "page-length";

    /**
     * Child element of generated xml content, ie <code>hits</code>.
     * This element describes all hits.
     *
     * @since
     */
    protected final static String HITS_ELEMENT = "hits";

    /**
     * Attribute <code>total-count</code> of <code>hits</code> element.
     * The value describes total number of hits found by the search engine.
     *
     * @since
     */
    protected final static String TOTAL_COUNT_ATTRIBUTE = "total-count";

    /**
     * Attribute <code>count-of-pages</code> of <code>hits</code> element.
     * The value describes number of pages needed for all hits.
     *
     * @since
     */
    protected final static String COUNT_OF_PAGES_ATTRIBUTE = "count-of-pages";

    /**
     * Child element of generated xml content, ie <code>hit</code>.
     * This element describes a single hit.
     *
     * @since
     */
    protected final static String HIT_ELEMENT = "hit";

    /**
     * Attribute <code>rank</code> of <code>hit</code> element.
     * The value describes the count index of this hits, ranging between 0, and
     * total-count minus 1.
     *
     * @since
     */
    protected final static String RANK_ATTRIBUTE = "rank";

    /**
     * Attribute <code>score</code> of <code>hit</code> element.
     * The value describes the score of this hits, ranging between 0, and 1.0.
     *
     * @since
     */
    protected final static String SCORE_ATTRIBUTE = "score";

    /**
     * Attribute <code>uri</code> of <code>hit</code> element.
     * The value describes the uri of a document matching the search query.
     *
     * @since
     */
    protected final static String URI_ATTRIBUTE = "uri";

    /**
     * Child element of generated xml content, ie <code>navigation</code>.
     * This element describes some hints for easier navigation.
     *
     * @since
     */
    protected final static String NAVIGATION_ELEMENT = "navigation";

    /**
     * Child element of generated xml content, ie <code>navigation</code>.
     * This element describes the start-index of page containing hits.
     *
     * @since
     */
    protected final static String NAVIGATION_PAGE_ELEMENT = "navigation-page";

    /**
     * Attribute <code>has-next</code> of <code>navigation-page</code> element.
     * The value is true if a next navigation control should be presented.
     *
     * @since
     */
    protected final static String HAS_NEXT_ATTRIBUTE = "has-next";

    /**
     * Attribute <code>has-next</code> of <code>navigation-page</code> element.
     * The value is true if a previous navigation control should be presented.
     *
     * @since
     */
    protected final static String HAS_PREVIOUS_ATTRIBUTE = "has-previous";

    /**
     * Attribute <code>next-index</code> of <code>navigation-page</code> element.
     * The value describes the start-index of the next-to-be-presented page.
     *
     * @since
     */
    protected final static String NEXT_INDEX_ATTRIBUTE = "next-index";

    /**
     * Attribute <code>previous-index</code> of <code>navigation-page</code> element.
     * The value describes the start-index of the previous-to-be-presented page.
     *
     * @since
     */
    protected final static String PREVIOUS_INDEX_ATTRIBUTE = "previous-index";

    /**
     * Setup parameter name of index directory, ie <code>index</code>.
     *
     * @since
     */
    protected final static String INDEX_PARAM = "index";

    /**
     * Default value of setup parameter <code>index</code>, ie <code>index</code>.
     *
     * @since
     */
    protected final static String INDEX_PARAM_DEFAULT = "index";

    /**
     * Setup parameter name specifying the name of query-string query parameter,
     * ie <code>query-string</code>.
     *
     * @since
     */
    protected final static String QUERY_STRING_PARAM = "query-string";

    /**
     * Default value of setup parameter <code>query-string</code>, ie <code>queryString</code>.
     *
     * @since
     */
    protected final static String QUERY_STRING_PARAM_DEFAULT = "queryString";

    /**
     * Setup parameter name specifying the name of start-index query parameter,
     * ie <code>start-index</code>.
     *
     * @since
     */
    protected final static String START_INDEX_PARAM = "start-index";

    /**
     *Description of the Field
     *
     * @since
     */
    protected final static String START_INDEX_PARAM_DEFAULT = "startIndex";

    /**
     * Setup parameter name specifying the name of start-next-index query parameter,
     * ie <code>start-next-index</code>.
     *
     * @since
     */
    protected final static String START_INDEX_NEXT_PARAM = "start-next-index";

    /**
     *Description of the Field
     *
     * @since
     */
    protected final static String START_INDEX_NEXT_PARAM_DEFAULT = "startNextIndex";

    /**
     * Setup parameter name specifying the name of start-previous-index query parameter,
     * ie <code>start-previous-index</code>.
     *
     * @since
     */
    protected final static String START_INDEX_PREVIOUS_PARAM = "start-previous-index";

    /**
     *Description of the Field
     *
     * @since
     */
    protected final static String START_INDEX_PREVIOUS_PARAM_DEFAULT = "startPreviousIndex";

    /**
     *Description of the Field
     *
     * @since
     */
    protected final static int START_INDEX_DEFAULT = 0;

    /**
     * Setup parameter name specifying the name of page-length query parameter,
     * ie <code>page-length</code>.
     *
     * @since
     */
    protected final static String PAGE_LENGTH_PARAM = "page-length";

    /**
     *Description of the Field
     *
     * @since
     */
    protected final static String PAGE_LENGTH_PARAM_DEFAULT = "pageLength";

    /**
     *Description of the Field
     *
     * @since
     */
    protected final static int PAGE_LENGTH_DEFAULT = 10;



    /**
     * Default home directory of index directories.
     * <p>
     *   Releative index directories specified in the setup of this generator are resolved
     *   relative to this directory.
     * </p>
     * <p>
     *   By default this directory is set to the <code>WORKING_DIR</code> of Cocoon.
     * </p>
     *
     * @since
     */
    private File workDir = null;

    /**
     * The avalon component to use for searching.
     *
     * @since
     */
    private LuceneCocoonSearcher lcs;

    /**
     * Absolute filesystem directory of lucene index directory
     *
     * @since
     */
    private File index = null;

    /**
     * Query-string to search for
     *
     * @since
     */
    private String queryString = "";

    /**
     * Attributes used when generating xml content.
     *
     * @since
     */
    private final AttributesImpl atts = new AttributesImpl();

    /**
     * startIndex of query parameter
     *
     * @since
     */
    private Integer startIndex = null;

    /**
     * pageLength of query parameter
     *
     * @since
     */
    private Integer pageLength = null;


    // TODO: parameterize()

    /**
     * Set the current <code>ComponentManager</code> instance used by this
     * <code>Composable</code>.
     */
    public void compose(ComponentManager manager) throws ComponentException {
        super.compose(manager);
//        lcs = (LuceneCocoonSearcher) this.manager.lookup(LuceneCocoonSearcher.ROLE);
    }

    /**
     * setup all members of this generator.
     *
     * @since
     */
    public void setup(SourceResolver resolver, Map objectModel, String src, Parameters par)
             throws ProcessingException, SAXException, IOException {
        super.setup(resolver, objectModel, src, par);

        String param_name;
        Request request = (Request) objectModel.get(Constants.REQUEST_OBJECT);

        // get the analyzer
//        Analyzer analyzer = LuceneCocoonHelper.getAnalyzer("org.apache.lucene.analysis.standard.StandardAnalyzer");
//        lcs.setAnalyzer(analyzer);

        param_name = par.getParameter(INDEX_PARAM, INDEX_PARAM_DEFAULT);
        String index_file_name = param_name;
        if (request.getParameter(INDEX_PARAM) != null) {
            index_file_name = request.getParameter(INDEX_PARAM);
        }

        // now set the index
        index = new File(index_file_name);
        if (!index.isAbsolute()) {
            index = new File(workDir, index.toString());
        }

        // get queryString
        // always try lookup the start index from the request params
        startIndex = null;
        param_name = par.getParameter(QUERY_STRING_PARAM, QUERY_STRING_PARAM_DEFAULT);
        if (request.getParameter(param_name) != null) {
            queryString = request.getParameter(param_name);
        }

        // get startIndex
        param_name = par.getParameter(START_INDEX_NEXT_PARAM, START_INDEX_NEXT_PARAM_DEFAULT);
        if (request.getParameter(param_name) != null) {
            startIndex = createInteger(request.getParameter(param_name));
        }

        if (startIndex == null) {
            param_name = par.getParameter(START_INDEX_PREVIOUS_PARAM, START_INDEX_PREVIOUS_PARAM_DEFAULT);
            if (request.getParameter(param_name) != null) {
                startIndex = createInteger(request.getParameter(param_name));
            }
        }
        if (startIndex == null) {
            param_name = par.getParameter(START_INDEX_PARAM, START_INDEX_PARAM_DEFAULT);
            if (request.getParameter(param_name) != null) {
                startIndex = createInteger(request.getParameter(param_name));
            }
        }

        // get pageLength
        param_name = par.getParameter(PAGE_LENGTH_PARAM, PAGE_LENGTH_PARAM_DEFAULT);
        if (request.getParameter(param_name) != null) {
            pageLength = createInteger(request.getParameter(param_name));
        }
    }


    /**
     * Contextualize this class.
     *
     * <p>
     *   Especially retrieve the work directory.
     *   If the index directory is specified relativly, the working directory is
     *   used as home directory of the index directory.
     * </p>
     *
     * @param  context               Context to use
     * @exception  ContextException  If contextualizing fails.
     * @since
     */
    public void contextualize(Context context) throws ContextException {
        // retrieve the working directory, assuming that the index may reside there
        workDir = (File) context.get(Constants.CONTEXT_WORK_DIR);
    }

    public void initialize() throws IOException {
        // get the directory where the index resides
//        Directory directory = LuceneCocoonHelper.getDirectory(new File(workDir, "index"), false);
//        lcs.setDirectory(directory);
    }

    /**
     * Generate xml content describing search results.
     * Entry point of the ComposerGenerator.
     * The xml content is generated from the hits object.
     *
     *
     * @exception  IOException       when there is a problem reading the from file system.
     * @since
     * @throws  SAXException         when there is a problem creating the output SAX events.
     * @throws  ProcessingException  when there is a problem obtaining the hits
     */
    public void generate() throws IOException, SAXException, ProcessingException {
        // set default parameter value, in case of no values are set yet.
        if (startIndex == null) {
            startIndex = new Integer(START_INDEX_DEFAULT);
        }
        if (pageLength == null) {
            pageLength = new Integer(PAGE_LENGTH_DEFAULT);
        }

        // Start the document and set the namespace.
        this.contentHandler.startDocument();
        this.contentHandler.startPrefixMapping("search", namespace);
        this.contentHandler.startPrefixMapping("xlink", xlinkNamespace);

        generateResults();

        // End the document.
        this.contentHandler.endPrefixMapping("xlink");
        this.contentHandler.endPrefixMapping("");
        this.contentHandler.endDocument();
    }


    /**
     * Create an Integer from a String.
     * <p>
     *   Create an Integer from String s, iff conversion fails use default_value.
     * </p>
     *
     * @param  s              Converting s to an Integer
     * @param  default_value  Default value to use, iff conversion of s fails.
     * @return                Integer        converted value originating from s, or default_value
     * @since
     */
    private Integer createInteger(String s, int default_value) {
        Integer i = new Integer(default_value);
        try {
            i = new Integer(s);
        } catch (NumberFormatException nfe) {
            // ignore it, write only warning
            if (getLogger().isWarnEnabled()) {
                getLogger().warn("Cannot convert " + s + " to Integer", nfe);
            }
        }
        return i;
    }


    /**
     * Create an Integer.
     * <p>
     *   Create an Integer from String s, iff conversion fails return null.
     * </p>
     *
     * @param  s  Converting s to an Integer
     * @return    Integer converted value originating from s, or null
     * @since
     */
    private Integer createInteger(String s) {
        Integer i = null;
        try {
            i = new Integer(s);
        } catch (NumberFormatException nfe) {
            // ignore it, write only warning
            if (getLogger().isWarnEnabled()) {
                getLogger().warn("Cannot convert " + s + " to Integer", nfe);
            }
        }
        return i;
    }


    /**
     * Build and generate the search results.
     * <p>
     *  First build the hits, next generate xml content from the hits,
     *  taking page index, and length into account.
     * </p>
     *
     * @since
     * @throws  SAXException         when there is a problem creating the output SAX events.
     * @throws  ProcessingException  when there is a problem obtaining the hits
     */
    private void generateResults() throws SAXException, ProcessingException {

        // Make the hits
        LuceneCocoonPager pager = buildHits();

        // The current date and time.
        long time = System.currentTimeMillis();

        atts.clear();
        atts.addAttribute(namespace, DATE_ATTRIBUTE, 
            DATE_ATTRIBUTE, CDATA, String.valueOf(time));
        if (queryString != null && queryString.length() > 0)
            atts.addAttribute(namespace, QUERY_STRING_ATTRIBUTE, 
                QUERY_STRING_ATTRIBUTE, CDATA, String.valueOf(queryString));
        atts.addAttribute(namespace, START_INDEX_ATTRIBUTE, 
            START_INDEX_ATTRIBUTE, CDATA, String.valueOf(startIndex));
        atts.addAttribute(namespace, PAGE_LENGTH_ATTRIBUTE, 
            PAGE_LENGTH_ATTRIBUTE, CDATA, String.valueOf(pageLength));

        contentHandler.startElement(namespace, RESULTS_ELEMENT, RESULTS_ELEMENT, atts);

        // build xml from the hits
        generateHits(pager);
        generateNavigation(pager);

        // End root element.
        contentHandler.endElement(namespace, "results", "results");
    }


    /**
     * Generate the xml content of all hits
     *
     * @param  pager                 the LuceneContentPager with the search results
     * @since
     * @throws  SAXException         when there is a problem creating the output SAX events.
     * @throws  ProcessingException  when there is a problem obtaining the hits
     */
    private void generateHits(LuceneCocoonPager pager) throws SAXException, ProcessingException {
        if (pager != null && pager.hasNext()) {
            atts.clear();
            atts.addAttribute(namespace, TOTAL_COUNT_ATTRIBUTE, TOTAL_COUNT_ATTRIBUTE, 
                CDATA, String.valueOf(pager.getCountOfHits()));
            atts.addAttribute(namespace, COUNT_OF_PAGES_ATTRIBUTE, COUNT_OF_PAGES_ATTRIBUTE, 
                CDATA, String.valueOf(pager.getCountOfPages()));
            contentHandler.startElement(namespace, HITS_ELEMENT, HITS_ELEMENT, atts);
            generateHit(pager);
            contentHandler.endElement(namespace, HITS_ELEMENT, HITS_ELEMENT);
        }
    }


    /**
     * Generate the xml content for each hit.
     *
     * @param  pager                 the LuceneCocoonPager with the search results.
     * @since
     * @throws  SAXException         when there is a problem creating the output SAX events.
     * @throws  ProcessingException  when there is a problem obtaining the hits
     */
    private void generateHit(LuceneCocoonPager pager) throws SAXException, ProcessingException {
        // get the off set to start from
        int counter = pager.getStartIndex();

        // get an list of hits which should be placed onto a single page
        List l = (List) pager.next();
        Iterator i = l.iterator();
        for (; i.hasNext(); counter++) {
            LuceneCocoonPager.HitWrapper hw = (LuceneCocoonPager.HitWrapper) i.next();
            Document doc = hw.getDocument();
            float score = hw.getScore();
            String uri = doc.get(LuceneXMLIndexer.URL_FIELD);

            atts.clear();
            atts.addAttribute(namespace, RANK_ATTRIBUTE, RANK_ATTRIBUTE, CDATA,
                    String.valueOf(counter));
            atts.addAttribute(namespace, SCORE_ATTRIBUTE, SCORE_ATTRIBUTE, CDATA,
                    String.valueOf(score));
            atts.addAttribute(namespace, URI_ATTRIBUTE, URI_ATTRIBUTE, CDATA,
                    String.valueOf(uri));
            contentHandler.startElement(namespace, HIT_ELEMENT, HIT_ELEMENT, atts);
            // fix me, add here a summary of this hit
            contentHandler.endElement(namespace, HIT_ELEMENT, HIT_ELEMENT);
        }
    }


    /**
     * Generate the navigation element.
     *
     * @param  pager                    Description of Parameter
     * @exception  SAXException         Description of Exception
     * @exception  ProcessingException  Description of Exception
     * @since
     */
    private void generateNavigation(LuceneCocoonPager pager) throws SAXException, ProcessingException {

        if (pager != null) {
            // generate navigation element
            atts.clear();
            atts.addAttribute(namespace, TOTAL_COUNT_ATTRIBUTE, TOTAL_COUNT_ATTRIBUTE, 
                CDATA, String.valueOf(pager.getCountOfHits()));
            atts.addAttribute(namespace, COUNT_OF_PAGES_ATTRIBUTE, COUNT_OF_PAGES_ATTRIBUTE, 
                CDATA, String.valueOf(pager.getCountOfPages()));
            atts.addAttribute(namespace, HAS_NEXT_ATTRIBUTE, HAS_NEXT_ATTRIBUTE, 
                CDATA, String.valueOf(pager.hasNext()));
            atts.addAttribute(namespace, HAS_PREVIOUS_ATTRIBUTE, HAS_PREVIOUS_ATTRIBUTE, 
                CDATA, String.valueOf(pager.hasPrevious()));
            atts.addAttribute(namespace, NEXT_INDEX_ATTRIBUTE, NEXT_INDEX_ATTRIBUTE, 
                CDATA, String.valueOf(pager.nextIndex()));
            atts.addAttribute(namespace, PREVIOUS_INDEX_ATTRIBUTE, PREVIOUS_INDEX_ATTRIBUTE, 
                CDATA, String.valueOf(pager.previousIndex()));
            contentHandler.startElement(namespace, NAVIGATION_ELEMENT, NAVIGATION_ELEMENT, atts);
            int count_of_pages = pager.getCountOfPages();
            for (int i = 0, page_start_index = 0;
                    i < count_of_pages;
                    i++, page_start_index += pageLength.intValue()) {
                atts.clear();
                atts.addAttribute(namespace, START_INDEX_ATTRIBUTE, START_INDEX_ATTRIBUTE, 
                    CDATA, String.valueOf(page_start_index));
                contentHandler.startElement(namespace, NAVIGATION_PAGE_ELEMENT, NAVIGATION_PAGE_ELEMENT, atts);
                contentHandler.endElement(namespace, NAVIGATION_PAGE_ELEMENT, NAVIGATION_PAGE_ELEMENT);
            }
            // navigation is EMPTY element
            contentHandler.endElement(namespace, NAVIGATION_ELEMENT, NAVIGATION_ELEMENT);
        }
    }


    /**
     * Build hits from a query input, and setup paging object.
     *
     * @since
     * @throws  ProcessingException  iff an error occurs
     */
    private LuceneCocoonPager buildHits() throws ProcessingException {
        
        if (queryString != null && queryString.length() != 0) {
            Hits hits = null;

            // TODO (VG): Move parts into compose/initialize/recycle
            try {
                lcs = (LuceneCocoonSearcher) this.manager.lookup(LuceneCocoonSearcher.ROLE);
                Analyzer analyzer = LuceneCocoonHelper.getAnalyzer("org.apache.lucene.analysis.standard.StandardAnalyzer");
                lcs.setAnalyzer(analyzer);
                // get the directory where the index resides
                Directory directory = LuceneCocoonHelper.getDirectory(new File(workDir, "index"), false);
                lcs.setDirectory(directory);
                hits = lcs.search(queryString, LuceneXMLIndexer.BODY_FIELD);
            } catch (IOException ioe) {
                throw new ProcessingException("IOException in search", ioe);
            } catch (ComponentException ce) {
                throw new ProcessingException("ComponentException in search", ce);
            } finally {
                if (lcs != null) {
                    this.manager.release(lcs);
                    lcs = null;
                }
            }
        
            // wrap the hits by an pager help object for accessing only a range of hits
            LuceneCocoonPager pager = new LuceneCocoonPager(hits);

            int start_index = START_INDEX_DEFAULT;
            if (this.startIndex != null) {
                start_index = this.startIndex.intValue();
                if (start_index <= 0) {
                    start_index = 0;
                }
                pager.setStartIndex(start_index);
            }

            int page_length = PAGE_LENGTH_DEFAULT;
            if (this.pageLength != null) {
                page_length = this.pageLength.intValue();
                if (page_length <= 0) {
                    page_length = hits.length();
                }
                pager.setCountOfHitsPerPage(page_length);
            }

            return pager;
        }

        return null;
    }

    /**
     * Recycle the generator
     */
    public void recycle() {
        super.recycle();
        this.queryString = null;
        this.startIndex = null;
        this.pageLength = null;
        this.index = null;
    }

    public void dispose() {
//        if (lcs != null) {
//            this.manager.release(lcs);
//            lcs = null;
//        }
    }
}

