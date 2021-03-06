//
// be_visitor.cpp,v 1.9 1999/11/19 23:56:51 kirthika Exp
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor.h
//
// = DESCRIPTION
//    Abstract class for back end visitors.
//
// = NOTES
//    This is not a pure abstract class
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor.h"

ACE_RCSID(be, be_visitor, "be_visitor.cpp,v 1.9 1999/11/19 23:56:51 kirthika Exp")

be_visitor::be_visitor (void)
{
}

be_visitor::~be_visitor (void)
{
}

// all the visit methods. Concrete visitors will selectively override each one
int be_visitor::visit_decl (be_decl *)
{
  return 0;
}

int be_visitor::visit_scope (be_scope *)
{
  return 0;
}

int be_visitor::visit_type (be_type *)
{
  return 0;
}

int be_visitor::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int be_visitor::visit_module (be_module *)
{
  return 0;
}

int be_visitor::visit_interface (be_interface *)
{
  return 0;
}

int be_visitor::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

#ifdef IDL_HAS_VALUETYPE
int be_visitor::visit_valuetype (be_valuetype *)
{
  return 0;
}

int be_visitor::visit_valuetype_fwd (be_valuetype_fwd *)
{
  return 0;
}
#endif /* IDL_HAS_VALUETYPE */

int be_visitor::visit_structure (be_structure *)
{
  return 0;
}

int be_visitor::visit_exception (be_exception *)
{
  return 0;
}

int be_visitor::visit_expression (be_expression *)
{
  return 0;
}

int be_visitor::visit_enum (be_enum *)
{
  return 0;
}

int be_visitor::visit_operation (be_operation *)
{
  return 0;
}

int be_visitor::visit_field (be_field *)
{
  return 0;
}

int be_visitor::visit_argument (be_argument *)
{
  return 0;
}

int be_visitor::visit_attribute (be_attribute *)
{
  return 0;
}

int be_visitor::visit_union (be_union *)
{
  return 0;
}

int be_visitor::visit_union_branch (be_union_branch *)
{
  return 0;
}

int be_visitor::visit_union_label (be_union_label *)
{
  return 0;
}

int be_visitor::visit_constant (be_constant *)
{
  return 0;
}

int be_visitor::visit_enum_val (be_enum_val *)
{
  return 0;
}

int be_visitor::visit_array (be_array *)
{
  return 0;
}

int be_visitor::visit_sequence (be_sequence *)
{
  return 0;
}

int be_visitor::visit_string (be_string *)
{
  return 0;
}

int be_visitor::visit_typedef (be_typedef *)
{
  return 0;
}

int be_visitor::visit_root (be_root *)
{
  return 0;
}

int be_visitor::visit_native (be_native *)
{
  return 0;
}
