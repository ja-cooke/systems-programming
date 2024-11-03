	AREA mainarea,CODE
	EXPORT asm_main
	
	IMPORT fib

asm_main

	MOV r0, #7
	
	BL fib 		; Your call to 'fib' goes here

	MOV r6, #9
	MOV r7, #9

	B .			; Loop forever

	ALIGN
	END
