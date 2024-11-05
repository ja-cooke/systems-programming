	AREA mainarea,CODE
	EXPORT process

process
	; preserve clobbered registers
	STMFD sp!, {r4-r8,lr}
	
	; Extract the input argument
	MOV r4, r0
	
	; Load C struct contents into registers
	LDR r5, [r4]
	LDR r6, [r4, #4]
	LDR r7, [r4, #8]
	LDR r8, [r4, #12]
	
	; Add integer data in structure and return it
	ADD r6, r5, r6
	STR r6, [r4, #4]
	
	; Call the callback function 
	; Input argument ptr
	MOV r0, r7
	BLX r8
	
	; Restore registers from stack
	LDMFD sp!, {r4-r8,lr}
	; Return to main.c
	BX lr

	END
