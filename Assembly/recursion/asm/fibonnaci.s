	AREA mainarea,CODE
	EXPORT fib		
		
fib
	CMP r0, #2
	BHI branch
	
	MOV r1, #1
	
	BX	lr; exit
		
branch
	SUB r0, #1
	
	STMFD sp!, {r0,r4-r5,lr}
	BL fib 	; Call to find n-1
	LDMFD sp!, {r0,r4-r5,lr}
	
	MOV r4, r1
	
	SUB r0, #1
	
	STMFD sp!, {r0,r4-r5,lr}
	BL fib 	; Call to find n-2
	LDMFD sp!, {r0,r4-r5,lr}
	
	MOV r5, r1
	
	ADD r1, r4, r5
	
	BX 	lr; exit
	
	END