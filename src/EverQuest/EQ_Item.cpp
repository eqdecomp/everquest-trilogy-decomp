// EQ_Item.cpp - EQ_Item methods: prices of an item (cost at 0x8C, in copper).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (from Mac debug map)

#include <string.h>
#include <stdio.h>

#include "trilogy/common.h"
#include "trilogy/item.h"
#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x004384e5
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (address range; TAKP: EQ_Item_ValueSRent_plat)
// __thiscall. The platinum part of the cost (0 when the cost is not positive).
s16 EQ_Item::ValueSRent_plat_guess(void)
{
    if (cost <= 0)
        return 0;
    return (s16)(cost / 1000);
}

// FUNCTION: TRILOGY 0x004384fc
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (address range; TAKP: EQ_Item_ValueSRent_gold)
// __thiscall. The gold part of the cost.
s16 EQ_Item::ValueSRent_gold_guess(void)
{
    if (cost <= 0)
        return 0;
    return (s16)(cost % 1000 / 100);
}

// FUNCTION: TRILOGY 0x0043851b
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (address range; TAKP: EQ_Item_ValueSRent_silver)
// __thiscall. The silver part of the cost.
s16 EQ_Item::ValueSRent_silver_guess(void)
{
    if (cost <= 0)
        return 0;
    return (s16)(cost % 100 / 10);
}

// FUNCTION: TRILOGY 0x00438538
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (address range; TAKP: EQ_Item_ValueSRent_copper)
// __thiscall. The copper part of the cost.
s16 EQ_Item::ValueSRent_copper_guess(void)
{
    if (cost <= 0)
        return 0;
    return (s16)(cost % 10);
}

// FUNCTION: TRILOGY 0x00438550
// Calculates the rate-adjusted price for buying a quantity of the item.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (Mac: EQ_Item::ValueBuy, low)
// __thiscall. Price of `count` items at the merchant's rate: cost * rate (times the equipment's own rate when
// above 1.0), rounded. 0 for a negative cost. The client's float -> int conversion is __ftol, which
// truncates like the CRT's _ftol.
int EQ_Item::ValueBuy(float rate, int count)
{
    float price;

    if (cost < 0)
        return 0;
    price = cost * rate;
    if (type == EQ_ITEM_EQUIPMENT && ((EQ_Equipment*)this)->sell_rate_guess > 1.0f)
        price = price * ((EQ_Equipment*)this)->sell_rate_guess;
    return (int)(count * price + 0.5f);
}

// FUNCTION: TRILOGY 0x004385a7
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (Mac: EQ_Item::ValueSell, low)
// __thiscall. What the merchant pays for `count` items: (cost / rate, rounded) * count; 0 for a negative cost.
int EQ_Item::ValueSell_guess(float rate, int count)
{
    if (cost < 0)
        return 0;
    return (int)(cost / rate + 0.5f) * count;
}

// FUNCTION: TRILOGY 0x004385d5
// Formats the merchant's payout for a quantity of the item as coin text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Item.cpp (Mac: EQ_Item::ValueSSell, low)
// __thiscall (item header lacks the declaration; __fastcall stand-in, EDX unused). Text of the sell price,
// e.g. " 1 platinum 2 gold", into the client's shared buffer at 0x0059207C.
char* __fastcall EQ_Item_ValueSSell_guess(EQ_Item* item, int edx, float rate, int count)
{
    char plat[16];
    char gold[16];
    char silver[16];
    char copper[16];
    int value;
    int p, g, s, c;

    if (item->cost <= 0)
        return " absolutely nothing";
    value = item->ValueSell_guess(rate, count);
    p = value / 1000;
    value -= p * 1000;
    g = value / 100;
    value -= g * 100;
    s = value / 10;
    c = value - s * 10;
    if (p > 0)
        sprintf(plat, " %d platinum", p);
    else
        strcpy(plat, EQ_ARRAY(char, 0x0058FBA4));
    if (g > 0)
        sprintf(gold, " %d gold", g);
    else
        strcpy(gold, EQ_ARRAY(char, 0x0058FBA4));
    if (s > 0)
        sprintf(silver, " %d silver", s);
    else
        strcpy(silver, EQ_ARRAY(char, 0x0058FBA4));
    if (c > 0)
        sprintf(copper, " %d copper", c);
    else
        strcpy(copper, EQ_ARRAY(char, 0x0058FBA4));
    sprintf(EQ_ARRAY(char, 0x0059207C), "%s%s%s%s", plat, gold, silver, copper);
    return EQ_ARRAY(char, 0x0059207C);
}

