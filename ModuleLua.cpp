/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModuleLua.cpp
 * Author: deangelis
 * 
 * Created on 21 settembre 2018, 13.52
 */

#include "ModuleLua.h"

ModuleLua::ModuleLua() {}

ModuleLua::ModuleLua(const ModuleLua& orig) {
    name = orig.name;
    func = orig.func;
}

ModuleLua::~ModuleLua() {}

void ModuleLua::addFunctionOnModule(const char* name, lua_CFunction f){
    this->name.push_back(name);
    this->func.push_back(f);
}

luaL_Reg* ModuleLua::getModule(){
    int end = name.size();
    luaL_Reg* r = new luaL_Reg[end+1];
    for(int i=0;i<end;i++){
        r[i].name = name[i].c_str();
        r[i].func = func[i];
    }
    //Add canary
    r[end].name = NULL;
    r[end].func = NULL;
    return r;
}

void ModuleLua::clear(){
    this->func.clear();
    this->name.clear();
}