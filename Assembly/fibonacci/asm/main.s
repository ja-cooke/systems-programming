	AREA storage,DATA
array
	SPACE 4*20	; Declares a 20-word storage area
array_end

	AREA mainarea,CODE
	EXPORT asm_main
asm_main
	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
	
	MOV r2, #1
	MOV r3, #1
	
loop
	STR r3, [r0], #4

	B .			; Loop forever

	ALIGN
	END
