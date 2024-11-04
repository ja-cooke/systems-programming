	AREA mainarea,CODE
	EXPORT calculate

	; 5x + y
	; 5.r0 + r1
calculate
	ADD r3, r0, r0, LSL #2
	ADD r3, r3, r1
	
	MOV r0, r3
	
	BX LR
	
	END
