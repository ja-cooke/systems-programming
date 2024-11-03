	AREA mainarea,CODE
	EXPORT asm_main
	
	IMPORT fib

asm_main

	MOV r0, #7
	
	BL fib 		; Your call to 'fib' goes here

	MOV r8, r0
	MOV r9, r0

	B .			; Loop forever

	ALIGN
	END
