/* 
 * File:   ScriptLua.cpp
 * Author: deangelis
 * 
 * Created on 20 settembre 2018, 12.11
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ScriptLua.h"


ScriptLua::ScriptLua() {
    state = luaL_newstate();
}

void ScriptLua::printf_error(){
    const char* message = lua_tostring(state, -1);
    puts(message);
    lua_pop(state, 1);
}

ScriptLua::~ScriptLua() {
    lua_close(state); //free lua interpeter state machine
}

void ScriptLua::addInt32Module(){ 
    luaopen_bit32(state);
}

void ScriptLua::addStringModule(){
    luaopen_string(state);
}

void ScriptLua::addIOModule(){
    luaopen_io(state);
}

void ScriptLua::addMathModule(){
    luaopen_math(state);
}

void ScriptLua::addOSModule(){
    luaopen_math(state);
}

int ScriptLua::call_function(const char* nfuc,const char* fmt,...){
    va_list args;
    va_start(args, fmt);
 
    // push functions and arguments
    lua_getglobal(state, nfuc);  /* function to be called */
    int nargs = strlen(fmt);
    while (*fmt != '\0') {
        switch (*fmt)
        {
        case 'd':
            lua_pushinteger(state, va_arg(args, int)); //integer arg
            break;
        case 's':
            lua_pushstring(state, va_arg(args, const char*)); //string argument
            break;
        case 'f':
            lua_pushnumber(state, va_arg(args, double)); //double arg
            break;
        }
        ++fmt;
    }
 
    // call method in protected mode
    int result = lua_pcall(state, nargs, LUA_MULTRET, 0);

    va_end(args); 
    return result;
}

double ScriptLua::getResultNumber(){
    double z = 0;
    if (lua_isnumber(state, -1)){
        z = lua_tonumber(state, -1);
        lua_pop(state, 1);  // pop returned value      
    }
    return z;
}

int ScriptLua::getResultInt(){
    int z = 0;
    if (lua_isinteger(state, -1)){
        z = lua_tointeger(state, -1);
        lua_pop(state, 1);  // pop returned value        
    }
    return z;
}

const char* ScriptLua::getResultString(){
    const char* z = 0;
    if (lua_isstring(state, -1)){
        z = lua_tostring(state, -1);
        lua_pop(state, 1);  // pop returned value        
    }
    return z;
}

void ScriptLua::addModule(const char* name,const luaL_Reg lib[]){
    luaL_newlib(state, lib);
    lua_setglobal(state, name);
}

int ScriptLua::load_script(const char* nf){
  int result = luaL_loadfile(state, nf);
  if ( result == LUA_OK ) 
    result = lua_pcall(state, 0, LUA_MULTRET, 0);
  return result;
}