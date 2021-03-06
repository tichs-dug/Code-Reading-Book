// client_i.cpp,v 1.1 1999/07/02 05:21:01 irfan Exp

#include "client_i.h"

client_i::client_i (int quiet,
                    server_ptr s)
  : quiet_ (quiet),
    server_ (server::_duplicate (s))
{
}

void
client_i::ping (CORBA::UShort time_to_live,
                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (!this->quiet_)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) client_i::ping -> time to live = %d\n",
                time_to_live));

  --time_to_live;

  if (time_to_live > 0)
    {
      this->server_->ping (time_to_live,
                           ACE_TRY_ENV);
      ACE_CHECK;
    }
}
