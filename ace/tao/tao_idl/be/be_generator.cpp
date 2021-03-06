// be_generator.cpp,v 1.30 2000/10/04 23:54:26 parsons Exp

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

// Implementation of BE generator class.
//
// This implements the same protocol as AST_Generator but creates instances
// of the BE-subclassed classes instead of of AST classes.

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "ace/config-all.h"

ACE_RCSID(be, be_generator, "be_generator.cpp,v 1.30 2000/10/04 23:54:26 parsons Exp")

AST_Root *
be_generator::create_root (UTL_ScopedName *n,
                           UTL_StrList *p)
{
  be_root *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_root (n,
                           p),
                  0);

  return retval;
}

AST_PredefinedType *
be_generator::create_predefined_type (AST_PredefinedType::PredefinedType t,
                                      UTL_ScopedName *n,
                                      UTL_StrList *p)
{
  be_predefined_type *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_predefined_type (t,
                                      n,
                                      p),
                  0);

  return retval;
}

AST_Module *
be_generator::create_module (UTL_Scope *s,
                             UTL_ScopedName *n,
                             UTL_StrList *p)
{
  AST_Decl *d = 0;
  AST_Module *m = 0;

  UTL_ScopeActiveIterator *iter = 0;
  ACE_NEW_RETURN (iter,
                  UTL_ScopeActiveIterator (s,
                                           UTL_Scope::IK_decls),
                  0);

  // We create this first so if we find a module with the
  // same name from an included file, we can add its
  // members to the new module's scope.
  AST_Module *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_module (n, 
                             p),
                  0);


  // Check for another module of the same name in this scope.
  while (!iter->is_done ())
    {
      d = iter->item ();

      if (d->node_type () == AST_Decl::NT_module)
        {
          // Does it have the same name as the one we're
          // supposed to create.
          if (d->local_name ()->compare (n->last_component ()))
            {
              m = AST_Module::narrow_from_decl (d);

              // Get m's previous_ member, plus all it's decls,
              // into the new modules's previous_ member.
              retval->add_to_previous (m);
            }
        }

      iter->next ();
    }

  delete iter;

  // If this scope is itself a module, and has been previously
  // opened, the previous opening may contain a previous opening
  // of the module we're creating.
  d = ScopeAsDecl (s);
  AST_Decl::NodeType nt = d->node_type ();

  if (nt == AST_Decl::NT_module || nt == AST_Decl::NT_root)
    {
      m = AST_Module::narrow_from_decl (d);

      // AST_Module::previous_ is a set, so it contains each
      // entry only once, but previous_ will contain the decls
      // from all previous openings. See comment in
      // AST_Module::add_to_previous() body.
      d = m->look_in_previous (n->last_component ());

      if (d != 0)
        {
          if (d->node_type () == AST_Decl::NT_module)
            {
              m = AST_Module::narrow_from_decl (d);

              retval->add_to_previous (m);
            }
        }
    }

  // If we are opening module CORBA, we must add the predefined
  // types TypeCode, TCKind and maybe ValueBase.
  if (!ACE_OS::strcmp (retval->local_name ()->get_string (), "CORBA"))
    {
      retval->add_CORBA_members ();
    }

  return retval;
}

AST_Interface *
be_generator::create_interface (UTL_ScopedName *n,
                                AST_Interface **ih,
                                long nih,
                                AST_Interface **ih_flat,
                                long nih_flat,
                                UTL_StrList *p,
                                idl_bool l,
                                idl_bool a)
{
  be_interface *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_interface (n,
                                ih,
                                nih,
                                ih_flat,
                                nih_flat,
                                p,
                                l,
                                a),
                  0);

  return retval;
}

AST_InterfaceFwd *
be_generator::create_interface_fwd (UTL_ScopedName *n,
                                    UTL_StrList *p,
                                    idl_bool local,
                                    idl_bool abstract)
{
  be_interface_fwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_interface_fwd (this->create_interface (n,
                                                            0,
                                                            -1,
                                                            0,
                                                            0,
                                                            p,
                                                            local,
                                                            abstract),
                                    n,
                                    p),
                   0);

  return retval;
}

AST_Interface *
be_generator::create_valuetype (UTL_ScopedName *n,
                                AST_Interface **ih,
                                long nih,
                                UTL_StrList *p)
{
#ifdef IDL_HAS_VALUETYPE
  be_valuetype *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_valuetype (n, 
                                ih, 
                                nih, 
                                p),
                  0);

  return retval;
#else
  ACE_ERROR_RETURN ((LM_ERROR,
                     "Valuetype support not enabled\n"),
                    0);
#endif /* IDL_HAS_VALUETYPE */
}

AST_InterfaceFwd *
be_generator::create_valuetype_fwd (UTL_ScopedName *n, 
                                    UTL_StrList *p)
{
#ifdef IDL_HAS_VALUETYPE
  be_valuetype_fwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_valuetype_fwd (this->create_valuetype (n, 
                                                            0, 
                                                            -1, 
                                                            p),
                                    n,
                                    p),
                  0);

  return retval;
#else
  ACE_ERROR_RETURN ((LM_ERROR,
                     "Valuetype support not enabled\n"),
                    0);
#endif /* IDL_HAS_VALUETYPE */
}

AST_Exception *
be_generator::create_exception (UTL_ScopedName *n,
                                UTL_StrList *p,
                                idl_bool local,
                                idl_bool abstract)
{
  be_exception *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_exception (n,
                                p,
                                local,
                                abstract),
                  0);

  return retval;
}

