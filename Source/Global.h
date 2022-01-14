#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

//CONSTEXPR
constexpr auto MAX_LIBRARY_CHARCOUNT = 4096;
constexpr auto MIN_FREQUENCY = 10;
constexpr auto MAX_FREQUENCY = 2000;
constexpr auto VERSION = "V. 1.0.0";

// User defined sizeof macro 
# define my_sizeof(type) ((WString *)(&type+1)-(WString*)(&type)) 

#endif