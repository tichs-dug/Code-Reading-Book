// fixed_struct.h,v 1.7 2000/03/21 20:04:47 coryan Exp

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    fixed_struct.h
//
// = DESCRIPTION
//     Tests fixed size structs
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_FIXED_STRUCT_H
#define PARAM_TEST_FIXED_STRUCT_H

#include "param_testCli.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test fixed structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Fixed_Struct
{
public:
  Test_Fixed_Struct (void);
  // ctor

  ~Test_Fixed_Struct (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  const char *opname (void) const;
  // return operation name

  int init_parameters (Param_Test_ptr objref,
                        CORBA::Environment &env);
  // set values for parameters

  int reset_parameters (void);
  // reset values for CORBA

  CORBA::Boolean check_validity (void);
  // check if results are valid

  CORBA::Boolean check_validity (CORBA::Request_ptr req);
  // check if results are valid. This is used for DII results

  void print_values (void);
  // print all the values

  void dii_req_invoke (CORBA::Request *, CORBA::Environment &);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  Param_Test::Fixed_Struct in_;
  // in parameter

  Param_Test::Fixed_Struct inout_;
  // inout parameter

  Param_Test::Fixed_Struct out_;
  // out parameter

  Param_Test::Fixed_Struct ret_;
  // return value
};

#endif /* PARAM_TEST_FIXED_STRUCT_H */
