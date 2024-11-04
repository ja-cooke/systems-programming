	AREA storage,DATA
array
	SPACE 4*19	; Declares a 20-word storage area
array_end
	
	AREA mainarea,CODE
	IMPORT report
	EXPORT fib
	
fib
	STMFD sp!, {r4-r6, lr}

	LDR r4, =array
	LDR r5, =array_end
	
	MOV r2, #1
	MOV r3, #1
	
	STMIA r4!, {r2, r3}
	
loop
	ADD r2, r2, r3
	ADD r3, r2, r3
	STMIA r4!, {r2, r3}
	
	CMP r4, r5
	BCC loop
	
	MOV r0, r3
	
	BL report

	LDMFD sp!, {r4-r6, lr}
	
	END
