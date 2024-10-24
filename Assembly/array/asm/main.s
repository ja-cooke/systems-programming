	AREA storage,DATA
array
	; Total is 232 for the example list
	;DCD 1, 7, 31, 193, 2, -5, 1
	DCD 5456, 894, 295, -456, -8754, -89, 6, -78, 5, 9
array_end

	AREA main,CODE
	EXPORT asm_main

mod
	; do something
	CMP r2, #0
	BMI negative
	BX LR
	
negative
	MVN r2, r2
	ADD r2, #1
	BX LR

asm_main
	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
 
	LDR r2, [r0], #4
	BL mod
	MOV r3, r2
	
loop
	LDR r2, [r0], #4
	BL mod
	
	ADD r3, r2, r3
	
	CMP r0, r1
	BLT loop
	
	B .			; Loop forever

	ALIGN
	END
