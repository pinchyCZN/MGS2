#include <windows.h>
#include <string.h>


char    wname[]="Metal Gear Solid 2 : SUBSTANCE";
BYTE	ON=1;
BYTE	OFF=0;
int		OPTIONS=0;

struct VERSIONS{
	char *name;
	int health_offset;
	int air_offset;
	int clip_offset;
	int weapons_offset;
	int items_offset;
	int weapons_pointer;
	int items_pointer;
	int no_dvd_offset;
};

/*
weapons
.004ABEA8: 9090909090909090               nop
.004ABEB0: A12C33A000                     mov          eax,[000A0332C]
.004ABEB5: 48                             dec          eax
.004ABEB6: A32C33A000                     mov          [000A0332C],eax
no dvd
.008DDDE1: B801000000                     mov          eax,1
*/

struct VERSIONS mgs_versions[]={
	{
		"MGS2 v1",
		0x0889485,	//health
		0x004BF1C7, //air
		0x004ABEB5, //clip
		0x0087B9E0, //weapons
		0x0087BA1F, //items
		0xA3E608,	//weapons pointer
		0xA3E610,	//items pointer
		0x008DCF81	//no dvd
	},
	{
		"MGS2 SSE v1",
		0x0088A875,	//health
		0x004BF287, //air
		0x004ABF35, //clip
		0x0087CDB0, //weapons
		0x0087CDEF, //items
		0xA45348,	//weapons pointer
		0xA45350,	//items pointer
		0x008E0311	//no dvd
	},
	{
		"MGS2 v2",
		0x0088B4B5,	//health
		0x004BF2C7, //air
		0x004ABFA5, //clip
		0x0087DA00, //weapons
		0x0087DA3F, //items
		0xA4D008,	//weapons pointer
		0xA4D010,	//items pointer
		0x008DDDE1	//no dvd
	},
	{
		"MGS2 SSE v2",
		0x0088C435,	//health
		0x004BF377,	//air
		0x004AC015, //clip
		0x0087E910, //weapons
		0x0087E94F, //items
		0xA53E08,	//weapons pointer
		0xA53E10,	//items pointer
		0x008E0C61	//no dvd
	}
};

char *weapons_text[]=
{
    "M9",
    "USP",
    "SOCOM",
    "PSG1",
    "RGB6",
    "NIKITA",
    "STINGER",
    "CLAYMORE",
    "C4", //     ;58
    "CHAFF G",
    "STUN G",
    "D MICROPHONE",
    "HF BLADE",
    "COOLANT",
    "RK5 74U",
    "MAGAZINE",
    "GRENADE",
    "M4",
    "PSG1 T",
    "D MICROPHONE",
    "BOOK"
};
char *items_text[]=
{
    "RATIONS",          //;0
    "BINOCS",
    "MEDICINE",
    "BANDAGE",
    "PENTAZEMIN",
    "BDU",
    "B ARMOUR",
    "STEALTH",
    "M DETECTOR",
    "SENSOR A",
    "SENSOR B",
    "N V G",
    "THERM G",
    "SCOPE",
    "DG CAMERA",
    "BOX1",
    "CIGS",
    "L CARD  ",           //;17
    "SHAVER",
    "PHONE",
    "CAMERA",
    "BOX2",
    "BOX3",
    "WETBOX",
    "RP SENSOR",
    "BOX4",
    "BOX5",
    "XXXXXX",
    "SOCOM SUPP",
    "RK SUPP",
    "CAMERA",
    "BANDANA",
    "DOGTAGS",         //;32
    "MO DISK",
    "USP SUPP",
    "SP WIG",
    "BLUE WIG",
    "ORANGE WIG",
    "WIG C",
    "WIG D",
};

int getkey()
{
	int i=0;
	i=getch();
	if(i==0 || i==0xE0)
	{
		i=getch();
	}
	return i&0xFF;

}
int getkey2()
{
	int i=0;
	if(kbhit())
	{
		i=getch();
		if(i==0 || i==0xE0)
			i=getch();
	}
	return i&0xFF;

}

void clrscr()
{
	DWORD nchars;
	HANDLE handle = GetStdHandle (STD_OUTPUT_HANDLE);
	COORD xy_first = { 0, 0 };
	FillConsoleOutputCharacter (handle, ' ',  80*50, xy_first, &nchars);
	SetConsoleCursorPosition (handle, xy_first);
}

