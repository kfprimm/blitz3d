
#ifndef CODEGEN_H
#define CODEGEN_H

#include "std.h"

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>


enum{
	IR_JUMP,IR_JUMPT,IR_JUMPF,IR_JUMPGE,

	IR_SEQ,IR_MOVE,IR_MEM,IR_LOCAL,IR_GLOBAL,IR_ARG,IR_CONST,

	IR_JSR,IR_RET,IR_AND,IR_OR,IR_XOR,IR_SHL,IR_SHR,IR_SAR,

	IR_CALL,IR_RETURN,IR_CAST,
	IR_NEG,IR_ADD,IR_SUB,IR_MUL,IR_DIV,
	IR_SETEQ,IR_SETNE,IR_SETLT,IR_SETGT,IR_SETLE,IR_SETGE,

	IR_FCALL,IR_FRETURN,IR_FCAST,
	IR_FNEG,IR_FADD,IR_FSUB,IR_FMUL,IR_FDIV,
	IR_FSETEQ,IR_FSETNE,IR_FSETLT,IR_FSETGT,IR_FSETLE,IR_FSETGE,
};

struct TNode{

	int op;				//opcode
	TNode *l,*r;		//args
	int iconst;			//for CONST type_int
	string sconst;		//for CONST type_string
	llvm::Value *value;

	TNode( int op,TNode *l=0,TNode *r=0,llvm::Value *value=0 ):op(op),l(l),r(r),value(value),iconst(0){}
	TNode( int op,TNode *l,TNode *r,int i,llvm::Value *value=0 ):op(op),l(l),r(r),iconst(i),value(value){}
	TNode( int op,TNode *l,TNode *r,const string &s,llvm::Value *value=0 ):op(op),l(l),r(r),iconst(0),sconst(s),value(value){}
	TNode( llvm::Value *value ):value(value),l(0),r(0){}
	~TNode(){ delete l;delete r; }

	void log();
};

class Codegen{
public:
  llvm::LLVMContext &context;
  llvm::IRBuilder<> builder;
	llvm::Module *module;
	Environ *env;
	map<string,llvm::Value *> values;

	bool debug;
	Codegen( llvm::LLVMContext &context, bool debug ):context( context ),builder( context ),debug( debug ){}

	virtual void enter( const string &l,int frameSize ){}
	virtual void code( TNode *code ){}
	virtual void leave( TNode *cleanup,int pop_sz ){}
	virtual void label( const string &l ){}
	virtual void i_data( int i,const string &l="" ){}
	virtual void s_data( const string &s,const string &l="" ){}
	virtual void p_data( const string &p,const string &l="" ){}
	virtual void align_data( int n ){}
	virtual void flush(){}

	llvm::Value *findValue( Decl *d ){
		auto value=values[d->name];
		if( !value && d->kind==DECL_LOCAL ){
			value=values[d->name]=builder.CreateAlloca( findType( d->type ),nullptr,d->name );
		}
		return value;
	}

	llvm::Function *findFunc( const string &ident ){
		auto func=module->getFunction( ident );
		if( !func ){

			Decl *d=env->findFunc( ident );
			FuncType *f=d->type->funcType();

			vector<llvm::Type *> args;
			for( int k=0;k<f->params->size();++k ){
				Decl *p=f->params->decls[k];
				auto t=findType( p->type );
				if( f->cfunc && p->type==Type::string_type ) t=cstrType();
				args.push_back( t );
			}

			auto proto = llvm::FunctionType::get( findType( d->type ),args,false );
			func=llvm::Function::Create( proto,llvm::Function::ExternalLinkage,ident,module );

		}
		return func;
	}

	llvm::Type *intType(){ return llvm::Type::getInt32Ty(context); }
	llvm::Type *floatType(){ return llvm::Type::getFloatTy(context); }
	// llvm::Type *stringType(){ return ptrType(); }
	llvm::Type *stringType(){
		static llvm::PointerType *type=nullptr;
		if( !type ){
			type=llvm::PointerType::get( llvm::StructType::create( context,"BBStr" ),0 );
		}
		return type;
	}
	llvm::Type *ptrType(){ return llvm::Type::getInt32PtrTy(context); }
	llvm::Type *cstrType(){ return llvm::Type::getInt8PtrTy(context); }
	llvm::Type *charType(){ return llvm::Type::getInt8Ty(context); }
	llvm::Type *voidType(){ return llvm::Type::getVoidTy(context); }

	llvm::Type *findType( Type *t ){
		if( t==Type::int_type ) return intType();
		if( t==Type::float_type ) return floatType();
		if( t==Type::string_type ) return stringType();
		if( t==Type::void_type ) return voidType();
		return voidType();
	}

	llvm::Value *debugEnter(){
		static llvm::Function *func=0;
		if( !func ){
			vector<llvm::Type *> args;
			llvm::FunctionType *proto=llvm::FunctionType::get( voidType(),args,false );
			func=llvm::Function::Create( proto,llvm::Function::InternalLinkage,"__bbDebugEnter",module );
		}
		return builder.CreateCall( func );
	}
	llvm::Value *debugLeave(){
		static llvm::Function *func=0;
		if( !func ){
			vector<llvm::Type *> args;
			llvm::FunctionType *proto=llvm::FunctionType::get( voidType(),args,false );
			func=llvm::Function::Create( proto,llvm::Function::InternalLinkage,"__bbDebugLeave",module );
		}
		return builder.CreateCall( func );
	}

	llvm::Value *callLib( const string &ident,llvm::Type *ret,int n_args,... ){
		cout<<ident<<endl;
		vector<llvm::Value *> args;
		va_list ap;
		va_start( ap,n_args );
		for( int i=1;i <= n_args; i++) {
			args.push_back( va_arg(ap, llvm::Value *) );
		}
		va_end( ap );

		auto func=module->getFunction( ident );
		if( !func ){
			vector<llvm::Type *> argTypes;
			for( uint i=0;i<args.size();i++ ){
				argTypes.push_back( args[i]->getType() );
			}

			llvm::FunctionType *proto=llvm::FunctionType::get( ret,argTypes,false );
			func=llvm::Function::Create( proto,llvm::Function::ExternalLinkage,ident,module );
		}

		return builder.CreateCall( func,args );
	}

	void dump(){ module->dump(); }
};

#endif
