#pragma once

#include "object.hpp"

Value firFiltNative(ObjectFactory* factory, int argc, Value* args);
Value iirFiltNative(ObjectFactory* factory, int argc, Value* args);
Value fir1Native(ObjectFactory* factory, int argc, Value* args);
Value delayNative(ObjectFactory* factory, int argc, Value* args);
