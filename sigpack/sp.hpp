#pragma once

#include "object.hpp"

Value firFiltNative(ObjectFactory* factory, int argc, Value* args);
Value iirFiltNative(ObjectFactory* factory, int argc, Value* args);
Value fir1Native(ObjectFactory* factory, int argc, Value* args);
Value delayNative(ObjectFactory* factory, int argc, Value* args);
Value gplotNative(ObjectFactory* factory, int argc, Value* args);
Value linspaceNative(ObjectFactory* factory, int argc, Value* args);
Value specgramNative(ObjectFactory* factory, int argc, Value* args);
Value pwelchNative(ObjectFactory* factory, int argc, Value* args);
