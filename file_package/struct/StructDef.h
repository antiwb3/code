#ifndef _STRUCT_DEF_H
#define _STRUCT_DEF_H

#pragma pack ( push, 1 )
struct KItemNull
{
	int Left;
	int Top;
	int Width;
	int Height;
	int PosType;
	int EventID;
	int DisableScale;
	unsigned char LockShowAndHide;
	int ShowTipType;
	unsigned char TipRichText;
	int Alpha;
	char Script[128];
	int AniID;
	float AniSPercentX;
	float AniSPercentY;
	int AniDelayTime;
	unsigned char bEndPos;
	float AniEPercentX;
	float AniEPercentY;
	int AniID;
	float AniSPercentX;
	float AniSPercentY;
	int AniDelayTime;
	unsigned char AniCalcByTwoPoint;
	float AniEPercentX;
	float AniEPercentY;
};

struct Image : Item
{
	unsigned char MultiLine;
	int RowSpacing;
	int FontSpacing;
	unsigned char HAlign;
	unsigned char VAlign;
	char Script[128];
};

#pragma pack ( pop)

#endif // !_STRUCT_DEF_H
