/* $XConsortium: CrCmap.c,v 11.12 94/04/17 20:18:58 dave Exp $ */
/*

Copyright (c) 1986  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/

#include "Xlibint.h"

Colormap XCreateColormap(dpy, w, visual, alloc)
register Display *dpy;
Window w;
Visual *visual;
int alloc;
{
    register xCreateColormapReq *req;
    Colormap mid;

    LockDisplay(dpy);
    GetReq(CreateColormap, req);
    req->window = w;
    mid = req->mid = XAllocID(dpy);
    req->alloc = alloc;
    if (visual == CopyFromParent) req->visual = CopyFromParent;
    else req->visual = visual->visualid;

    UnlockDisplay(dpy);
    SyncHandle();

    _XcmsAddCmapRec(dpy, mid, w, visual);

    return(mid);
}
