
#include "std.h"
#include "nodes.h"

//////////////////
// The program! //
//////////////////
Environ *ProgNode::semant( Environ *e ){

	file_lab=genLabel();

	StmtSeqNode::reset( stmts->file,file_lab );

	a_ptr<Environ> env( d_new Environ( genLabel(),Type::int_type,0,e ) );

	consts->proto( env->decls,env );
	structs->proto( env->typeDecls,env );
	structs->semant( env );
	funcs->proto( env->funcDecls,env );
	stmts->semant( env );
	funcs->semant( env );
	datas->proto( env->decls,env );
	datas->semant( env );

	sem_env=env.release();
	return sem_env;
}

void ProgNode::translate( Codegen *g,const vector<UserFunc> &usrfuncs ){
	g->module=new llvm::Module( fullfilename( stmts->file ),g->context );

	//non-main functions
	funcs->translate( g );

	vector<llvm::Type *> args;
	llvm::FunctionType *proto = llvm::FunctionType::get( g->voidType(),args,false );

	llvm::Function *main = llvm::Function::Create( proto,llvm::Function::ExternalLinkage,"bbMain",g->module );

	// int k;
	//
	// if( g->debug ) g->s_data( stmts->file,file_lab );

	llvm::BasicBlock *main_block = llvm::BasicBlock::Create( g->context,"entry",main );
  g->builder.SetInsertPoint( main_block );


	// //enumerate locals
	// int size=enumVars( sem_env );
	//
	// //'Main' label
	// g->enter( "__MAIN",size );
	//
	// //reset data pointer
	// g->code( call( "__bbRestore",global( "__DATA" ) ) );
	//
	// //load external libs
	// g->code( call( "__bbLoadLibs",global( "__LIBS" ) ) );
	//
	// //call main program
	// g->code( jsr( sem_env->funcLabel+"_begin" ) );
	// g->code( jump( sem_env->funcLabel+"_leave" ) );
	// g->label( sem_env->funcLabel+"_begin" );
	//
	//create locals
	TNode *t=createVars( g,sem_env );
	g->values.clear();
	// if( t ) g->code( t );
	if( g->debug ){
		// string t=genLabel();
		// g->s_data( "<main program>",t );
		// g->code( call( "__bbDebugEnter",local(0),iconst((bint_t)sem_env),global(t) ) );
		g->debugEnter();
	}
	//
	// //no user funcs used!
	// usedfuncs.clear();

	//program statements
	stmts->translate( g );

	//emit return
	g->builder.CreateRetVoid();

	// //check labels
	// for( k=0;k<sem_env->labels.size();++k ){
	// 	if( sem_env->labels[k]->def<0 )	ex( "Undefined label '"+sem_env->labels[k]->name+"'",sem_env->labels[k]->ref,stmts->file );
	// }
	//
	// //leave main program
	// g->label( sem_env->funcLabel+"_leave" );
	// t=deleteVars( sem_env );
	// if( g->debug ) t=d_new TNode( IR_SEQ,call( "__bbDebugLeave" ),t );
	if( g->debug ) g->debugLeave();
	// g->leave( t,0 );

	// //structs
	// structs->translate( g );

	// //data
	datas->translate( g );

	// llvm::verifyFunction( *main );

	// //library functions
	// map<string,vector<int> > libFuncs;
	//
	// //lib ptrs
	// g->flush();
	// g->align_data(4);
	// for( k=0;k<usrfuncs.size();++k ){
	// 	const UserFunc &fn=usrfuncs[k];
	//
	// 	if( !usedfuncs.count(fn.ident) ) continue;
	//
	// 	libFuncs[fn.lib].push_back( k );
	//
	// 	g->i_data( 0,"_f"+fn.ident );
	// }
	//
	// //LIBS chunk
	// g->flush();
	// g->label( "__LIBS" );
	// map<string,vector<int> >::const_iterator lf_it;
	// for( lf_it=libFuncs.begin();lf_it!=libFuncs.end();++lf_it ){
	//
	// 	//lib name
	// 	g->s_data( lf_it->first );
	//
	// 	const vector<int> &fns=lf_it->second;
	//
	// 	for( int j=0;j<fns.size();++j ){
	// 		const UserFunc &fn=usrfuncs[ fns[j] ];
	//
	// 		//proc name
	// 		g->s_data( fn.proc );
	//
	// 		g->p_data( "_f"+fn.ident );
	// 	}
	// 	g->s_data( "" );
	// }
	// g->s_data( "" );
	//
	// //DATA chunk
	// g->flush();
	// g->align_data( 4 );
	// g->label( "__DATA" );
	// datas->transdata( g );
	// g->i_data(0);
	//
	// //Thats IT!
	// g->flush();
}

void ProgNode::dump(){
	cout<<"FUNCTIONS:"<<endl;
	for( int k=0;k<funcs->decls.size();++k ){
		FuncDeclNode *n=(FuncDeclNode*)funcs->decls[k];
		cout<<"  - "<<n->ident<<":"<<endl;
	}
}
