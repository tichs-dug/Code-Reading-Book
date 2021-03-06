/* -*- C++ -*- iiop_endpoints.h,v 1.3 2000/09/26 03:55:57 marina Exp */
// iiop_endpoints.h,v 1.3 2000/09/26 03:55:57 marina Exp
//
// ******  Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO and the TAO IDL Compiler have been developed by the Center for
// Distributed Object Computing at Washington University, St. Louis.
//
// Information about TAO is available at:
//                 http://www.cs.wustl.edu/~schmidt/TAO.html

#ifndef _TAO_IDL_IIOP_ENDPOINTSC_H_
#define _TAO_IDL_IIOP_ENDPOINTSC_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Managed_Types.h"
#include "tao/Sequence.h"
#include "tao/Any.h"
#include "tao/Typecode.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

struct TAO_IIOP_Endpoint_Info;
class TAO_IIOP_Endpoint_Info_var;

struct TAO_Export TAO_IIOP_Endpoint_Info
{

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_IIOP_Endpoint_Info_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static void _tao_any_destructor (void*);

  TAO_String_Manager host;
  CORBA::Short port;
  CORBA::Short priority;
};

extern TAO_Export CORBA::TypeCode_ptr  _tc_TAO_IIOP_Endpoint_Info;


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

#if !defined (__TAO_UNBOUNDED_SEQUENCE_TAO_IIOPENDPOINTSEQUENCE_CH_)
#define __TAO_UNBOUNDED_SEQUENCE_TAO_IIOPENDPOINTSEQUENCE_CH_

  class TAO_EXPORT_NESTED_MACRO _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence : public TAO_Unbounded_Base_Sequence
  {
  public:
    // = Initialization and termination methods.

    _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence (void); // Default constructor.
    _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence (CORBA::ULong maximum);
    _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence (CORBA::ULong maximum,
      CORBA::ULong length,
      TAO_IIOP_Endpoint_Info *data,
      CORBA::Boolean release = 0);
    _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence (const _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence &rhs);
    _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence &operator= (const _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence &rhs);
    virtual ~_TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence (void); // Dtor.
    // = Accessors.
    TAO_IIOP_Endpoint_Info &operator[] (CORBA::ULong i);
    const TAO_IIOP_Endpoint_Info &operator[] (CORBA::ULong i) const;
    // = Static operations.
    static TAO_IIOP_Endpoint_Info *allocbuf (CORBA::ULong size);
    static void freebuf (TAO_IIOP_Endpoint_Info *buffer);
    virtual void _allocate_buffer (CORBA::ULong length);
    virtual void _deallocate_buffer (void);
    // Implement the TAO_Base_Sequence methods (see Sequence.h)

    TAO_IIOP_Endpoint_Info *get_buffer (CORBA::Boolean orphan = 0);
    const TAO_IIOP_Endpoint_Info *get_buffer (void) const;
    void replace (CORBA::ULong max,
      CORBA::ULong length,
      TAO_IIOP_Endpoint_Info *data,
      CORBA::Boolean release);
  };

#endif /* end #if !defined */


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

#if !defined (_TAO_IIOPENDPOINTSEQUENCE_CH_)
#define _TAO_IIOPENDPOINTSEQUENCE_CH_

class TAO_IIOPEndpointSequence;
class TAO_IIOPEndpointSequence_var;

// *************************************************************
// TAO_IIOPEndpointSequence
// *************************************************************

class TAO_Export TAO_IIOPEndpointSequence : public
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_TAO_IIOPEndpointSequence
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<TAO_IIOP_Endpoint_Info>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
{
public:
  TAO_IIOPEndpointSequence (void); // default ctor
  TAO_IIOPEndpointSequence (CORBA::ULong max); // uses max size
  TAO_IIOPEndpointSequence (
    CORBA::ULong max,
    CORBA::ULong length,
    TAO_IIOP_Endpoint_Info *buffer,
    CORBA::Boolean release=0
  );
  TAO_IIOPEndpointSequence (const TAO_IIOPEndpointSequence &); // copy ctor
  ~TAO_IIOPEndpointSequence (void);
  static void _tao_any_destructor (void*);

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_IIOPEndpointSequence_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

};

#endif /* end #if !defined */


#if !defined (_TAO_IIOPENDPOINTSEQUENCE___VAR_CH_)
#define _TAO_IIOPENDPOINTSEQUENCE___VAR_CH_

// *************************************************************
// class TAO_IIOPEndpointSequence_var
// *************************************************************

class TAO_Export TAO_IIOPEndpointSequence_var
{
public:
  TAO_IIOPEndpointSequence_var (void); // default constructor
  TAO_IIOPEndpointSequence_var (TAO_IIOPEndpointSequence *);
  TAO_IIOPEndpointSequence_var (const TAO_IIOPEndpointSequence_var &); // copy constructor
  ~TAO_IIOPEndpointSequence_var (void); // destructor

  TAO_IIOPEndpointSequence_var &operator= (TAO_IIOPEndpointSequence *);
  TAO_IIOPEndpointSequence_var &operator= (const TAO_IIOPEndpointSequence_var &);
  TAO_IIOPEndpointSequence *operator-> (void);
  const TAO_IIOPEndpointSequence *operator-> (void) const;

  operator const TAO_IIOPEndpointSequence &() const;
  operator TAO_IIOPEndpointSequence &();
  operator TAO_IIOPEndpointSequence &() const;
  operator TAO_IIOPEndpointSequence *&(); // variable-size base types only

  TAO_IIOP_Endpoint_Info &operator[] (CORBA::ULong index);
  // in, inout, out, _retn
  const TAO_IIOPEndpointSequence &in (void) const;
  TAO_IIOPEndpointSequence &inout (void);
  TAO_IIOPEndpointSequence *&out (void);
  TAO_IIOPEndpointSequence *_retn (void);
  TAO_IIOPEndpointSequence *ptr (void) const;

private:
  TAO_IIOPEndpointSequence *ptr_;
};


#endif /* end #if !defined */


extern TAO_Export CORBA::TypeCode_ptr  _tc_TAO_IIOPEndpointSequence;

#ifndef __ACE_INLINE__

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const TAO_IIOP_Endpoint_Info &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, TAO_IIOP_Endpoint_Info &);

#if !defined _TAO_CDR_OP_TAO_IIOPEndpointSequence_H_
#define _TAO_CDR_OP_TAO_IIOPEndpointSequence_H_

TAO_Export CORBA::Boolean operator<< (
    TAO_OutputCDR &,
    const TAO_IIOPEndpointSequence &
  );
TAO_Export CORBA::Boolean operator>> (
    TAO_InputCDR &,
    TAO_IIOPEndpointSequence &
  );

#endif /* _TAO_CDR_OP_TAO_IIOPEndpointSequence_H_ */


#endif /* __ACE_INLINE__ */


#if defined (__ACE_INLINE__)
#include "iiop_endpoints.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* ifndef */
