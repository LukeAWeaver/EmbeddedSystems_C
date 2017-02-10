;;*****************************************************************************
;;
;;	Assembly language subroutine
;;
;;		Author: 		Gary J. Minden
;;		Organization:	KU/EECS/EECS 388
;;		Date:			2014-04-18
;;		Version:		1.0
;;
;;		Purpose:		Assembly language subroutine
;;
;;		Notes:			Modified by Brad Torrence
;;
;;*****************************************************************************
;;

;;
;;	This subroutine computes a value based on four input arguements.
;;	The arguments are assumed to be in CPU registers R0 - R3
;;	  (AKA A1 - A4)
;;

;;	Declare sections and external references

		.global		AsmCompute				; Declare entry point as a global symbol

		.text								; Program sections

AsmCompute:									; Entry point

;;
;;	Save necessary registers
;;
;;	Since this subroutine does not use registers other than R0 - R3,
;;	  and we do not call another subroutine, we don't need to save
;;	  any registers.
;;  If you modify the following code to use registers > R3,
;;    then you MUST save the registers to the stack before use,
;;    and you MUST restore these registers before returning to the caller

;;	Allocate local variables on the Stack
;;
;;	Since this subroutine does not have local variables,
;;	  no Stack space need be allocated.
;;

;;
;;	For demonstration, this subroutine computes the following:
;;        RESULT = R0 + R1 + R2 + R3

;;		adds	r0, r1
;;		adds 	r0, r2
;;		adds 	r0, r3




	push{R4,R5,R6,R7};

	MUL R4,R3,R0
	SDIV R4,R4,R3
	MUL R5,R2,R0
	ADD R4,R4,R5
	EOR R5,R4,R1
	ORR R4,R5,R2

	SDIV R5,R3,R2 ;;D/C
	MUL R6,R3,R0	;;D*A
	EOR R6,R0,R6	;;A^(D*A)
	ORR R5,R5,R6	;;(D/C)|(A^(D*A))
	EOR	R7,R2,R3
	ORR R5,R5,R7

	CMP R4,R5
	BGT	lbl1
	B	lbl2

lbl1:
	mov r0,r4
	B exit

lbl2:
	mov r0,r5
	B exit
exit:
	pop{R4,R5,R6,R7};





;;
;;	Return from the subroutine with the result in R0
;;
		BX			LR				; Branch to the program address in the Link Register

		.end
