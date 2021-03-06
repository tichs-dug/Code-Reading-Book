 
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# run_test.pl,v 1.1 2001/09/07 22:25:23 anandk Exp
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$threads = '2';
$iorfile = PerlACE::LocalFile ("test.ior");
$sv_conf = PerlACE::LocalFile ("server.conf");
$cl_conf = PerlACE::LocalFile ("client.conf");

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-ORBsvcconf $sv_conf -o $iorfile -n $threads");

$CL1 = new PerlACE::Process 
         ("client", "-k file://$iorfile -n 2 -i 100 -ORBSvcConf $cl_conf");

$CLS = new PerlACE::Process 
         ("client", "-k file://$iorfile -n 2 -i 100 -ORBSvcConf $cl_conf -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$CL1->Spawn ();
$client = $CL1->WaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$client = $CLS->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1
}

unlink $iorfile;

exit $status





