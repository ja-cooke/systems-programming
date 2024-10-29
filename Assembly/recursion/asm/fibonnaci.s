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
		
fib
	CMP LR, fib
	
	MOV r1, LR
	MOV r2, r0
	
	CMP r0, #2
	BHI branch
	
	MOV r5, #1
	MOV LR, r1
	
	BX LR
	
	END