	AREA storage,DATA
array
	SPACE 4*21	; Declares a 20-word storage area
array_end

	AREA mainarea,CODE
	EXPORT asm_main
asm_main
	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
	
	MOV r2, #1
	MOV r3, #1
	
	STMIA r0!, {r2, r3}
	
loop
	ADD r2, r2, r3
	ADD r3, r2, r3
	STMIA r0!, {r2, r3}
	
	CMP r0, r1
	BCC loop

	B .			; Loop forever

	ALIGN
	END