AST_Structure *
be_generator::create_structure (UTL_ScopedName *n,
                                UTL_StrList *p,
                                idl_bool local,
                                idl_bool abstract)
{
  be_structure *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_structure (n,
                                p,
                                local,
                                abstract),
                  0);

  return retval;
}

AST_Enum *
be_generator::create_enum (UTL_ScopedName *n,
                           UTL_StrList *p,
                           idl_bool local,
                           idl_bool abstract)
{
  be_enum *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_enum (n, 
                           p, 
                           local, 
                           abstract),
                  0);

  return retval;
}

AST_Operation *
be_generator::create_operation (AST_Type *rt,
                                AST_Operation::Flags fl,
                                UTL_ScopedName *n,
                                UTL_StrList *p,
                                idl_bool local,
                                idl_bool abstract)
{
  be_operation *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_operation (rt,
                                fl,
                                n,
                                p,
                                local,
                                abstract),
                  0);

  return retval;
}

AST_Field *
be_generator::create_field (AST_Type *ft,
                            UTL_ScopedName *n,
                            UTL_StrList *p,
                            AST_Field::Visibility vis)
{
  be_field *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_field (ft, 
                            n, 
                            p, 
                            vis),
                  0);

  return retval;
}

AST_Argument *
be_generator::create_argument (AST_Argument::Direction d,
                               AST_Type *ft,
                               UTL_ScopedName *n,
                               UTL_StrList *p)
{
  be_argument *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_argument (d, 
                               ft, 
                               n, 
                               p),
                  0);

  return retval;
}

AST_Attribute *
be_generator::create_attribute (idl_bool ro,
                                AST_Type *ft,
                                UTL_ScopedName *n,
                                UTL_StrList *p,
                                idl_bool local,
                                idl_bool abstract)
{
  be_attribute *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_attribute (ro,
                                ft,
                                n,
                                p,
                                local,
                                abstract),
                  0);

  return retval;
}

AST_Union *
be_generator::create_union (AST_ConcreteType *dt,
                            UTL_ScopedName *n,
                            UTL_StrList *p,
                            idl_bool local,
                            idl_bool abstract)
{
  be_union *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_union (dt,
                            n,
                            p,
                            local,
                            abstract),
                  0);

  return retval;
}

AST_UnionBranch *
be_generator::create_union_branch (UTL_LabelList *ll,
                                   AST_Type *ft,
                                   UTL_ScopedName *n,
                                   UTL_StrList *p)
{
  be_union_branch *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_union_branch (ll, 
                                   ft, 
                                   n, 
                                   p),
                  0);

  return retval;
}

AST_UnionLabel *
be_generator::create_union_label (AST_UnionLabel::UnionLabel ul,
                                  AST_Expression *lv)
{
  be_union_label *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_union_label (ul, 
                                  lv),
                  0);

  return retval;
}

AST_Constant *
be_generator::create_constant (AST_Expression::ExprType et,
                               AST_Expression *ev,
                               UTL_ScopedName *n,
                               UTL_StrList *p)
{
  be_constant *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_constant (et, 
                               ev, 
                               n, 
                               p),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (UTL_ScopedName *n)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (n),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (AST_Expression *b, 
                           AST_Expression::ExprType t)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (b, 
                                 t),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (AST_Expression::ExprComb c,
                           AST_Expression *v1,
                           AST_Expression *v2)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (c, 
                                 v1,
                                 v2),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (long l)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (l),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (long l, 
                           AST_Expression::ExprType t)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (l, 
                                 t),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (unsigned long l)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (l),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (UTL_String *s)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (s),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr(char c)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (c),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (ACE_OutputCDR::from_wchar wc)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (wc),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (char *s)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (s),
                  0);

  return retval;
}

AST_Expression *
be_generator::create_expr (double d)
{
  be_expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_expression (d),
                  0);

  return retval;
}

AST_EnumVal *
be_generator::create_enum_val (unsigned long v,
                               UTL_ScopedName *n,
                               UTL_StrList *p)
{
  be_enum_val *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_enum_val (v, 
                               n, 
                               p),
                  0);

  return retval;
}

AST_Array *
be_generator::create_array (UTL_ScopedName *n,
                            unsigned long ndims,
                            UTL_ExprList *dims,
                            idl_bool local,
                            idl_bool abstract)
{
  be_array *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_array (n,
                            ndims,
                            dims,
                            local,
                            abstract),
                  0);

  return retval;
}

AST_Sequence *
be_generator::create_sequence (AST_Expression *v,
                               AST_Type *bt,
                               idl_bool local,
                               idl_bool abstract)
{
  be_sequence *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_sequence (v,
                               bt,
                               local,
                               abstract),
                  0);

  return retval;
}

AST_String *
be_generator::create_string (AST_Expression *v)
{
  be_string *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_string (v),
                  0);

  return retval;
}

AST_String *
be_generator::create_wstring (AST_Expression *v)
{
  be_string *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_string (v,
                             sizeof (ACE_CDR::WChar)),
                  0);

  return retval;
}

AST_Typedef *
be_generator::create_typedef (AST_Type *bt,
                              UTL_ScopedName *n,
                              UTL_StrList *p,
                              idl_bool local,
                              idl_bool abstract)
{
  be_typedef *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_typedef (bt, 
                              n, 
                              p, 
                              local, 
                              abstract),
                  0);

  return retval;
}

AST_Native *
be_generator::create_native (UTL_ScopedName *n,
                             UTL_StrList *p)
{
  be_native *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_native (n, 
                             p),
                  0);

  return retval;
}
