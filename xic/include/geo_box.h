
/*========================================================================*
 *                                                                        *
 *  Distributed by Whiteley Research Inc., Sunnyvale, California, USA     *
 *                       http://wrcad.com                                 *
 *  Copyright (C) 2017 Whiteley Research Inc., all rights reserved.       *
 *  Author: Stephen R. Whiteley, except as indicated.                     *
 *                                                                        *
 *  As fully as possible recognizing licensing terms and conditions       *
 *  imposed by earlier work from which this work was derived, if any,     *
 *  this work is released under the Apache License, Version 2.0 (the      *
 *  "License").  You may not use this file except in compliance with      *
 *  the License, and compliance with inherited licenses which are         *
 *  specified in a sub-header below this one if applicable.  A copy       *
 *  of the License is provided with this distribution, or you may         *
 *  obtain a copy of the License at                                       *
 *                                                                        *
 *        http://www.apache.org/licenses/LICENSE-2.0                      *
 *                                                                        *
 *  See the License for the specific language governing permissions       *
 *  and limitations under the License.                                    *
 *                                                                        *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      *
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-        *
 *   INFRINGEMENT.  IN NO EVENT SHALL WHITELEY RESEARCH INCORPORATED      *
 *   OR STEPHEN R. WHITELEY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER     *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE       *
 *   USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                        *
 *========================================================================*
 *               XicTools Integrated Circuit Design System                *
 *                                                                        *
 * Xic Integrated Circuit Layout and Schematic Editor                     *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

#ifndef GEO_BOX_H
#define GEO_BOX_H

#include "geo_memmgr_cfg.h"
#include "geo_point.h"
#include "cd_const.h"


struct Blist;
struct BYlist;
struct Poly;
struct Wire;

// bounding box element
struct BBox
{
    BBox() { left = 0; bottom = 0; right = 0; top = 0; }
    BBox(int l, int b, int r, int t)
        { left = l; bottom = b; right = r; top = t; }

    // From 4-sided polygon point list.
    BBox(const Point *p)
        { left = p[0].x; bottom = p[0].y; right = p[2].x; top = p[2].y;
           fix(); }

    BBox(const Point *p, int npts)
        {
            if (p && npts > 0) {
                left = right = p->x;
                bottom = top = p->y;
                for (int i = 1; i < npts; i++)
                    add(p[i].x, p[i].y);
            }
            else {
                left = right = 0;
                bottom = top = 0;
            }
        }

    // Init to intersect area, negative width or height if none.
    BBox(const BBox &BB1, const BBox &BB2)
        {
            left = mmMax(BB1.left, BB2.left);
            bottom = mmMax(BB1.bottom, BB2.bottom);
            right = mmMin(BB1.right, BB2.right);
            top = mmMin(BB1.top, BB2.top);
        }

    bool valid() const { return (right > left && top > bottom); }

    int width()  const { return (right - left); }

    // These assume top > bottom as is true for window coords.
    int height() const { return (top - bottom); }
    int max_wh() const
        {
            int w = width();
            int h = height();
            return (w >= h ? w : h);
        }

    bool operator==(const BBox &BB) const
        { return (left == BB.left && bottom == BB.bottom &&
        right == BB.right && top == BB.top); }
    bool operator!=(const BBox &BB) const
        { return (left != BB.left || bottom != BB.bottom ||
        right != BB.right || top != BB.top); }

    // This is enclosed in BB, not touching edge
    bool operator<(const BBox &BB) const
        { return (left > BB.left && bottom > BB.bottom &&
        right < BB.right && top < BB.top); }

    // This is enclosed in BB, touching edge ok
    bool operator<=(const BBox &BB) const
        { return (left >= BB.left && bottom >= BB.bottom &&
        right <= BB.right && top <= BB.top); }

    // This encloses BB, edges not touching
    bool operator>(const BBox &BB) const
        { return (left < BB.left && bottom < BB.bottom &&
        right > BB.right && top > BB.top); }

    // This encloses BB, edges touching ok
    bool operator>=(const BBox &BB) const
        { return (left <= BB.left && bottom <= BB.bottom &&
        right >= BB.right && top >= BB.top); }

    double aspect() const
        { double t = (right - left)/(double)(top - bottom);
        return (t < 0 ? -t : t); }

    void to_path(Point *p) const
        {
            p[0].set(left, bottom);
            p[1].set(left, top);
            p[2].set(right, top);
            p[3].set(right, bottom);
            p[4] = p[0];
        }

    // These methods assume top >= bottom!

    void add(const BBox *BB)
        {
            if (left > BB->left)
                left = BB->left;
            if (bottom > BB->bottom)
                bottom = BB->bottom;
            if (right < BB->right)
                right = BB->right;
            if (top < BB->top)
                top = BB->top;
        }

    void add(int x, int y)
        {
            if (x < left)
                left = x;
            if (x > right)
                right = x;
            if (y < bottom)
                bottom = y;
            if (y > top)
                top = y;
        }

    int addc(const BBox *BB)
        {
            int s = 0;
            if (left > BB->left) {
                left = BB->left;
                s++;
            }
            if (bottom > BB->bottom) {
                bottom = BB->bottom;
                s++;
            }
            if (right < BB->right) {
                right = BB->right;
                s++;
            }
            if (top < BB->top) {
                top = BB->top;
                s++;
            }
            return (s);
        }

    bool intersect(const Point* px, bool touchok) const
        {
            if (touchok) {
                if (px->x >= left && px->x <= right &&
                        px->y >= bottom && px->y <= top)
                    return (true);
            }
            else {
                if (px->x > left && px->x < right &&
                        px->y > bottom && px->y < top)
                    return (true);
            }
            return (false);
        }

    bool intersect(int x, int y, bool touchok) const
        {
            if (touchok) {
                if (x >= left && x <= right && y >= bottom && y <= top)
                    return (true);
            }
            else {
                if (x > left && x < right && y > bottom && y < top)
                    return (true);
            }
            return (false);
        }
    bool isect_i(const BBox *BB) const    // touching ok
        { return (right >= BB->left && BB->right >= left &&
            top >= BB->bottom && BB->top >= bottom); }

    bool isect_x(const BBox *BB) const    // touching not ok
        { return (right > BB->left && BB->right > left &&
            top > BB->bottom && BB->top > bottom); }

    bool intersect(const BBox *BB, bool touchok) const
        { return ((bool)(touchok ? isect_i(BB) : isect_x(BB))); }

    void bloat(int delta) { left -= delta; bottom -= delta;
        right += delta; top += delta; }
    double area() const { return ((((double)(right-left))*(top-bottom))/
        (CDphysResolution*CDphysResolution)); }

    // Swap values if necessary.
    void fix()
        {
            if (left > right) { int t = left; left = right; right = t; }
            if (bottom > top) { int t = bottom; bottom = top; top = t; }
        }

    void fixLR()
        {
            if (left > right) { int t = left; left = right; right = t; }
        }

    void fixBT()
        {
            if (bottom > top) { int t = bottom; bottom = top; top = t; }
        }

    void print(const char *s = 0) const
        {
            if (s)
                printf("%s %d %d %d %d\n", s, left, bottom, right, top);
            else
                printf("%d %d %d %d\n", left, bottom, right, top);
        }

    void scale(double sc)
        {
            if (sc != 1.0) {
                left = mmRnd(left*sc);
                bottom = mmRnd(bottom*sc);
                right = mmRnd(right*sc);
                top = mmRnd(top*sc);
            }
        }

    void set_cif(int, int, int, int);
    bool intersect(const Poly*, bool) const;
    bool intersect(const Wire*, bool) const;
    Blist *clip_to(const BBox*) const;
    Blist *clip_out(const BBox*, bool* = 0) const;
    Blist *clip_out(const Blist*) const;
    Blist *clip_out(const BYlist*) const;
    Blist *clip_merge(const BBox*) const;
    void scale(double, int, int);

    int left, bottom, right, top;
};

// List of boxes
struct Blist
{
#ifdef GEO_USE_MANAGER
    void *operator new(size_t);
    void operator delete(void*, size_t);
#endif
    Blist() { next = 0; }
    Blist(const BBox *b, Blist *n = 0) { next = n; BB = *b; }
    Blist(int l, int b, int r, int t, Blist *bn = 0) : BB(l, b, r, t)
        { next = bn; }

    static void wtov(Blist *thisbl)
        {
            for (Blist *b = thisbl; b; b = b->next) {
                int t = b->BB.top;
                b->BB.top = b->BB.bottom;
                b->BB.bottom = t;
            }
        }

    static void destroy(Blist *bl)
        {
            while (bl) {
                Blist *bx = bl;
                bl = bl->next;
                delete bx;
            }
        }

    static int length(const Blist *thisbl)
        {
            int cnt = 0;
            const Blist *bl = thisbl;
            while (bl) {
                cnt++;
                bl = bl->next;
            }
            return (cnt);
        }

    static double area(const Blist *thisbl)
        {
            double d = 0.0;
            for (const Blist *bl = thisbl; bl; bl = bl->next)
                d += bl->BB.area();
            return (d);
        }

    // geo_box.cc
    static Blist *sort(Blist*, int = 0);
    static Blist *insert_merge(Blist*, const BBox*);
    static Blist *merge(Blist*);
    static Blist *clip_out(Blist*, const BBox*);
    static Blist *clip_out(Blist*, const Blist*);
    static Blist *clip_to(Blist*, const BBox*);
    static Blist *clip_to(Blist*, const Blist*);
    static bool intersect(const Blist*, const BBox*, bool);

    Blist *next;
    BBox BB;
};

struct BYlist
{
#ifdef GEO_USE_MANAGER
    void *operator new(size_t);
    void operator delete(void*, size_t);
#endif
    BYlist(Blist*, bool = false);
    ~BYlist() { Blist::destroy(blist); }

    static void destroy(BYlist *y) {
        while (y) {
            BYlist *yx = y;
            y = y->next;
            delete yx;
        }
    }

    static BYlist *insert_merge(BYlist*, const BBox *BB);
    static BYlist *merge(BYlist*);
    static BYlist *clip_out(BYlist*, const BBox*);
    static BYlist *insert(BYlist*, Blist*);
    static Blist *toblist(BYlist*);

private:
    // ZTST ok
    BYlist *trymerge(const BBox*, Blist**);
    bool merge_rows();
    bool merge_cols();
    BYlist *clip();
    BYlist *first(Blist**);

public:
    BYlist *next;       // next->yu < yu
    Blist *blist;       // list of boxes, each have top == yu
    int yu;             // top of boxes
    int yl;             // minimum bottom in blist
};

#endif

