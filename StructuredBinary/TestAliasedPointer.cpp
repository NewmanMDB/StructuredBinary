//
//  TestAliasedPointer.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestAliasedPointer.h"

#include <string.h>
#include <stdio.h>

#include "sbSchema.h"
#include "sbNode.h"
#include "sbAllocator.h"

namespace Src
{
  struct Struct
  {
    const char* string1;
    const char* string2;
    const char* string3;
    const char* string4;
  };
};

namespace Dst
{
  struct Struct
  {
    const char* string3;
    const char* string4;
    const char* string1;
    const char* string2;
  };
};

UnitTest::Result TestAliasedPointer::RunTest() const
{
  const char* src_stringA = "Aaa";
  const char* src_stringB = "Bbb";
  
  Src::Struct src_struct =
  {
    src_stringA,  // const char* string1;
    src_stringA,  // const char* string2;
    src_stringB,  // const char* string3;
    src_stringB,  // const char* string4;
  };
  
  sbSchema dst_schema;
  dst_schema.Begin();

  dst_schema.BeginNode( "Struct" );
  dst_schema.AddString( "string1", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  dst_schema.AddString( "string2", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  dst_schema.AddString( "string3", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  dst_schema.AddString( "string4", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  dst_schema.EndNode();
  
  dst_schema.End();
  
  sbSchema src_schema;
  src_schema.Begin();

  src_schema.BeginNode( "StringElem" );
  src_schema.AddScalar( "c", 1, kScalar_I8  );
  src_schema.EndNode();
  
  src_schema.BeginNode( "Struct" );
  src_schema.AddString( "string3", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  src_schema.AddString( "string4", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  src_schema.AddString( "string1", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  src_schema.AddString( "string2", 1, kScalar_I8, sbScalarValue::Int( 0 ) );
  src_schema.EndNode();

  src_schema.End();
  
  sbAllocator alloc( NULL, 0 );
  dst_schema.Convert( NULL, ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  printf( "Memory needed %lu\n", alloc.GetSize() );
  
  Dst::Struct dst_struct;
  char buffer[ 1000 ];
  alloc = sbAllocator( buffer, sizeof( buffer ) );
  dst_schema.Convert( ( char* )&dst_struct, ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  
  if( 0 != strcmp( dst_struct.string1, "Aaa" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string1 );
  if( 0 != strcmp( dst_struct.string2, "Aaa" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string2 );
  if( 0 != strcmp( dst_struct.string3, "Bbb" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string3 );
  if( 0 != strcmp( dst_struct.string4, "Bbb" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string4 );

  if( dst_struct.string1 != dst_struct.string2 ) return Error( "Source aliased pointer is no longer aliased after conversion" );
  if( dst_struct.string3 != dst_struct.string4 ) return Error( "Source aliased pointer is no longer aliased after conversion" );

  return Ok();
}