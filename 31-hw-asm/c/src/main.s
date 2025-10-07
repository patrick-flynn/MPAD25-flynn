        .org    $0800
        .section "CODE"
        .global main
main:   

	lda     #$0c	; neo_console_clear_screen
        sta     $ff01
	lda	#$02	; console group
	sta	$ff00	; api call, go

lp1:	lda	$ff00
	bne	lp1

			; neo_console_set_cursor_pos(0,0)
	stz	$ff04	;                           (0,
	stz	$ff05	;                              0)
	lda	#$07	; neo_console_set_cursor_pos
	sta	$ff01
	lda	#$02	; console group
	sta	$ff00	; api call, go	

lp2:	lda	$ff00
	bne	lp2

chars:
	ldx	#$00
loop:	lda	str,x	; get char
	beq	done	; null, we are finished
	sta	$ff04
	lda	#$06	; neo_console_write_char
	sta	$ff01
	lda	#$02	; console group
	sta	$ff00	; api call, go	
lp0:	lda	$ff00
	bne	lp0
	inx
	jmp	loop

done:	jmp	done

str:	.ascii   "Hello, world!"
