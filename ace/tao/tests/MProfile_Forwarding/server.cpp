// server.cpp,v 1.3 2001/03/04 17:53:46 othman Exp

#include "ace/Get_Opt.h"
#include "test_i.h"

ACE_RCSID(MT_Client, server, "server.cpp,v 1.3 2001/03/04 17:53:46 othman Exp")

const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
	ior_output_file = get_opts.optarg;
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
			   "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());

      Simple_Server_var server =
        server_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
	orb->object_to_string (server.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
	{
	  FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
	  if (output_file == 0)
	    ACE_ERROR_RETURN ((LM_ERROR,
			       "Cannot open output file for writing IOR: %s",
			       ior_output_file),
			      1);
	  ACE_OS::fprintf (output_file, "%s", ior.in ());
	  ACE_OS::fclose (output_file);
	}


      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_OS::sleep (5);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
