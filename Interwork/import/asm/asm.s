	AREA mainarea,CODE
	IMPORT report
	EXPORT fib

zero
	MOV r0, #0
	BL report
	B exit

one
	CMP r6, #0
	BEQ zero
	
	MOV r0, #1
	BL report
	B exit

fib
	STMFD sp!, {r4-r6, lr} ; preserve scratch registers

	MOV r6, r0 ; number of fib numbers to compute
	
	CMP r6, #3
	BCC one
	
	MOV r2, #1
	MOV r3, #1
	SUB r6, #2
	
	TST r6, #1
	BEQ loop
	B odd_offset
	
odd_offset
	ADD r3, r2, r3
	SUB r6, #1
	
	CMP r6, #0
	BHI loop
	B rep
	
loop
	ADD r2, r2, r3
	ADD r3, r2, r3
	SUB r6, #2
	
	CMP r6, #0
	BHI loop
	
rep
	MOV r0, r3
	BL report

exit
	LDMFD sp!, {r4-r6, lr}
	
	END
