#include "dim.h"

//////////////////////////////
// Dim AND declare an Array //
//////////////////////////////
void DimNode::semant( Environ *e ){
	Type *t=tagType( tag,e );
	if( Decl *d=e->findDecl( ident ) ){
		ArrayType *a=d->type->arrayType();
		if( !a || a->dims!=exprs->size() || (t && a->elementType!=t) ){
			ex( "Duplicate identifier" );
		}
		sem_type=a;sem_decl=0;
	}else{
		if( e->level>0 ) ex( "Array not found in main program" );
		if( !t ) t=Type::int_type;
		sem_type=d_new ArrayType( t,exprs->size() );
		sem_decl=e->decls->insertDecl( ident,sem_type,DECL_ARRAY );
		e->types.push_back( sem_type );
	}
	exprs->semant( e );
	exprs->castTo( Type::int_type,e );
}

void DimNode::translate( Codegen *g ){
	TNode *t;
	g->code( call( "__bbUndimArray",global( "_a"+ident ) ) );
	for( int k=0;k<exprs->size();++k ){
		t=add( global( "_a"+ident ),iconst( k*4+12 ) );
		t=move( exprs->exprs[k]->translate(g),mem( t ) );
		g->code( t );
	}
	g->code( call( "__bbDimArray",global( "_a"+ident ) ) );

	if( !sem_decl ) return;

	int et;
	Type *ty=sem_type->arrayType()->elementType;
	if( ty==Type::int_type ) et=1;
	else if( ty==Type::float_type ) et=2;
	else if( ty==Type::string_type ) et=3;
	else et=5;

	g->align_data( 4 );
	g->i_data( 0,"_a"+ident );
	g->i_data( et );
	g->i_data( exprs->size() );
	for( int k=0;k<exprs->size();++k ) g->i_data( 0 );
}
