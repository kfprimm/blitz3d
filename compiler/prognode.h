
#ifndef PROGNODE_H
#define PROGNODE_H

#include "node.h"
#include "codegen.h"

extern vector<string> modules;

struct UserFunc{
	string ident,proc,lib;
	UserFunc( const UserFunc &t ):ident(t.ident),proc(t.proc),lib(t.lib){}
	UserFunc( const string &id,const string &pr,const string &lb ):ident(id),proc(pr),lib(lb){}
};

struct ProgNode : public Node{

	DeclSeqNode *consts;
	DeclSeqNode *structs;
	DeclSeqNode *funcs;
	DeclSeqNode *datas;
	StmtSeqNode *stmts;

	Environ *sem_env;

	string file_lab;

	ProgNode( DeclSeqNode *c,DeclSeqNode *s,DeclSeqNode *f,DeclSeqNode *d,StmtSeqNode *ss ):consts(c),structs(s),funcs(f),datas(d),stmts(ss){}
	~ProgNode(){
		delete consts;
		delete structs;
		delete funcs;
		delete datas;
		delete stmts;
	}

	Environ *semant( Environ *e );
	void translate( Codegen *g,const vector<UserFunc> &userfuncs );

	json toJSON(){
		Environ *e=sem_env;

		json tree;tree["@class"]="ProgNode";
		tree["modules"]=modules;
		tree["funcs"]=funcs->toJSON( e );
		tree["structs"]=structs->toJSON( e );
		tree["locals"]=json::array();
		for( int k=0;k<e->decls->size();++k ){
			Decl *d=e->decls->decls[k];
			if( d->kind!=DECL_LOCAL ) continue;
			tree["locals"].push_back( d->toJSON() );
		}
		tree["stmts"]=stmts->toJSON( e );
		return tree;
	}
};

#endif
