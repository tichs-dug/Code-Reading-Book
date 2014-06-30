// -*- C++ -*-
//
// ImplRepoS.h,v 1.7 2001/08/02 01:19:48 coryan Exp

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

#ifndef _TAO_IDL_ORIG_IMPLREPOS_H_
#define _TAO_IDL_ORIG_IMPLREPOS_H_

#include "ace/pre.h"
#include "ImplRepoC.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/Collocated_Object.h"
#include "tao/PortableServer/ThruPOA_Object_Proxy_Impl.h"
#include "tao/PortableServer/Direct_Object_Proxy_Impl.h"
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option push -w-rvl -w-rch -w-ccc -w-inl
#endif /* __BORLANDC__ */

TAO_NAMESPACE  POA_ImplementationRepository
{
  class ServerObject;
  typedef ServerObject *ServerObject_ptr;
  // Forward Classes Declaration
  class _TAO_ServerObject_ThruPOA_Proxy_Impl;
  class _TAO_ServerObject_Direct_Proxy_Impl;
  class _TAO_ServerObject_Strategized_Proxy_Broker;
  
  class TAO_PortableServer_Export ServerObject
    : public virtual PortableServer::ServantBase
  {
  protected:
    ServerObject (void);

  public:
    ServerObject (const ServerObject& rhs);
    virtual ~ServerObject (void);


    virtual CORBA::Boolean _is_a (
        const char* logical_type_id,
        CORBA::Environment &ACE_TRY_ENV = 
          TAO_default_environment ()
      );

    virtual void* _downcast (
        const char* logical_type_id
      );

    static void _is_a_skel (
        TAO_ServerRequest &req,
        void *obj,
        void *servant_upcall,
        CORBA::Environment &ACE_TRY_ENV
      );

    static void _non_existent_skel (
        TAO_ServerRequest &req,
        void *obj,
        void *servant_upcall,
        CORBA::Environment &ACE_TRY_ENV
      );

    static void _interface_skel (
        TAO_ServerRequest &req,
        void *obj,
        void *servant_upcall,
        CORBA::Environment &ACE_TRY_ENV
      );

    virtual void _dispatch (
        TAO_ServerRequest &req,
        void *_servant_upcall,
        CORBA::Environment &ACE_TRY_ENV
      );

    ::ImplementationRepository::ServerObject *_this (
        CORBA::Environment &ACE_TRY_ENV = 
          TAO_default_environment ()
      );

    virtual const char* _interface_repository_id (void) const;

    virtual void ping (
        CORBA::Environment &ACE_TRY_ENV = 
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;
    
        static void ping_skel (
        TAO_ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_servant_upcall,
        CORBA::Environment &ACE_TRY_ENV
      );

    virtual void shutdown (
        CORBA::Environment &ACE_TRY_ENV = 
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;
    
        static void shutdown_skel (
        TAO_ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_servant_upcall,
        CORBA::Environment &ACE_TRY_ENV
      );


  };

  
  ///////////////////////////////////////////////////////////////////////
  //               Strategized Proxy Broker Declaration 
  //
  
  class TAO_PortableServer_Export _TAO_ServerObject_Strategized_Proxy_Broker : public virtual ::ImplementationRepository::_TAO_ServerObject_Proxy_Broker
  {
  public: 
    _TAO_ServerObject_Strategized_Proxy_Broker (void);
    
    virtual ~_TAO_ServerObject_Strategized_Proxy_Broker (void);
    
    virtual ::ImplementationRepository::_TAO_ServerObject_Proxy_Impl &select_proxy (
      ::ImplementationRepository::ServerObject *object,
      CORBA_Environment &ACE_TRY_ENV
    );
  
  private:
    // Helper methods that takes care to create the proxy
    // as soon as their use is necessary.
    void create_proxy (
      int collocation_strategy,
      CORBA::Environment &ACE_TRY_ENV
    );
    
    // Caches the proxy implementations. The proxy implementation
    // are totally stateless, and those can be shared by all the
    // instances of a given IDL interface type.
    ::ImplementationRepository::_TAO_ServerObject_Proxy_Impl
    *proxy_cache_[TAO_Collocation_Strategies::CS_LAST];
    
    TAO_SYNCH_MUTEX mutex_;
    // This funxtion is used to get an handle to the unique instance
    // of the Strategized Proxy Broker that is available for a given
    // interface.
  
  public:
    static _TAO_ServerObject_Strategized_Proxy_Broker *the_TAO_ServerObject_Strategized_Proxy_Broker (void);
  };
  
  
  //
  //            End Strategized Proxy Broker Declaration 
  ///////////////////////////////////////////////////////////////////////
  
  
  ///////////////////////////////////////////////////////////////////////
  //                    ThruPOA  Impl. Declaration
  //
  
  class TAO_PortableServer_Export _TAO_ServerObject_ThruPOA_Proxy_Impl : 
    public virtual ::ImplementationRepository::_TAO_ServerObject_Proxy_Impl,
    public virtual TAO_ThruPOA_Object_Proxy_Impl
  {
  public:
    _TAO_ServerObject_ThruPOA_Proxy_Impl (void);
    
    virtual ~_TAO_ServerObject_ThruPOA_Proxy_Impl (void) { }
    
    virtual void ping (
        CORBA_Object *_collocated_tao_target_,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    virtual void shutdown (
        CORBA_Object *_collocated_tao_target_,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    
  };
  
  //
  //                ThruPOA  Proxy Impl. Declaration
  ///////////////////////////////////////////////////////////////////////
  
  
///////////////////////////////////////////////////////////////////////
//                    Direct  Impl. Declaration
//

class TAO_PortableServer_Export _TAO_ServerObject_Direct_Proxy_Impl : 
  public virtual ImplementationRepository::_TAO_ServerObject_Proxy_Impl,
  public virtual TAO_Direct_Object_Proxy_Impl
{
public:
  _TAO_ServerObject_Direct_Proxy_Impl (void);
  
  virtual ~_TAO_ServerObject_Direct_Proxy_Impl (void) { }
  
  virtual void ping (
      CORBA_Object *_collocated_tao_target_,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual void shutdown (
      CORBA_Object *_collocated_tao_target_,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  };


//
//                Direct  Proxy Impl. Declaration
///////////////////////////////////////////////////////////////////////


class Administration;
typedef Administration *Administration_ptr;
// Forward Classes Declaration
class _TAO_Administration_ThruPOA_Proxy_Impl;
class _TAO_Administration_Direct_Proxy_Impl;
class _TAO_Administration_Strategized_Proxy_Broker;

class TAO_PortableServer_Export Administration
  : public virtual PortableServer::ServantBase
{
protected:
  Administration (void);

public:
  Administration (const Administration& rhs);
  virtual ~Administration (void);


  virtual CORBA::Boolean _is_a (
      const char* logical_type_id,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );

  virtual void* _downcast (
      const char* logical_type_id
    );

  static void _is_a_skel (
      TAO_ServerRequest &req,
      void *obj,
      void *servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  static void _non_existent_skel (
      TAO_ServerRequest &req,
      void *obj,
      void *servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  static void _interface_skel (
      TAO_ServerRequest &req,
      void *obj,
      void *servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void _dispatch (
      TAO_ServerRequest &req,
      void *_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  ::ImplementationRepository::Administration *_this (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );

  virtual const char* _interface_repository_id (void) const;

  virtual void activate_server (
      const char * server,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound,
      ImplementationRepository::Administration::CannotActivate
    )) = 0;
  
    static void activate_server_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void register_server (
      const char * server,
      const ImplementationRepository::StartupOptions & options,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::AlreadyRegistered
    )) = 0;
  
    static void register_server_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void reregister_server (
      const char * server,
      const ImplementationRepository::StartupOptions & options,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    )) = 0;
  
    static void reregister_server_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void remove_server (
      const char * server,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    )) = 0;
  
    static void remove_server_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void shutdown_server (
      const char * server,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    )) = 0;
  
    static void shutdown_server_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual char * server_is_running (
      const char * server,
      const char * addr,
      ImplementationRepository::ServerObject_ptr server_object,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    )) = 0;
  
    static void server_is_running_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void server_is_shutting_down (
      const char * server,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    )) = 0;
  
    static void server_is_shutting_down_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void find (
      const char * server,
      ImplementationRepository::ServerInformation_out info,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    )) = 0;
  
    static void find_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void list (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      ImplementationRepository::ServerInformationIterator_out server_iterator,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    )) = 0;
  
    static void list_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );


};


///////////////////////////////////////////////////////////////////////
//               Strategized Proxy Broker Declaration 
//

class TAO_PortableServer_Export _TAO_Administration_Strategized_Proxy_Broker : public virtual ::ImplementationRepository::_TAO_Administration_Proxy_Broker
{
public: 
  _TAO_Administration_Strategized_Proxy_Broker (void);
  
  virtual ~_TAO_Administration_Strategized_Proxy_Broker (void);
  
  virtual ::ImplementationRepository::_TAO_Administration_Proxy_Impl &select_proxy (
    ::ImplementationRepository::Administration *object,
    CORBA_Environment &ACE_TRY_ENV
  );

private:
  // Helper methods that takes care to create the proxy
  // as soon as their use is necessary.
  void create_proxy (
    int collocation_strategy,
    CORBA::Environment &ACE_TRY_ENV
  );
  
  // Caches the proxy implementations. The proxy implementation
  // are totally stateless, and those can be shared by all the
  // instances of a given IDL interface type.
  ::ImplementationRepository::_TAO_Administration_Proxy_Impl
  *proxy_cache_[TAO_Collocation_Strategies::CS_LAST];
  
  TAO_SYNCH_MUTEX mutex_;
  // This funxtion is used to get an handle to the unique instance
  // of the Strategized Proxy Broker that is available for a given
  // interface.

public:
  static _TAO_Administration_Strategized_Proxy_Broker *the_TAO_Administration_Strategized_Proxy_Broker (void);
};


//
//            End Strategized Proxy Broker Declaration 
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//                    ThruPOA  Impl. Declaration
//

class TAO_PortableServer_Export _TAO_Administration_ThruPOA_Proxy_Impl : 
  public virtual ::ImplementationRepository::_TAO_Administration_Proxy_Impl,
  public virtual TAO_ThruPOA_Object_Proxy_Impl
{
public:
  _TAO_Administration_ThruPOA_Proxy_Impl (void);
  
  virtual ~_TAO_Administration_ThruPOA_Proxy_Impl (void) { }
  
  virtual void activate_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound,
      ImplementationRepository::Administration::CannotActivate
    ));
  
  virtual void register_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      const ImplementationRepository::StartupOptions & options,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::AlreadyRegistered
    ));
  
  virtual void reregister_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      const ImplementationRepository::StartupOptions & options,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual void remove_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void shutdown_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual char * server_is_running (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      const char * addr,
      ImplementationRepository::ServerObject_ptr server_object,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void server_is_shutting_down (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void find (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      ImplementationRepository::ServerInformation_out info,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void list (
      CORBA_Object *_collocated_tao_target_,
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      ImplementationRepository::ServerInformationIterator_out server_iterator,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  
};

//
//                ThruPOA  Proxy Impl. Declaration
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//                    Direct  Impl. Declaration
//

class TAO_PortableServer_Export _TAO_Administration_Direct_Proxy_Impl : 
  public virtual ImplementationRepository::_TAO_Administration_Proxy_Impl,
  public virtual TAO_Direct_Object_Proxy_Impl
{
public:
  _TAO_Administration_Direct_Proxy_Impl (void);
  
  virtual ~_TAO_Administration_Direct_Proxy_Impl (void) { }
  
  virtual void activate_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound,
      ImplementationRepository::Administration::CannotActivate
    ));
  
  virtual void register_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      const ImplementationRepository::StartupOptions & options,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::AlreadyRegistered
    ));
  
  virtual void reregister_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      const ImplementationRepository::StartupOptions & options,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual void remove_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void shutdown_server (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual char * server_is_running (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      const char * addr,
      ImplementationRepository::ServerObject_ptr server_object,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void server_is_shutting_down (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void find (
      CORBA_Object *_collocated_tao_target_,
      const char * server,
      ImplementationRepository::ServerInformation_out info,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      ImplementationRepository::Administration::NotFound
    ));
  
  virtual void list (
      CORBA_Object *_collocated_tao_target_,
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      ImplementationRepository::ServerInformationIterator_out server_iterator,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  };


//
//                Direct  Proxy Impl. Declaration
///////////////////////////////////////////////////////////////////////


class ServerInformationIterator;
typedef ServerInformationIterator *ServerInformationIterator_ptr;
// Forward Classes Declaration
class _TAO_ServerInformationIterator_ThruPOA_Proxy_Impl;
class _TAO_ServerInformationIterator_Direct_Proxy_Impl;
class _TAO_ServerInformationIterator_Strategized_Proxy_Broker;

class TAO_PortableServer_Export ServerInformationIterator
  : public virtual PortableServer::ServantBase
{
protected:
  ServerInformationIterator (void);

public:
  ServerInformationIterator (const ServerInformationIterator& rhs);
  virtual ~ServerInformationIterator (void);


  virtual CORBA::Boolean _is_a (
      const char* logical_type_id,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );

  virtual void* _downcast (
      const char* logical_type_id
    );

  static void _is_a_skel (
      TAO_ServerRequest &req,
      void *obj,
      void *servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  static void _non_existent_skel (
      TAO_ServerRequest &req,
      void *obj,
      void *servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  static void _interface_skel (
      TAO_ServerRequest &req,
      void *obj,
      void *servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void _dispatch (
      TAO_ServerRequest &req,
      void *_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  ::ImplementationRepository::ServerInformationIterator *_this (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );

  virtual const char* _interface_repository_id (void) const;

  virtual CORBA::Boolean next_n (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    )) = 0;
  
    static void next_n_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );

  virtual void destroy (
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    )) = 0;
  
    static void destroy_skel (
      TAO_ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_servant_upcall,
      CORBA::Environment &ACE_TRY_ENV
    );


};


///////////////////////////////////////////////////////////////////////
//               Strategized Proxy Broker Declaration 
//

class TAO_PortableServer_Export _TAO_ServerInformationIterator_Strategized_Proxy_Broker : public virtual ::ImplementationRepository::_TAO_ServerInformationIterator_Proxy_Broker
{
public: 
  _TAO_ServerInformationIterator_Strategized_Proxy_Broker (void);
  
  virtual ~_TAO_ServerInformationIterator_Strategized_Proxy_Broker (void);
  
  virtual ::ImplementationRepository::_TAO_ServerInformationIterator_Proxy_Impl &select_proxy (
    ::ImplementationRepository::ServerInformationIterator *object,
    CORBA_Environment &ACE_TRY_ENV
  );

private:
  // Helper methods that takes care to create the proxy
  // as soon as their use is necessary.
  void create_proxy (
    int collocation_strategy,
    CORBA::Environment &ACE_TRY_ENV
  );
  
  // Caches the proxy implementations. The proxy implementation
  // are totally stateless, and those can be shared by all the
  // instances of a given IDL interface type.
  ::ImplementationRepository::_TAO_ServerInformationIterator_Proxy_Impl
  *proxy_cache_[TAO_Collocation_Strategies::CS_LAST];
  
  TAO_SYNCH_MUTEX mutex_;
  // This funxtion is used to get an handle to the unique instance
  // of the Strategized Proxy Broker that is available for a given
  // interface.

public:
  static _TAO_ServerInformationIterator_Strategized_Proxy_Broker *the_TAO_ServerInformationIterator_Strategized_Proxy_Broker (void);
};


//
//            End Strategized Proxy Broker Declaration 
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//                    ThruPOA  Impl. Declaration
//

class TAO_PortableServer_Export _TAO_ServerInformationIterator_ThruPOA_Proxy_Impl : 
  public virtual ::ImplementationRepository::_TAO_ServerInformationIterator_Proxy_Impl,
  public virtual TAO_ThruPOA_Object_Proxy_Impl
{
public:
  _TAO_ServerInformationIterator_ThruPOA_Proxy_Impl (void);
  
  virtual ~_TAO_ServerInformationIterator_ThruPOA_Proxy_Impl (void) { }
  
  virtual CORBA::Boolean next_n (
      CORBA_Object *_collocated_tao_target_,
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual void destroy (
      CORBA_Object *_collocated_tao_target_,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  
};

//
//                ThruPOA  Proxy Impl. Declaration
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//                    Direct  Impl. Declaration
//

class TAO_PortableServer_Export _TAO_ServerInformationIterator_Direct_Proxy_Impl : 
  public virtual ImplementationRepository::_TAO_ServerInformationIterator_Proxy_Impl,
  public virtual TAO_Direct_Object_Proxy_Impl
{
public:
  _TAO_ServerInformationIterator_Direct_Proxy_Impl (void);
  
  virtual ~_TAO_ServerInformationIterator_Direct_Proxy_Impl (void) { }
  
  virtual CORBA::Boolean next_n (
      CORBA_Object *_collocated_tao_target_,
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  virtual void destroy (
      CORBA_Object *_collocated_tao_target_,
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
  
  };


//
//                Direct  Proxy Impl. Declaration
///////////////////////////////////////////////////////////////////////


}
TAO_NAMESPACE_CLOSE // module ImplementationRepository

#include "ImplRepoS_T.h"

#if defined (__ACE_INLINE__)
#include "ImplRepoS.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option pop
#endif /* __BORLANDC__ */

#include "ace/post.h"
#endif /* ifndef */