// FUNCTION: TRILOGY 0x00438701
// Formats the purchase price for a quantity of the item as coin text.
// ORIGINAL FILE: EverQuest/EQ_Item.cpp (from Mac debug map)
char* __fastcall EQ_Item_ValueSBuy_guess(EQ_Item* item, int edx, float rate, int count)
{
    char platinumText[16];
    char goldText[16];
    char silverText[16];
    char copperText[16];
    int remainingCopper;
    int platinumCount;
    int goldCount;
    int silverCount;

    if (item->cost <= 0)
        return EQ_ARRAY(char, 0x005591FC);
    remainingCopper = EQ_THISCALL(int, 0x00438550, (EQ_Item*, int, float, int))(item, edx, rate, count);
    platinumCount = remainingCopper / 1000;
    remainingCopper -= platinumCount * 1000;
    goldCount = remainingCopper / 100;
    remainingCopper -= goldCount * 100;
    silverCount = remainingCopper / 10;
    remainingCopper -= silverCount * 10;
    count = remainingCopper;
    if (platinumCount > 0)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(platinumText, EQ_ARRAY(char, 0x005591D8), platinumCount);
    }
    else
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(platinumText, EQ_ARRAY(char, 0x0058FBA4));
    }
    if (goldCount > 0)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(goldText, EQ_ARRAY(char, 0x005591CC), goldCount);
    }
    else
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(goldText, EQ_ARRAY(char, 0x0058FBA4));
    }
    if (silverCount > 0)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(silverText, EQ_ARRAY(char, 0x005591C0), silverCount);
    }
    else
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(silverText, EQ_ARRAY(char, 0x0058FBA4));
    }
    if (count > 0)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(copperText, EQ_ARRAY(char, 0x005591B4), count);
    }
    else
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(copperText, EQ_ARRAY(char, 0x0058FBA4));
    }
    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(EQ_ARRAY(char, 0x00591F7C), EQ_ARRAY(char, 0x005591A8), platinumText, goldText, silverText, copperText);
    return EQ_ARRAY(char, 0x00591F7C);
}

// FUNCTION: TRILOGY 0x0043882d
// Formats the item's reduced price in platinum, gold, silver, and copper.
// ORIGINAL FILE: EverQuest/EQ_Item.cpp (from Mac debug map)
char* __fastcall EQ_Item_ValueSRent_guess(EQ_Item* item, int edx)
{
    int originalCost;
    short coinCount;
    int scaledCost;
    char* priceText;
    char platinumText[16];
    char goldText[16];
    char silverText[16];
    char copperText[16];

    originalCost = item->cost;
    scaledCost = originalCost / 0xc;
    item->cost = scaledCost;
    if (scaledCost <= 0)
    {
        priceText = EQ_ARRAY(char, 0x005591FC);
    }
    else
    {
        coinCount = EQ_METHOD(0, short, 0x004384E5, (EQ_Item*), item)();
        if (coinCount > 0)
        {
            coinCount = EQ_METHOD(0, short, 0x004384E5, (EQ_Item*), item)();
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (platinumText, EQ_ARRAY(char, 0x005591D8), (int)coinCount);
        }
        else
        {
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (platinumText, EQ_ARRAY(char, 0x0058FBA4));
        }
        coinCount = EQ_METHOD(0, short, 0x004384FC, (EQ_Item*), item)();
        if (coinCount > 0)
        {
            coinCount = EQ_METHOD(0, short, 0x004384FC, (EQ_Item*), item)();
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (goldText, EQ_ARRAY(char, 0x005591CC), (int)coinCount);
        }
        else
        {
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (goldText, EQ_ARRAY(char, 0x0058FBA4));
        }
        coinCount = EQ_METHOD(0, short, 0x0043851B, (EQ_Item*), item)();
        if (coinCount > 0)
        {
            coinCount = EQ_METHOD(0, short, 0x0043851B, (EQ_Item*), item)();
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (silverText, EQ_ARRAY(char, 0x005591C0), (int)coinCount);
        }
        else
        {
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (silverText, EQ_ARRAY(char, 0x0058FBA4));
        }
        coinCount = EQ_METHOD(0, short, 0x00438538, (EQ_Item*), item)();
        if (coinCount > 0)
        {
            coinCount = EQ_METHOD(0, short, 0x00438538, (EQ_Item*), item)();
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (copperText, EQ_ARRAY(char, 0x005591B4), (int)coinCount);
        }
        else
        {
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (copperText, EQ_ARRAY(char, 0x0058FBA4));
        }
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (EQ_ARRAY(char, 0x00591F3C), EQ_ARRAY(char, 0x005591A8),
             platinumText, goldText, silverText, copperText);
        priceText = EQ_ARRAY(char, 0x00591F3C);
    }
    item->cost = originalCost;
    return priceText;
}
