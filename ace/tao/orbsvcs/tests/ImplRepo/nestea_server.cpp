// nestea_server.cpp,v 1.6 2001/03/06 22:05:52 brunsch Exp

#include "nestea_server_i.h"

ACE_RCSID(ImplRepo, nestea_server, "nestea_server.cpp,v 1.6 2001/03/06 22:05:52 brunsch Exp")

int
main (int argc, char *argv[])
{
  Nestea_Server_i server;

  ACE_DEBUG ((LM_DEBUG, "\n\tNestea Bookshelf Server\n\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int retval = server.init (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (retval == -1)
        return -1;
      else
        {
          server.run (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "Nestea Bookshelf Server says goodnight\n"));

  return 0;
}
