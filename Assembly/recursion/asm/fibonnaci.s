	AREA mainarea,CODE
	EXPORT fib		
		
	; fibonacci number generator for fib(n), the nth fib number
	; r0 is input, r1 is output

fib
	STMFD sp!, {r4-r6,lr} ; these registers require preservation

	CMP r0, #2  
	BHI branch 	; if the input is >2 then branch
	
	MOV r1, #1	; always returns 1 if input<=2
	
	LDMFD sp!, {r4-r6,pc} ; earlier LR pushed straight to PC: exit
		
branch
	MOV r6, r0
	SUB r0, r6, #1	; set r0 to n-1

	BL fib 	; Call to find n-1
	MOV r4, r1	; output -> r4
	
	SUB r0, r6, #2	; set r0 to n-2
	
	BL fib 	; Call to find n-2
	MOV r5, r1	; output -> r5
	
	ADD r1, r4, r5 	; return r4+r5 (i.e. fib(n-1) + fin(n-2))
	
	LDMFD sp!, {r4-r6,pc} ; earlier LR pushed straight to PC: exit
	
	END