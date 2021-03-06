// client.cpp,v 1.2 2001/05/11 00:02:17 coryan Exp

#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"

#include "tao/Strategies/advanced_resource.h"

ACE_RCSID(Thread_Pool_Latency, client, "client.cpp,v 1.2 2001/05/11 00:02:17 coryan Exp")

const char *ior = "file://test.ior";
int niterations = 1000;
int do_shutdown = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "xk:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 0;
        break;

      case 'k':
        ior = get_opts.optarg;
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-x (disable shutdown) "
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
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "Starting threads\n"));

      Client_Task task0(roundtrip.in (), niterations);
      Client_Task task1(roundtrip.in (), niterations);
      Client_Task task2(roundtrip.in (), niterations);
      Client_Task task3(roundtrip.in (), niterations);

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      task0.activate(THR_NEW_LWP | THR_JOINABLE);
      task1.activate(THR_NEW_LWP | THR_JOINABLE);
      task2.activate(THR_NEW_LWP | THR_JOINABLE);
      task3.activate(THR_NEW_LWP | THR_JOINABLE);

      task0.thr_mgr()->wait ();
      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "Threads finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      ACE_Basic_Stats totals;
      task0.accumulate_and_dump (totals, "Task[0]", gsf);
      task1.accumulate_and_dump (totals, "Task[1]", gsf);
      task2.accumulate_and_dump (totals, "Task[2]", gsf);
      task3.accumulate_and_dump (totals, "Task[3]", gsf);

      totals.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             test_end - test_start,
                                             totals.samples_count ());

      if (do_shutdown)
        {
          roundtrip->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
