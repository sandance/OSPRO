.intel_syntax noprefix
.global writescr,_Loop,_done_printing,pmWriteScr, schedule
.extern gp
.extern enqueue, dequeue
.extern struct PCB *current
.global _linker
.global _loadcs,_Loop1,_pm_print
.global gdt_flush,schedule, go	
writescr:			
        push    ebp   /*Set up  ebp so that we can get the argument of r, c and string*/	
        mov     ebp,esp /* the stack pointer */
	pushf	 

	push	eax		
        push	ebx
	
	push	edi
        push	esi

	push    ds
	push es
	push ss
	push fs
	push gs
	
	 
	mov eax,[ebp+8]  /* get the row value */
	imul eax,80  /* multiplying row with 80 and put the value at ecx*/
	
	add eax,[ebp+12] /* get the column value */
	imul eax,2	/* multiply the ecx address value by 2*/
	
	mov edi,0xB8000  /* move the offset value 
						/* add ecx sum value to the edx*/
	
	add edi,eax
	mov esi,[ebp+16] /* the string value*/
	cli
	cld
_Loop:		
	cmpb [esi],0
	jz  _done_printing
	movsb 
	movb es:[edi],31
	inc edi
	jmp _Loop
	



_done_printing:
	pop gs
	pop fs
	pop ss
	pop es
	pop ds
			
	pop esi
	pop edi
	pop ebx
	pop eax
	popf
	pop ebp
	ret
	
_linker:
	/*push ebp*/
	/*mov ebp, esp*/
	/*push eax*/
	mov eax,[esp+4]
	lgdt [eax]
	/*pop eax*/
	/*pop ebp*/
	ret
	

gdt_flush:

	
        push ebp   /*Set up  ebp so that we can get the argument of r, c and string*/	
        mov ebp, esp                /* mov ebp,esp the stack pointer */
 	 
		

	
	
	mov eax,[ebp+12] /*set up ds selector */ 
	mov ds, eax
	
	mov eax,[ebp+24] /*set up es selector*/
	mov es,eax
	
	mov eax,[ebp+20] /*set up fs selector*/
	mov fs, eax
	
	mov eax,[ebp+20] /* set up gs selector*/
	mov gs, eax
	
	mov eax, [ebp+16] /*set up ss selector*/
	mov ss,eax
	
	mov eax, [ebp+8] 
	push eax
	
	push OFFSET _loadcs
	retf
_loadcs:
        pop ebp
	ret
	
	
pmWriteScr:
	push ebp
	mov ebp,esp
	push eax
	push es
	push edi
	push esi
	
	mov eax,[ebp+8]  /* get the row value */
	imul eax,80
	imul eax,2
	
	mov es,[ebp+12] /* get the video segment value */
	
	/*mov edi,es*/  /*Using the video segment value */
	 mov edi,0      /* move the offset value*/ 
	
	
	add edi,eax
	mov esi,[ebp+16] /* the string value*/
	jmp _Loop1


_Loop1:		
	cmpb [esi],0
	jz  _pm_print
	movsb 
	movb es:[edi],31
	inc edi
	jmp _Loop1

_pm_print: 
	pop esi
	pop edi
	pop es
	pop eax
	pop ebp
	ret


go:    
	push ebp 
	mov ebp, esp
	mov eax,[ebp+8]   
	mov [current],eax /* deque from PCB and make it current */
 	mov esp, [eax]    
	mov ss , [eax+4]
	/*pop ss*/          /* No need to pop him */
	pop gs             /*gs*/
	pop fs             /*fs*/
	pop es             /*es*/
	pop ds             /*ds*/
	popad
	iret
	
	
	
schedule :

	//pushad			//save the state of the current process
	
	pushf			//save the state of the current process
	push cs
	push OFFSET  _afterSwitch
	pushad			//save all general purpose registers
	
	push ds                        // save esp registers in the current processes pcb
	push es
	push fs
	push gs
	mov eax, [current]
	push eax
	call enqueue;           // call the enqueue function from  C
	call dequeue;
	mov [current], eax
	mov esp,[eax]
	mov ss, [eax+4]	                 // dequeue pcb for the next process and make it current
	pop gs			// restore all 
	pop fs
	pop es
	pop ds
	popad
	iret
	  
_afterSwitch:
	// should get back to caller
	//return to caller	
	ret	
