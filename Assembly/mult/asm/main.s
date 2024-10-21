	AREA mainarea,CODE
	EXPORT asm_main
asm_main

	; Inititilisations
	MOV r0, #2
	MOV r1, #0
	
	; Simple routine to multiply 10 by 13 and store it in r1
	;MOV r1, r0, LSL #4 		; R1 = R0 * 16
	;SUB r1, r1, r0, LSL #1 	; R1 = R1 - R0 * 2 = R0 *14
	;SUB r1, r1, r0				; R1 = R1 - R0 = R0 * 13
	
	; Solution from SPIRAL
	ADD r1, r0, r0, LSL #4		; R1 = R0 + R0 * 16 = R0 * 17
	SUB r1, r1, r0, LSL #2		; R1 = R1 - R0 * 4 = R0*17 - R0*4 = R0 * 13

	B .			; Loop forever

	END
