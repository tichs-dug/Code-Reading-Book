//demux_test_client.i,v 1.1 1999/04/28 03:47:33 vishal Exp
static void M302 (Demux_Test_ptr objref, 
                CORBA::Environment &env)
{
  objref->M302 (env);
}

static void shutdown (Demux_Test_ptr objref, 
                CORBA::Environment &env)
{
  objref->shutdown (env);
}

int Demux_Test_Client::init_operation_db (void)
{
   this->op_db_[0].op_ = M302;
   this->op_db_[1].op_ = shutdown;
   return 0;
}

