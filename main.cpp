#include <stdio.h>
#include "ScriptLua.h"

static int l_foo (lua_State *L) {
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, d/2);
    printf("d: %f\n",d);
    return 1;  // number of results
}

static const struct luaL_Reg gsyslib[] = {
      {"div", l_foo},
      {NULL, NULL}  //canarino
};

int main(int argc, char** argv)
{
    ScriptLua script,script2;
    
    script.addModule("mymodule",gsyslib);
    
    if ( script.load_script("test.lua") != LUA_OK ) {
        script.printf_error();
        return -1;
    }    

    if ( script2.load_script("test2.lua") != LUA_OK ) {
        script2.printf_error();
        return -1;
    }
    
    //i want pass 2 double number
    if(script.call_function("func","ff",10.f,1.f) == LUA_OK){
        printf("%f\n",script.getResultNumber());
    }
    else{
        script.printf_error();
    }
    if(script2.call_function("func2","s","Hello World") == LUA_OK){
        printf("%s\n",script2.getResultString());
    }else{
        script.printf_error();
    }
    
  return 0;
}
