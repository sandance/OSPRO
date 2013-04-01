#include <stdio.h>

extern void writescr(int ,int, char *);
extern void pmWriteScr(int,int,char *);

void clearscreen(int);
void writeln(char *);
extern void gdt_flush(int,int ,int ,int ,int,int);
extern void schedule();
void  enqueue(); 
void pmWriteln(int,int);
void createProcess(unsigned int  ,unsigned int, unsigned int, unsigned int ,unsigned int);
unsigned int *allocStack();

struct PCB {

	unsigned int esp;
	unsigned int ss;
};

extern void go(struct PCB *);
//Queue Functions





void p1();

void p2();
void p3();
void p4();

void p5();
void p6();
void p7();

void p8();
void p9();
void p10();






int size=10;
int rear=0;
int front=0;
struct PCB *q[11];

int ch;











/* The En-Queue Function */


void enqueue(struct PCB *pcb)
{

	rear = (rear + 1) % 11;
	q[rear] = pcb;
}



/* The D-Queue Function */


struct PCB *dequeue()
{
        struct PCB* ret;
	
	front = (front + 1) % 11;
	ret = q[front];
	return ret;
}








/* Define GDT entry */

struct gdt_entry
{
	unsigned short limit_low;
	unsigned short base_low;    /* Base value goes to three different direction */
	unsigned char  base_middle;
	unsigned char  access;
	unsigned char  attributes;
	unsigned char  base_high;
} __attribute__((packed));

struct gdt_ptr
{
	unsigned short  limit;
	unsigned int base;
}__attribute__((packed));

extern void _linker(struct gdt_ptr *);

struct gdt_entry gdt[5]; /*GDT with 5 entry*/
struct gdt_ptr gp; /* GDT pointer */


/* Descriptor in the Global Descriptor Table */

void initGDTEntry(int index, unsigned long base , unsigned long limit, unsigned char access,
			unsigned char attributes)
{
	/*descriptor base address */
	gdt[index].base_low =(base& 0xFFFF);
	gdt[index].base_middle=(base>>16) & 0xFF;
	gdt[index].base_high=(base>>24) & 0xFF;
	
	/* Descriptor Limits */
	gdt[index].limit_low =(limit & 0xFFFF);
	gdt[index].attributes=((limit >>16)& 0x0F);
	
	gdt[index].attributes |=(attributes & 0xF0);
	gdt[index].access = access;
	
}



void gdt_install()
{

	/* setup the GDT pointer and limit */
	
	gp.limit=(sizeof(struct gdt_entry)*5)-1;
	gp.base= (unsigned int)gdt;
	
	
	/* NULL descriptor */
	initGDTEntry(0,0,0,0,0);
	
	/* Code Segment Descriptor , limit 4GB,granularity 4Kb,
	32bit opcode*/
	
	/*gdt_set_gate(num,base,limit,access,gran)*/
	
	
	initGDTEntry(1,0,640*1024-1,0x9A,0x40);
	
	/* Data Segment */
	initGDTEntry(2,0,640*1024-1,0x92,0x40);
	
	/* stack segment*/
	initGDTEntry(3,0,640*1024-1,0x92,0x40);
	
	
	/* video segment */
	initGDTEntry(4,0xb8000,0xF9F,0x92,0x40); /* F9F hex value of 80*25*2-1*/
	

	
	
	/* Flush old GDT*/
	
	_linker(&gp);
	
	gdt_flush(8,16,24,16,16,32); /* 1x8 for CS
				   (2x8) for DS 
				   (3x8) for SS
				   4x8 for es */
	
	

	
}





void p1()
{
        int i=0;
        char *msg;

        msg="Process P1: 0";
        while(1)
        {

                i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(5,0,msg);
		schedule();


        }
}

		
void p2()
{
	int i=0;
	char *msg;
	msg="Process P2: 0";
	while(1)
	{
	
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(6,0,msg);
		schedule();
	}
}


void p3()
{
	int i=0;
	char *msg;
	
	msg="Process P3: 0";
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(7,0,msg);
		schedule();
	}
}

void p4()
{
	int i=0;
	char *msg;
	msg="Process P4: 0";
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(8,0,msg);
		schedule();
	}
}