void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle (STD_OUTPUT_HANDLE);
	COORD xy_first = { x, y };
	SetConsoleCursorPosition (handle, xy_first);
}
void getxpos()
{
	HANDLE handle = GetStdHandle (STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
	return cb.dwCursorPosition.X;
}
void getypos()
{
	HANDLE handle = GetStdHandle (STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
	return cb.dwCursorPosition.Y;
}


int read_proc_mem(char *windowname,int address,unsigned char *data,int len)
{
	HWND hwnd;
	HANDLE phandle;
	DWORD pid,bread;

	hwnd=FindWindow(0,windowname);
	if(hwnd==0){
		printf("error:cant find window\n");
		return FALSE;
	}
	GetWindowThreadProcessId(hwnd,&pid);

	phandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(phandle==0){
		printf("error:cant open process!\n");
		return FALSE;
	}
	ReadProcessMemory(phandle,address,data,len,&bread);
	CloseHandle(phandle);
	return TRUE;
}

int write_proc_mem(char *windowname,int address,unsigned char *data,int len)
{
	HWND hwnd;
	HANDLE phandle;
	DWORD pid,bread;
	hwnd=FindWindow(0,windowname);
	if(hwnd==0){
		printf("Error: Cannot Find Game Window!\n");
		return FALSE;
	}
	GetWindowThreadProcessId(hwnd,&pid);

	phandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(phandle==0){
		printf("Error: Cannot Open Game Process!\n");
		return FALSE;
	}

	WriteProcessMemory(phandle,address,data,len,&bread);
	CloseHandle(phandle);
	return TRUE;
}


BOOL GetKeyPress(int key)
{
return ((GetAsyncKeyState(key)>>15 & 1)==1);
}

int readlong(unsigned char *data)
{
	int i;
	int value;
	for(i=3;i>=0;i--)
	{
		value=value<<8;
		value|=data[i] & 0xFF;
	}
	return value;
}

int readshort(unsigned char *data)
{
	int value;
	value=data[1];
	value|=data[0]<<8;
	return value;
}
int get_version(int *ver)
{
	int i;
	int result=FALSE;
	for(i=0;i<sizeof(mgs_versions)/sizeof(struct VERSIONS);i++){
		const unsigned char sig[4]={0x66,0x8B,0x42,0x14};
		int address;
		unsigned char data[8]={0};
		address= -4 + mgs_versions[i].health_offset;
		if(read_proc_mem(wname,address,data,4)){
			if(0==memcmp(sig,data,4)){
				*ver=i;
				result=TRUE;
				break;
			}
		}
	}
	return result;
}
int inf_health(int enable,int ver_index)
{
	const unsigned char NOP[8]={0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};
	const unsigned char sub[4]={0x66,0x29,0x41,0x12};
	int offset=mgs_versions[ver_index].health_offset;
	if(enable)
		write_proc_mem(wname,offset,NOP,4);
	else
		write_proc_mem(wname,offset,sub,4);
	return 0;
}
int inf_air(int enable,int ver_index)
{
	const unsigned char NOP[2]={0x90,0x90};
	const unsigned char sub[2]={0x2B,0xE8};
	int offset=mgs_versions[ver_index].air_offset;
	if(enable)
		write_proc_mem(wname,offset,NOP,2);
	else
		write_proc_mem(wname,offset,sub,2);
	return 0;

}
int inf_weapons(int enable,int ver_index)
{
	const unsigned char NOP[8]={0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};
	const unsigned char dec[1]={0x48};
	const unsigned char sub[4]={0x66,0x29,0x14,0x41};
	int weapons_offset=mgs_versions[ver_index].weapons_offset;
	int clip_offset=mgs_versions[ver_index].clip_offset;
	if(enable){
		write_proc_mem(wname,clip_offset,NOP,1); //clip
		write_proc_mem(wname,weapons_offset,NOP,4); //weapons
	}
	else{
		write_proc_mem(wname,clip_offset,dec,1); //clip
		write_proc_mem(wname,weapons_offset,sub,4); //weapons
	}
	return 0;
}
int inf_items(int enable,int ver_index)
{
	unsigned char NOP[8]={0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};
	unsigned char sub[4]={0x66,0x29,0x14,0x41};
	int items_offset=mgs_versions[ver_index].items_offset;
	if(enable)
		write_proc_mem(wname,items_offset,NOP,4);
	else
		write_proc_mem(wname,items_offset,sub,4);
	return 0;
}
int get_ptr(int origin_ptr,int *out)
{
	int result=FALSE;
	unsigned char data[4]={0,0,0,0};
	if(read_proc_mem(wname,origin_ptr,data,4)){
		int tmp=readlong(data);
		if(tmp!=0){
			*out=tmp;
			result=TRUE;
		}
	}
	return result;
}
int enable_weapon(int ver_index,int weapon_index,int enable)
{
	const unsigned char ON[2]={1,1};
	const unsigned char OFF[2]={0xFF,0xFF};
	unsigned char data[4]={0};
	int weapons_ptr=0;
	int maxweapons_ptr=0;
	if(get_ptr(mgs_versions[ver_index].weapons_pointer,&weapons_ptr)){
		weapons_ptr+=2;
		maxweapons_ptr=weapons_ptr+0x48;
		if(enable){
			if(read_proc_mem(wname,maxweapons_ptr+(weapon_index*2),data,2))
				write_proc_mem(wname,weapons_ptr+(weapon_index*2),data,2);
		}
		else{
			write_proc_mem(wname,weapons_ptr+(weapon_index*2),OFF,2);
		}
	}
	return 0;
}
int enable_item(int ver_index,int item_index,int enable)
{
	const unsigned char ON[2]={1,0};
	const unsigned char OFF[2]={0xFF,0xFF};
	int items_pointer=0;
	if(get_ptr(mgs_versions[ver_index].items_pointer,&items_pointer)){
		items_pointer+=2;
		if(enable){
			write_proc_mem(wname,items_pointer+(item_index*2),ON,2);
		}
		else{
			write_proc_mem(wname,items_pointer+(item_index*2),OFF,2);
		}
	}
	return 0;
}
int level_card(int ver_index,int item_index,int inc)
{
	unsigned char data[2];
	int items_ptr=0;
	if(get_ptr(mgs_versions[ver_index].items_pointer,&items_ptr)){
		items_ptr+=2;
		if(read_proc_mem(wname,items_ptr+item_index*2,data,2)){
			int card;
			card=data[0];

			if(inc)
				card++;
			else
				card--;
			card&=0x1F;
			data[1]=0;
			data[0]=card;
			write_proc_mem(wname,items_ptr+(item_index*2),data,2);
		}
	}
	return 0;
}

int get_game_version(int *version)
{
	int i;
	int result=FALSE;
	for(i=0;i<sizeof(mgs_versions)/sizeof(struct VERSIONS);i++){
		const unsigned char sig[4]={0x66,0x8B,0x42,0x14};
		unsigned char data[4];
		int sig_offset= -4 + mgs_versions[i].health_offset;

		memset(data,0,4);
		if(read_proc_mem(wname,sig_offset,data,4)){
			if(0==memcmp(data,sig,4)){
				if(version!=0){
					result=TRUE;
					*version=i;
					break;
				}
			}
		}
	}
	return result;
}
int print_help()
{
	int version=0;
	clrscr();
	if(get_version(&version)){
		printf("version=%s\n",mgs_versions[version].name);
		printf("no dvd offset=0x%08X (change call to mov eax,1)\n",mgs_versions[version].no_dvd_offset);
	}else{
		printf("error unable to get game version\n");
	}
	printf("health offset=0x%08X\n",mgs_versions[version].health_offset);
	printf("items offset=0x%08X\n",mgs_versions[version].items_offset);
	printf("items pointer=0x%08X\n",mgs_versions[version].items_pointer);
	printf("weapons offset=0x%08X\n",mgs_versions[version].weapons_offset);
	printf("weapons pointer=0x%08X\n",mgs_versions[version].weapons_pointer);
	{
		int tmp=0;
		if(get_ptr(mgs_versions[version].items_pointer,&tmp))
			printf("items struct=0x%08X\n",tmp);
		if(get_ptr(mgs_versions[version].weapons_pointer,&tmp))
			printf("weapons struct=0x%08X\n",tmp);
	}
	printf("press space to switch between items and weapons\n");
	printf("keys 1,2,3,4 to toggle inf weapons,items,health,air\n");
	printf("\npress any key\n");
	getkey();
	clrscr();
	return 0;
}
int handle_keys(int version,int key)
{
	static int enable_inf_weapons=0;
	static int enable_inf_items=0;
	static int enable_inf_health=0;
	static int enable_inf_air=0;
	static int items_or_weapons=0;
	static int selection=0;

	switch(key){
	case 0x3B: //F1
		print_help();
		break;
	case 0x50: //down
		selection++;
		if(items_or_weapons){
			if(selection >= sizeof(items_text)/sizeof(char *))
				selection=0;
		}
		else{
			if(selection >= sizeof(weapons_text)/sizeof(char *))
				selection=0;
		}
		break;
	case 0x48: //up
		selection--;
		if(selection<0){
			if(items_or_weapons){
				selection=sizeof(items_text)/sizeof(char *);
				selection--;
			}
			else{
				selection=sizeof(weapons_text)/sizeof(char *);
				selection--;
			}
			if(selection<0)
				selection=0;
		}
		break;
	case 0x4b: //left
		if(items_or_weapons){
			if(17==selection)
				level_card(version,selection,FALSE);
			else
				enable_item(version,selection,FALSE);
		}
		else
			enable_weapon(version,selection,FALSE);
		break;
	case 0x4D: //right
		if(items_or_weapons){
			if(17==selection)
				level_card(version,selection,TRUE);
			else
				enable_item(version,selection,TRUE);
		}
		else
			enable_weapon(version,selection,TRUE);
		break;
	case '1':
		enable_inf_weapons=enable_inf_weapons ? FALSE:TRUE;
		break;
	case '2':
		enable_inf_items=enable_inf_items ? FALSE:TRUE;
		break;
	case '3':
		enable_inf_health=enable_inf_health ? FALSE:TRUE;
		break;
	case '4':
		enable_inf_air=enable_inf_air ? FALSE:TRUE;
		break;
	case ' ':
		items_or_weapons=items_or_weapons?FALSE:TRUE;
		selection=0;
		break;
	}
	printf("weapons=%i,items=%i,health=%i,air=%i\n",enable_inf_weapons,enable_inf_items,enable_inf_health,enable_inf_air);
	inf_weapons(enable_inf_weapons,version);
	inf_items(enable_inf_weapons,version);
	inf_health(enable_inf_health,version);
	inf_air(enable_inf_air,version);

	if(items_or_weapons){
		int i,j;
		int items_ptr=0;
		if(get_ptr(mgs_versions[version].items_pointer,&items_ptr)){
			items_ptr+=2;
			for(i=0;i<sizeof(items_text)/sizeof(char *);i++){
				unsigned char data[2];
				if(i>=25)
					gotoxy(40,i-25);
				if(selection==i)
					printf("-->");
				else
					printf("   ");
				printf("%s ",items_text[i]);
				
				read_proc_mem(wname,items_ptr+i*2,data,2);
				if(i==17)
					printf(" %i",data[0]);
				j=readshort(data);
				if((j==0xFFFF) || (j==0))
					printf(" (NO)");
				else
					printf(" (YES)");

				printf("\n");
			}
		}
	}
	else{
		int i,j;
		int weapons_ptr=0;
		if(get_ptr(mgs_versions[version].weapons_pointer,&weapons_ptr)){
			weapons_ptr+=2;
			for(i=0;i<sizeof(weapons_text)/sizeof(char *);i++){
				char data[2];
				if(selection==i)
					printf("-->");
				else
					printf("   ");
				printf("%s",weapons_text[i]);
				read_proc_mem(wname,weapons_ptr+i*2,data,2);
				if(readshort(data)==0xFFFF)
					printf(" (NO)");
				else
					printf(" (YES)");
				printf("\n");
			}
		}
	}
	return 0;
}

int main(int argc,char *argv[])
{

	int key=0;
	int version=0;
	do{
		clrscr();
		if(get_game_version(&version)){
			handle_keys(version,key);
		}
		else{
			printf("unable to get game version!\n");
		}
		key=getkey();
		if(key==VK_ESCAPE){
			printf("press ESC again to exit\n");
			key=getkey();
		}
	}while(key!=VK_ESCAPE);
	return 0;
}