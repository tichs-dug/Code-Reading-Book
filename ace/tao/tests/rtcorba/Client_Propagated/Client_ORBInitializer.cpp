// -*- C++ -*-
//
// Client_ORBInitializer.cpp,v 1.1 2001/03/28 17:26:02 fhunleth Exp
//

#include "Client_ORBInitializer.h"
#include "interceptors.h"

ACE_RCSID (Client_Propagated, Client_ORBInitializer, "Client_ORBInitializer.cpp,v 1.1 2001/03/28 17:26:02 fhunleth Exp")

Client_ORBInitializer::Client_ORBInitializer (void)
{
}

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  CORBA::String_var orb_id =
    info->orb_id (ACE_TRY_ENV);
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the Echo client request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Client_Request_Interceptor (orb_id.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in (),
                                        ACE_TRY_ENV);
  ACE_CHECK;
}