void p5()
{
	int i=0;
	char *msg;
	msg="Process P5: 0";
	
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(9,0,msg);
		schedule();
	}
}



void p6()
{
	int i=0;
	char *msg;
	msg="Process P6: 0";
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(10,0,msg);
		schedule();
	}
}




void p7()
{
	int i=0;
	char *msg;
	msg="Process P7: 0";
	
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(11,0,msg);
		schedule();
	}
}

void p8()
{
	int i=0;
	char *msg;
	msg="Process P8: 0";
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(12,0,msg);
		schedule();
	}
}


void p9()
{
	int i=0;
	char *msg;
	msg="Process P9: 0";
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(13,0,msg);
		schedule();
	}
}


void p10()
{
	int i=0;
	char *msg;
	msg="Process P10:0";
	while(1)
	{
	
		
		i=(i+1)%10;
                msg[12] = '0' + i;
		writescr(14,0,msg);
		schedule();
	}
}




int row=0;

int nextstack =0;

unsigned int mystacks [10][1024];

unsigned int *allocStack()

{
	return mystacks[nextstack++];
}


		


struct PCB PCBs[10];

struct PCB *current;
struct PCB *pcb;


//struct pcb *dequeue();


int next_pcb = 0;
struct PCB *allocatePCB()
{

	
	return &(PCBs[next_pcb++]);
};



void createProcess (unsigned int  ds,unsigned int ss, unsigned int stackTop, unsigned int cs , 
  unsigned  int processEntry)
{

	        unsigned int *st;
		

		st= (unsigned int *) stackTop;
		st=st-1;
		*st=0x0000;
		st=st-1;
		*st=cs;
		st=st-1;
		
		*st=processEntry;
		st = st - 1;
		*st = 0;  //ebp
		st=st-1;
		*st=0;    //esp 
		st=st-1;
		*st=0;    //edi 
		st=st-1;
		*st=0;    //esi
		st=st-1;
		*st=0;    //edx 
		st=st-1;
		*st=0;    //ecx
		st=st-1;
		*st=0;    // ebx
		st=st-1;
		*st=0;    // eax
		st=st-1;
		*st=ds;   //ds
		st=st-1;
		*st=ds;   //es
		st=st-1;
		*st=ds;   //fs
		st=st-1;
		*st=ds;   //gs
		
		pcb=allocatePCB();
		
		pcb->ss=ss;
		pcb->esp= (unsigned int) st;
		
		enqueue(pcb);
				
		
}		

void writeln (char *s)
{

	int col=1;
	writescr(row,col,s);
	col++;
	row++;
	
	
}

/* Clear the Screen */
void clearscreen(int line)

{
	int i=line;
	
	while(i >= 0)
	{	
		writescr(i,0,"                                                                                ");
		i++;
		
		if(i>25)
		return;
		
	}
}

/* This one used to fill the screen with *  in protected mood */

void pmWriteln(int row,int seg)
{
	int j=row;
	while (j>0)
	{
		pmWriteScr(j,seg,"********************************************************************************");	
		j++;
		
		if(j>25)
		return;
	}
}




int main(void)
{
	int line =0;
	unsigned int *s;
	int wCol=5;
	clearscreen(line);
	writeln("Initializing OS");
	writeln("Setting up OS descriptor...");
	
	gdt_install();
	
	pmWriteScr(wCol,32,"                done");
	wCol++;
	
	pmWriteln(wCol,32);
	
	/*while(1);*/


	//program3  starts here
	
	clearscreen(0);
	writeln("Running Ten processes");
	

	front = rear = 0;
	next_pcb = 0;
	nextstack = 0;
	
	s=allocStack(); //  Allocating stack 
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p1);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p2);
/*		
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p3);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p4);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p5);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p6);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p7);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p8);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p9);
	
	s=allocStack();
	createProcess((unsigned int)16,(unsigned int) 24,(unsigned int) (s+1024),(unsigned int ) 8,(unsigned int) p10);
	
*/	
	current = dequeue();
			
	go(current);	
	
	
	while(1);	
	return 0;
} /*Main ends Here */



