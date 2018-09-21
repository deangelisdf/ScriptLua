#include <stdio.h>
#include <string>
#include "ScriptLua.h"
using std::string;

static int l_foo (lua_State *L) {
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, d/2);
    return 1;  // number of results
}

static int l_funcTest (lua_State *L) {
    const char* s = luaL_checkstring(L, 1);
    printf("funcTest: %s\n",s);
    return 0;  // number of results
}

static const struct luaL_Reg gsyslib[] = {
      {"div", l_foo},
      {NULL, NULL}  //canarino
};

int main(int argc, char** argv)
{
    ModuleLua modulo;
    ScriptLua script,script2;
    
    //Create a safe module
    modulo.addFunctionOnModule("f1",l_foo);
    modulo.addFunctionOnModule("f2",l_funcTest);
    
    printf("After init\n");
    //Amount of memory used for Garbagel Collector
    printf("Script1 GC kb: %ld\nScript2 GC kb: %ld\n",script.memoryUsedGarbageCollector(),script2.memoryUsedGarbageCollector());
    
    //In this case, I should control a canary in array
    script.addModule_unsafe("mymodule",gsyslib);
    script.addFunction("func_test",l_funcTest);
    //more simply to use...
    script2.addModule("safe",modulo);
    
    if ( script.load_script("test.lua") != LUA_OK ) {
        script.printf_error();
        return -1;
    }    

    if ( script2.load_script("test2.lua") != LUA_OK ) {
        script2.printf_error();
        return -1;
    }
    
    printf("\nAfter load script\n");
    printf("Script1 GC kb: %ld\nScript2 GC kb: %ld\n",script.memoryUsedGarbageCollector(),script2.memoryUsedGarbageCollector());
    printf("\nExecution Script1:\n\n");
    
    //i want pass 2 double number
    if(script.call_function("func","ff",10.f,1.f) == LUA_OK){
        printf("type return: %d,%s\n",script.getTypeReturn(),script.getTypeNameReturn());
        printf("%f\n",script.getResultNumber());
    }
    else{
        script.printf_error();
    }

    printf("\nExecution Script2:\n\n");
    if(script2.call_function("func2","s","Hello World") == LUA_OK){
        printf("type return: %d,%s\n",script2.getTypeReturn(),script2.getTypeNameReturn());
        printf("%s\n",script2.getResultString());
    }else{
        script.printf_error();
    }
    
    printf("\nAfter Execution\n");
    printf("Script1 GC kb: %ld\nScript2 GC kb: %ld\n",script.memoryUsedGarbageCollector(),script2.memoryUsedGarbageCollector());
    
  return 0;
}
