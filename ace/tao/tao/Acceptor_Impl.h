// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Acceptor_Impl.h
 *
 *  Acceptor_Impl.h,v 1.13 2000/12/15 20:06:46 coryan Exp
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>Ossama Othman <othman@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ACCEPTOR_IMPL_H
#define TAO_ACCEPTOR_IMPL_H
#include "ace/pre.h"

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"

// Forward declarations.
class TAO_ORB_Core;

/**
 * @class TAO_Creation_Strategy
 *
 * @brief Creates a Svc_Handler and set the ORB_Core pointer on it.
 */
template <class SVC_HANDLER>
class TAO_Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
public:
  /**
   * Constructor. <arg> parameter is used to pass any special
   * state/info to the service handler upon creation.  Currently used
   * by IIOP and UIOP to pass protocol configuration properties.
   */
  TAO_Creation_Strategy (TAO_ORB_Core *orb_core,
                         void *arg = 0,
                         CORBA::Boolean flag = 0);

  /// Create a SVC_HANDLER  and set the ORB_Core pointer on it.
  int make_svc_handler (SVC_HANDLER *&sh);

protected:
  /// Pointer to the ORB Core.
  TAO_ORB_Core *orb_core_;

  /// Some info/state to be passed to the service handler we create.
  void *arg_;

  /// Should we use the Lite version for any protocol?
  CORBA::Boolean lite_flag_;
};

/**
 * @class TAO_Concurrency_Strategy
 *
 * @brief Activates the Svc_Handler, and then if specified by the
 * TAO_Server_Strategy_Factory, it activates the Svc_Handler to
 * run in its own thread.
 */
template <class SVC_HANDLER>
class TAO_Concurrency_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
{
public:
  /// Constructor.
  TAO_Concurrency_Strategy (TAO_ORB_Core *orb_core);

  /**
   * Activates the Svc_Handler, and then if specified by the
   * TAO_Server_Strategy_Factory, it activates the Svc_Handler to run
   * in its own thread.
   */
  int activate_svc_handler (SVC_HANDLER *svc_handler,
                            void *arg);

protected:
  /// Pointer to the ORB Core.
  TAO_ORB_Core *orb_core_;
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class TAO_Accept_Strategy : public ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:

  /// Constructor.
  TAO_Accept_Strategy (TAO_ORB_Core *orb_core);

  /// Initialize the <peer_acceptor_> with <local_addr>.  If the
  /// process runs out of handles, purge some "old" connections.
  int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
            int restart = 0);

  /// Delegates to the <accept> method of the PEER_ACCEPTOR. If the
  /// process runs out of handles, purge some "old" connections.
  int accept_svc_handler (SVC_HANDLER *svc_handler);

protected:
#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  /// Handler which deals with purging "old" connections.
  int out_of_sockets_handler (void);
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  /// Base class.
  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY_BASE;

  /// Pointer to the ORB Core.
  TAO_ORB_Core *orb_core_;
};

#if defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Acceptor_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Acceptor_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_ACCEPTOR_IMPL_H */
