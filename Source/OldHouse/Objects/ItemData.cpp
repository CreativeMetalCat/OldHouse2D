// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"

FItemData FItemData::ConvertFromPointer(FItemData* data)
{
    FItemData res;
    res.DisplaySprite = data->DisplaySprite;
    res.KeyId = data->KeyId;
    return res;
}
