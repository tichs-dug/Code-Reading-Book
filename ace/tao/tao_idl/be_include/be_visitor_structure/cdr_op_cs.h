/* -*- c++ -*- */
//
// cdr_op_cs.h,v 1.4 1999/05/28 02:16:45 coryan Exp
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Structure class
//    This one provides code generation for the CDR operators for the structure
//    in the client stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_CDR_OP_CS_H_
#define _BE_VISITOR_STRUCTURE_CDR_OP_CS_H_

class be_visitor_structure_cdr_op_cs : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for structure that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_structure_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_cdr_op_cs (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure
};

#endif /* _BE_VISITOR_STRUCTURE_CDR_OP_CS_H_ */
