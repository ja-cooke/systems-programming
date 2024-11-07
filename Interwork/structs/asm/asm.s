	AREA mainarea,CODE
	EXPORT process

process
	; preserve clobbered registers
	STMFD sp!, {r4-r8,lr}
	
	; Extract the input argument
	MOV r4, r0
	
	; Load C struct contents into registers
	;LDMIA r4, {r5,r6,r7,r8}
	LDR r5, [r4] 		; Callback
	LDR r6, [r4, #4]	; PTR
	LDR r7, [r4, #8]	; x
	LDR r8, [r4, #12]	; y
	
	; Add integer data in structure and return it
	ADD r8, r7, r8
	STR r8, [r4, #12]
	
	; Call the callback function 
	; Input argument ptr
	MOV r0, r6
	BLX r5
	
	; Restore registers from stack
	LDMFD sp!, {r4-r8,lr}
	; Return to main.c
	BX lr

	END
