/*
 * Copyright (C) 2018 deangelis domenico francesco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
#include <typeinfo>
#include "ModuleLua.h"

typedef enum: int{
   NoneType = -1,
   NilType = 0,
   BoolType,
   LightUserDataType,
   NumberType,
   StringType,
   TableType,
   FunctionType,
   UserDataType,
   ThreadType
} TypeLua;

class ScriptLua {
public:
    ScriptLua();
    virtual ~ScriptLua();
    
    /*@brief: Add new module on envirioment
    * @param [name]: name new module
    * @param [lib]: matrix of method's name and function's pointer
    *               no control on finaly canary = {NULL,NULL} in lib[] 
    */
    void addModule_unsafe(const char* name,const luaL_Reg lib[]);
    
    /**
     * Secure mode, because user don't manage directly canary
     * @brief: add new module with ModuleLua's functions inside
     * @param [name] : name of new Module
     */
    void addModule(const char*, ModuleLua&);
    
    /*@brief: Add new function on envirioment
    * @param [name]: name new function
    * @param [lib]: real function's pointer
    */
    void addFunction(const char* name, lua_CFunction f);
    
    //To use basic Module
    void addIOModule();
    void addStringModule();
    void addMathModule();
    void addOSModule();
    void addInt32Module();
    void addAllBasicModule();
    
    /*
    * @brief: load script textural formato or compiled format
    * @param [const char*]: name function
    * @return: lua response or -1 if name function == nullptr
    */
    int load_script(const char*);
    
    /*@brief: call lua function in our loaded script
    * @param [nfuc]: name function
    * @param [fmt]: string to specificate the format data of variadic list
    *               i- int, s- const char*, f- double
    * @param [...]: variadic list 
    * @return lua response or -1 in case nfuc == nullptr or fmt == nullptr
    */
    int call_function(const char* nfuc,const char* fmt,...);
    
    //Function for receive results of function
    double getResultNumber();
    int getResultInt();
    const char* getResultString();
    bool getResultBool();
    void* getResultUserData();
    
    /*
     * @brief return var's type on stack, returned in previously execution
     */
    TypeLua getTypeReturn() const;
    /*
     * @brief return var's type on stack, returned in previously execution
     * @return  version textual of @see getTypeReturn()
     */
    const char* getTypeNameReturn() const;
    
    void printf_error() const;
    
    //GarbageCollector
    bool isActiveGarbageCollector() const;
    void restartGarbageCollector();
    void stopGarbageCollector();
    int memoryUsedKBGarbageCollector() const;
    long memoryUsedGarbageCollector() const;//in byte
private:
    lua_State *state;//state of this particular virtual machine
};

#endif /* SCRIPTLUA_H */
