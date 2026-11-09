#ifndef TRILOGY_EQ_ACTOR_LISTS_H
#define TRILOGY_EQ_ACTOR_LISTS_H

// Partial C++ declarations for methods whose original class layouts are not yet known.
// These types are for method calls only; do not instantiate them or take sizeof them.
// class EQItemList (layout + methods): trilogy/layout/eq_itemlist.h
#include "trilogy/layout/eq_itemlist.h"

class EQMoneyList
{
public:
    int find_money_actor_guess(int actor);
    int is_money_actor(int actor);
};

// class EQObject (layout + methods): trilogy/layout/eq_object.h
#include "trilogy/layout/eq_object.h"

// EQ_CharCreateS (with IsEvilRace_guess / IsClassNecromancerOrShadowKnight_guess): layout/eq_charcreate.h

#include "trilogy/layout/eq_charsel.h"

#endif
