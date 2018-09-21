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

void ScriptLua::printf_error() const{
    const char* message = lua_tostring(state, -1);
    puts(message);
    lua_pop(state, 1);
}

ScriptLua::~ScriptLua() {
    lua_close(state); //free lua interpeter state machine
}

void ScriptLua::addInt32Module(){ 
    lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
    luaopen_bit32(state); 
    lua_gc(state, LUA_GCRESTART, 0);
}

void ScriptLua::addAllBasicModule(){
    lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
    luaL_openlibs(state);  
    lua_gc(state, LUA_GCRESTART, 0);
}

void ScriptLua::addStringModule(){
    lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
    luaopen_string(state);
    lua_gc(state, LUA_GCRESTART, 0);
}

void ScriptLua::addIOModule(){
    lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
    luaopen_io(state);
    lua_gc(state, LUA_GCRESTART, 0);
}

void ScriptLua::addMathModule(){
    lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
    luaopen_math(state);
    lua_gc(state, LUA_GCRESTART, 0);
}

void ScriptLua::addOSModule(){
    lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
    luaopen_os(state);
    lua_gc(state, LUA_GCRESTART, 0);
}

int ScriptLua::call_function(const char* nfuc,const char* fmt,...){
    if(nfuc == nullptr || fmt == nullptr) 
        return -1;
    
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

TypeLua ScriptLua::getTypeReturn() const{
    return (TypeLua)lua_type(state, -1);
}

const char* ScriptLua::getTypeNameReturn() const{
    return lua_typename(state,lua_type(state, -1));
}

double ScriptLua::getResultNumber(){
    double z = 0;
    if (lua_isnumber(state, -1)){
        z = lua_tonumber(state, -1);
        lua_pop(state, 1);  // pop returned value      
    }
    return z;
}

void* ScriptLua::getResultUserData(){
    void* ptr = nullptr;
    if(lua_islightuserdata(state,-1)){
        ptr = lua_touserdata(state,-1);
        lua_pop(state, 1);  // pop returned value  
    }
    return ptr;
}

int ScriptLua::getResultInt(){
    int z = 0;
    if (lua_isinteger(state, -1)){
        z = lua_tointeger(state, -1);
        lua_pop(state, 1);  // pop returned value        
    }
    return z;
}

bool ScriptLua::getResultBool(){
    bool z = 0;
    if (lua_isboolean(state, -1)){
        z = lua_toboolean(state, -1);
        lua_pop(state, 1);  // pop returned value        
    }
    return z;
}

const char* ScriptLua::getResultString(){
    const char* z = nullptr;
    if (lua_isstring(state, -1)){
        z = lua_tostring(state, -1);
        lua_pop(state, 1);  // pop returned value        
    }
    return z;
}

void ScriptLua::addFunction(const char* name, lua_CFunction f){
    if(f!=nullptr && name != nullptr){
        lua_pushcfunction(state, f);
        lua_setglobal(state, name);
    }
}

void ScriptLua::addModule_unsafe(const char* name,const luaL_Reg lib[]){
    if(name!= nullptr){
        lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
        luaL_newlib(state, lib);
        lua_setglobal(state, name);
        lua_gc(state, LUA_GCRESTART, 0);
    }
}

void ScriptLua::addModule(const char* name, ModuleLua& lib){
    if(name!= nullptr){
        lua_gc(state, LUA_GCSTOP, 0);//stop collector during initialization
        
        luaL_Reg* ptr = lib.getModule();
        luaL_newlib(state, ptr);
        lua_setglobal(state, name);
        
        lua_gc(state, LUA_GCRESTART, 0);
        
        if(ptr!=nullptr) delete ptr;
    }
}

int ScriptLua::load_script(const char* nf){
  if(nf == nullptr) return -1;
  int result = luaL_loadfile(state, nf);
  if ( result == LUA_OK ) 
    result = lua_pcall(state, 0, LUA_MULTRET, 0);
  return result;
}

bool ScriptLua::isActiveGarbageCollector() const{
    return lua_gc(state, LUA_GCISRUNNING, 0);
}

void ScriptLua::restartGarbageCollector(){
    lua_gc(state, LUA_GCRESTART, 0);
}

void ScriptLua::stopGarbageCollector(){
    lua_gc(state, LUA_GCSTOP, 0);
}

int ScriptLua::memoryUsedKBGarbageCollector() const{
    return lua_gc(state, LUA_GCCOUNT, 0);
}

long ScriptLua::memoryUsedGarbageCollector() const{
    return (lua_gc(state, LUA_GCCOUNT, 0)<<10) + lua_gc(state, LUA_GCCOUNTB, 0);
}
