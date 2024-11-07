	AREA storage,DATA
array
	SPACE 4*20	; Declares a 20-word storage area
array_end

	AREA mainarea,CODE
	EXPORT asm_main
asm_main
	;input
	
	MOV r4, #20

	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
	
	MOV r2, #1	; fib(1) = 1
	
	CMP r4, #1
	BEQ one ; If n=1
	
	MOV r3, #1	; fib(2) = 1
	
	STMIA r0!, {r2, r3}
		
	CMP r4, #2 ; If n=2
	BEQ exit
	
	TST r4, #1
	BEQ loop
	B odd_offset

loop
	ADD r2, r2, r3
	ADD r3, r2, r3
	STMIA r0!, {r2, r3}
	
	CMP r0, r1
	BCC loop
	B exit
	
odd_offset
	ADD r3, r2, r3
	STMIA r0!, {r3}
	CMP r0, r1
	BCC loop
	B exit
	
one
	STMIA r0!, {r4}
	
exit
	B .			; Loop forever

	ALIGN
	END
