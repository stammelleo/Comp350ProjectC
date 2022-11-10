// Team Awesome: Jesse Thompson, TJ Bourget, Sean Tammelleo, Craig Kimello
// COMP 350 - 001
// ProjectB

void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int,int,int,int);
void readFile(char*);
void executeProgram(char*);

void main()
{
    char buffer[13312];
    char* fname = "messag";
    int sectorsRead;
    char userInput[80];
    char fileInput[512];


    makeInterrupt21();
//	interrupt(0x21, 3, "messag", buffer, &sectorsRead);   /*read the file into buffer*/ 
//	if (sectorsRead>0)
//		interrupt(0x21, 0, buffer, 0, 0);   /*print out the file*/ 
//	else
//		interrupt(0x21, 0, "messag not found\r\n", 0, 0);  /*no sectors read? then print an error*/
//	while(1);   /*hang up*/ 

    //interrupt(0x21,1,userInput,0,0);
    //interrupt(0x21,0,userInput,0,0);
    //interrupt(0x21,2,fileInput,30,0);
    //interrupt(0x21,0,fileInput,0,0);
    interrupt(0x21,4,"tstpr1",0,0);
    while (1);
}

// Prints out the provided string to the console using interrupts
void printString(char* chars)
{
    int index = 0;
    while(chars[index] != 0x0)
    {
        interrupt(0x10, 0xe * 256 + chars[index], 0, 0, 0);
        index++;
    }
}

void readString(char* letter)
{
    int index = 0;
    //getting letter and printing to screen
    letter[index] = interrupt(0x16, 0, 0, 0,0);
    printChar(letter[index]);

    //check if enter key is pressed and under string limit but not zero
    while (letter[index] != 0xd  && 0 < index < 80)
    {
        char inputLetter = interrupt(0x16, 0, 0, 0, 0);
        //if input is backspace
        if (inputLetter == 0x8)
        {
            if(index >= 0)
            {
                index--;
                printChar(0x8);
                printChar(' ');
                printChar(0x8);
            }
        }
        else{
            index++;
            letter[index] = inputLetter;
            printChar(letter[index]);
        }
    }
    //once enter key pressed
    letter[index+1] = 0xa;
    letter[index+2] = 0x0;
    printChar(0xa);
}

void executeProgram(char* name)
{
	int index = 0;
	int numSectorsRead;
	char buffer[13312];
	readFile(name, buffer, &numSectorsRead);
	for (index = 0; index < 13312; index++)
	{
		putInMemory(0x2000, index, buffer[index]);
	}
	launchProgram(0x2000);
}

void printChar(char c)
{
    interrupt(0x10, 0xe*256+c, 0, 0, 0);
}

void readSector(char* buffer, int sector)
{
    int ah = 2;             // tells BIOS to read sector
    int al = 1;             // number of sectors to use
    int ax = ah * 256 + al;

    int ch = 0;             // track number
    int cl = sector + 1;    // relative sector number
    int cx = ch * 256 + cl;

    int dh = 0;             // head number
    int dl = 0x80;          // device number
    int dx = dh * 256 + dl;

    interrupt(0x13, ax, buffer, cx, dx);
}

void readFile(char* fileName, char* buffer, int* sectorsRead)
{
//    int printIndex; // Index used for printing out the characters of the directory
    int correctCharIndex; // Index used for comparing how many characters in fileName match with directory[fileEntry]
    int correctChars; // The number of matching characters when comparing fileName and directory[fileEntry]

    int sectorIndex; // Index used for reading what sectors a file is stored on

    // The following variables are used for padding out characters in fileName
    int i;
    int pad;

    int fileEntry; // fileEntry acts as an index for accessing the data inside the directory
    char directory[512];

    readSector(directory,2); // Directory is at sector 2

    // Padding out fileName with 0's
    pad = 0; // Pad is false
    for (i = 0; i < 6; ++i)
    {
        if (fileName[i] == '\r' || fileName[i] == '\n')
        {
            pad = 1;
        }
        if (pad == 1)
            fileName[i] = '\0';
    }
}
// Chooses the proper interrupt function call based on the value of 'ax'
void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    switch(ax)
    {
        case 0:
            printString(bx);
            break;
        case 1:
            readString(bx);
            break;
        case 2:
            readSector(bx, cx);
            break;
        case 3:
            readFile(bx, cx, dx);
	case 4:
	    executeProgram(bx);
        default:
            printString("No interrupt function correlated with AX number");

    }
}
