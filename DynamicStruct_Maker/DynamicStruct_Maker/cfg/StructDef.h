#ifndef _STRUCT_DEF_H
#define _STRUCT_DEF_H

#pragma pack ( push, 1 )
struct Item
{
	unsigned char MultiLine;
	int RowSpacing;
	int FontSpacing;
	unsigned char HAlign;
	unsigned char VAlign;
	char Script[128];
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
