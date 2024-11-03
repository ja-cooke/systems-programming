	AREA storage,DATA
array
	SPACE 16*31	; Declares a 30x 32-bit storage area
array_end
	
	AREA mainarea,CODE
	EXPORT fib		
		
branch
	SUB r0, r2, #1
	
	MOV r3, r4
	MOV r2, r3
	
	B fib 	; Call to find n-1
	
	MOV r3, r5
	
	B fib 	; Call to find n-2
	
	MOV r4, r5
	ADD r2, r3, r4
	
	LDR r1, [r6], #-4
	MOV LR, r1
	BX LR		; exit
	
arrayStore

arrayEnd
		
fib
	LDR r6, =array
	LDR r7, =array_end

	MOV r1, LR
	
	STR r1, [r6], #4
	
	MOV r2, r0
	
	CMP r0, #2
	BHI branch
	
	MOV r5, #1
	
	LDR r1, [r6], #-4
	MOV LR, r1
	BX LR 		; exit
	
	END