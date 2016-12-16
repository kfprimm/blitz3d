
#include <cstdio>
#include "../../blitz/blitz.h"

extern "C" {

void bbMain();

void print( BBStr *s ){
  printf( "%s\n",s->c_str() );
  // delete s;
}

}

BBMODULE_DECL( basic );

int main(){
  printf("BB: Starting program...\n");

  basic_create();
  bbMain();
  basic_destroy();

  printf("BB: DONE!\n");
  return 1;
}
