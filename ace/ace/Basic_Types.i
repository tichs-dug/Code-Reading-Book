/* -*- C++ -*- */
// Basic_Types.i,v 4.16 2001/09/15 13:18:11 schmidt Exp

#if defined (ACE_LACKS_LONGLONG_T)

ACE_INLINE
ACE_U_LongLong::ACE_U_LongLong (const ACE_UINT32 lo, const ACE_UINT32 hi)
{
  h_ () = hi;
  l_ () = lo;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::hi (void) const
{
  return h_ ();
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::lo (void) const
{
  return l_ ();
}

ACE_INLINE void
ACE_U_LongLong::hi (const ACE_UINT32 hi)
{
  h_ () = hi;
}

ACE_INLINE void
ACE_U_LongLong::lo (const ACE_UINT32 lo)
{
  l_ () = lo;
}

ACE_INLINE
ACE_U_LongLong::~ACE_U_LongLong (void)
{
}

ACE_INLINE int
ACE_U_LongLong::operator== (const ACE_U_LongLong &n) const
{
  return h_ () == n.h_ ()  &&  l_ () == n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator== (const ACE_UINT32 n) const
{
  return h_ () == 0  &&  l_ () == n;
}

ACE_INLINE int
ACE_U_LongLong::operator!= (const ACE_U_LongLong &n) const
{
  return ! (*this == n);
}

ACE_INLINE int
ACE_U_LongLong::operator!= (const ACE_UINT32 n) const
{
  return ! (*this == n);
}

ACE_INLINE int
ACE_U_LongLong::operator< (const ACE_U_LongLong &n) const
{
  return h_ () < n.h_ () ? 1
                         : h_ () > n.h_ () ? 0
                                           : l_ () < n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator< (const ACE_UINT32 n) const
{
  return operator< (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE int
ACE_U_LongLong::operator<= (const ACE_U_LongLong &n) const
{
  return h_ () < n.h_ () ? 1
                         : h_ () > n.h_ () ? 0
                                           : l_ () <= n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator<= (const ACE_UINT32 n) const
{
  return operator<= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE int
ACE_U_LongLong::operator> (const ACE_U_LongLong &n) const
{
  return h_ () > n.h_ () ? 1
                         : h_ () < n.h_ () ? 0
                                           : l_ () > n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator> (const ACE_UINT32 n) const
{
  return operator> (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE int
ACE_U_LongLong::operator>= (const ACE_U_LongLong &n) const
{
  return h_ () > n.h_ () ? 1
                         : h_ () < n.h_ () ? 0
                                           : l_ () >= n.l_ ();
}

ACE_INLINE int
ACE_U_LongLong::operator>= (const ACE_UINT32 n) const
{
  return operator>= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE
ACE_U_LongLong::ACE_U_LongLong (const ACE_U_LongLong &n)
{
  h_ () = n.h_ ();
  l_ () = n.l_ ();
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator= (const ACE_U_LongLong &n)
{
  h_ () = n.h_ ();
  l_ () = n.l_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator+ (const ACE_U_LongLong &n) const
{
  ACE_U_LongLong ret (l_ () + n.l_ (), h_ () + n.h_ ());
  if (ret.l_ () < n.l_ ()) /* carry */ ++ret.h_ ();

  return ret;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator+ (const ACE_UINT32 n) const
{
  return operator+ (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator- (const ACE_U_LongLong &n) const
{
  ACE_U_LongLong ret (l_ () - n.l_ (), h_ () - n.h_ ());
  if (l_ () < n.l_ ()) /* borrow */ --ret.h_ ();

  return ret;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator- (const ACE_UINT32 n) const
{
  return operator- (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator<< (const u_int n) const
{
  const ACE_UINT32 carry_mask = l_ () >> (32 - n);
  ACE_U_LongLong ret (n < 32  ?  l_ () << n  :  0,
                      n < 32  ?  (h_ () << n) | carry_mask  :  carry_mask);

  return ret;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator<<= (const u_int n)
{
  const ACE_UINT32 carry_mask = l_ () >> (32 - n);
  h_ () = n < 32  ?  (h_ () << n) | carry_mask  :  carry_mask;

  // g++ 2.7.2.3/Solaris 2.5.1 doesn't modify l_ () if shifted by 32.
  l_ () = n < 32  ?  l_ () << n  :  0;

  return *this;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator>> (const u_int n) const
{
  const ACE_UINT32 carry_mask = h_ () << (32 - n);
  ACE_U_LongLong ret (n < 32  ?  (l_ () >> n) | carry_mask  :  0,
                      n < 32  ?  h_ () >> n  :  0);

  return ret;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator>>= (const u_int n)
{
  const ACE_UINT32 carry_mask = h_ () << (32 - n);
  l_ () = n < 32  ?  (l_ () >> n) | carry_mask  :  carry_mask;
  h_ () = n < 32  ?  h_ () >> n  :  0;

  return *this;
}

ACE_INLINE double
ACE_U_LongLong::operator/ (const double n) const
{
  // See the derivation above in operator/ (const ACE_UINT32).

  return ((double) 0xffffffffu - n + 1.0) / n * h_ ()  +
         (double) h_ ()  +  (double) l_ () / n;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator+= (const ACE_U_LongLong &n)
{
  h_ () += n.h_ ();
  l_ () += n.l_ ();
  if (l_ () < n.l_ ()) /* carry */ ++h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator+= (const ACE_UINT32 n)
{
  return operator+= (ACE_static_cast (const ACE_U_LongLong, n));
}

#define ACE_HIGHBIT (~(~0UL >> 1))

ACE_INLINE ACE_UINT32
ACE_U_LongLong::ul_shift (ACE_UINT32 a, ACE_UINT32 c_in, ACE_UINT32 *c_out) const
{
  const ACE_UINT32 b = (a << 1) | c_in;
  *c_out = (*c_out << 1) + ((a & ACE_HIGHBIT) > 0);

  return b;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_shift (ACE_U_LongLong a, 
                           ACE_UINT32 c_in,
                           ACE_UINT32 *c_out) const
{
  ACE_U_LongLong b;

  b.l_ () = (a.l_ () << 1) | c_in;
  c_in = ((a.l_ () & ACE_HIGHBIT) > 0);
  b.h_ () = (a.h_ () << 1) | c_in;
  *c_out = (*c_out << 1) + ((a.h_ () & ACE_HIGHBIT) > 0);

  return b;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_add (ACE_U_LongLong a, ACE_U_LongLong b, ACE_UINT32 *carry) const
{
  ACE_U_LongLong r (0, 0);
  ACE_UINT32 c1, c2, c3, c4;

  c1 = a.l_ () % 2;
  c2 = b.l_ () % 2;
  c3 = 0;

  r.l_ () = a.l_ ()/2 +  b.l_ ()/2 + (c1+c2)/2;
  r.l_ () = ul_shift (r.l_ (), (c1+c2)%2, &c3);

  c1 = a.h_ () % 2;
  c2 = b.h_ () % 2;
  c4 = 0;

  r.h_ () = a.h_ ()/2 + b.h_ ()/2 + (c1+c2+c3)/2;
  r.h_ () = ul_shift (r.h_ (), (c1+c2+c3)%2, &c4);

  *carry = c4;

  return r;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::ull_mult (ACE_U_LongLong a, ACE_UINT32 b, ACE_UINT32 *carry) const
{
  register ACE_UINT32 mask = ACE_HIGHBIT;
  const ACE_U_LongLong zero (0, 0);
  ACE_U_LongLong accum (0, 0);
  ACE_UINT32 c;

  *carry = 0;
  if (b > 0)
    do
      {
        accum = ull_shift (accum, 0U, carry);
        if (b & mask)
          accum = ull_add (accum, a, &c);
        else
          accum = ull_add (accum, zero, &c);
        *carry += c;
        mask >>= 1;
      }
    while (mask > 0);

  return accum;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator* (const ACE_UINT32 n) const
{
  ACE_UINT32 carry;  // will throw the carry away

  return ull_mult (*this, n, &carry);
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator*= (const ACE_UINT32 n)
{
  ACE_UINT32 carry;  // will throw the carry away

  return *this = ull_mult (*this, n, &carry);
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-= (const ACE_U_LongLong &n)
{
  h_ () -= n.h_ ();
  if (l_ () < n.l_ ()) /* borrow */ --h_ ();
  l_ () -= n.l_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-= (const ACE_UINT32 n)
{
  return operator-= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator++ ()
{
  ++l_ ();
  if (l_ () == 0) /* carry */ ++h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator-- ()
{
  if (l_ () == 0) /* borrow */ --h_ ();
  --l_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator|= (const ACE_U_LongLong n)
{
  l_ () |= n.l_ ();
  h_ () |= n.h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator|= (const ACE_UINT32 n)
{
  return operator|= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator&= (const ACE_U_LongLong n)
{
  l_ () &= n.l_ ();
  h_ () &= n.h_ ();

  return *this;
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator&= (const ACE_UINT32 n)
{
  return operator&= (ACE_static_cast (const ACE_U_LongLong, n));
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const ACE_UINT32 n) const
{
  // This takes advantage of the fact that the return type has only 32
  // bits.  Replace 0x100000000 with 0xffffffff + 1 because the former
  // has 33 bits.
  // Quotient = (0x100000000u * hi_ + lo_) / n
  //          = ((0x100000000u - n + n) * hi_ + lo_) / n
  //          = ((0x100000000u - n) / n * hi_  +  hi_ * n / n  +  lo_ / n
  //          = (0x100000000u - n) / n * hi_ +  hi_  +  lo_ / n
  //          = (0xffffffffu - n + 1) / n * hi_ +  hi_  +  lo_ / n

  return (0xffffffffu - n + 1) / n * h_ ()  +  h_ ()  +  l_ () / n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator% (const ACE_UINT32 n) const
{
  // Because the argument is an ACE_UINT32, the result can never be
  // bigger than 32 bits.  Replace 0x100000000 with 0xffffffff + 1
  // because the former has 33 bits.
  // Mod = (0x100000000u * hi_ + lo_) % n
  //     = (0x100000000u % n * hi_  +  lo_ % n) % n
  //     = ((0x100000000u - n) % n * hi_  +  lo_ % n) % n
  //     = ((0xffffffffu - n + 1) % n * hi_  +  lo_ % n) % n

  return ((0xffffffff - n + 1)  % n * h_ ()  +  l_ () % n) % n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator| (const ACE_INT32 n) const
{
  return l_ () | n;
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator& (const ACE_INT32 n) const
{
  return l_ () & n;
}

ACE_INLINE ACE_U_LongLong
ACE_U_LongLong::operator* (const ACE_INT32 n) const
{
  return operator* ((ACE_UINT32) n);
}

ACE_INLINE ACE_U_LongLong &
ACE_U_LongLong::operator*= (const ACE_INT32 n)
{
  return operator*= ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const ACE_INT32 n) const
{
  return operator/ ((ACE_UINT32) n);
}

#if ACE_SIZEOF_INT == 4
ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const u_long n) const
{
  return operator/ ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const long n) const
{
  return operator/ ((ACE_UINT32) n);
}

#else  /* ACE_SIZEOF_INT != 4 */
ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const u_int n) const
{
  return operator/ ((ACE_UINT32) n);
}

ACE_INLINE ACE_UINT32
ACE_U_LongLong::operator/ (const int n) const
{
  return operator/ ((ACE_UINT32) n);
}
#endif /* ACE_SIZEOF_INT != 4 */

#endif /* ACE_LACKS_LONGLONG_T */
