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
 * File:   ModuleLua.h
 * Author: deangelis
 *
 * Created on 21 settembre 2018, 13.52
 */

#ifndef MODULELUA_H
#define MODULELUA_H

#include <vector>
#include <string>
#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
using std::vector;
using std::string;

class ModuleLua {
public:
    ModuleLua();
    ModuleLua(const ModuleLua& orig);
    virtual ~ModuleLua();
    
    /*@brief: add new function in module. The module isn't added at interpreter
     *@param [name]: name function
     *@param [f] : function's pointer
     */
    void addFunctionOnModule(const char* name, lua_CFunction f);
    
    luaL_Reg* getModule();
    
    void clear();
private:
    vector<string> name;
    vector<lua_CFunction> func;
};

#endif /* MODULELUA_H */

