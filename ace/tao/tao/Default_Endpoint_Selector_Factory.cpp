// Default_Endpoint_Selector_Factory.cpp,v 1.1 2001/06/12 18:57:58 fhunleth Exp

#include "tao/Default_Endpoint_Selector_Factory.h"
#include "tao/Invocation.h"

ACE_RCSID(tao, Default_Endpoint_Selector_Factory, "Default_Endpoint_Selector_Factory.cpp,v 1.1 2001/06/12 18:57:58 fhunleth Exp")

TAO_Default_Endpoint_Selector_Factory::TAO_Default_Endpoint_Selector_Factory (void)
{
  ACE_NEW (this->default_endpoint_selector_,
           TAO_Default_Endpoint_Selector);

}

TAO_Default_Endpoint_Selector_Factory::~TAO_Default_Endpoint_Selector_Factory (void)
{
  delete this->default_endpoint_selector_;
}


TAO_Invocation_Endpoint_Selector *
TAO_Default_Endpoint_Selector_Factory::get_selector (TAO_GIOP_Invocation *,
                                             CORBA::Environment &)
{
  // Trivial endpoint selector.  Just return the default selector.
  return this->default_endpoint_selector_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Endpoint_Selector_Factory,
                       ACE_TEXT ("Default_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Endpoint_Selector_Factory)

