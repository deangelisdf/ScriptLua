/* 
 * File:   ScriptLua.h
 * Author: deangelis domenico francesco
 *
 * Created on 20 settembre 2018, 12.11
 */

#ifndef SCRIPTLUA_H
#define SCRIPTLUA_H

#include <stdio.h>
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>

class ScriptLua {
public:
    ScriptLua();
    virtual ~ScriptLua();
    /*@brief: Add new module for script
    * @param [name]: name new module
    * @param [lib]: matrix of method's name and function's pointer
    */
    void addModule(const char* name,const luaL_Reg lib[]);
    
    void addIOModule();
    void addStringModule();
    void addMathModule();
    void addOSModule();
    void addInt32Module();
    
    int load_script(const char*);
    
    /*@brief: call lua function in our loaded script
    * @param [nfuc]: name function
    * @param [fmt]: string to specificate the format data of variadic list
    *               i- int, s- const char*, f- double
    * @param [...]: variadic list 
    * @return lua response
    */
    int call_function(const char* nfuc,const char* fmt,...);
    
    //Function for receive result of function
    double getResultNumber();
    int getResultInt();
    const char* getResultString();
    
    void printf_error();
private:
    lua_State *state;//state of this particular virtual machine
};

#endif /* SCRIPTLUA_H */

