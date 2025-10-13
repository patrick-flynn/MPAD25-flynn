    .org    $0800
    .section "CODE"
    .global main
main:   
    ; neo_console_clear_screen
    jsr $fff7   ; api call
    .byte $02   ; console
    .byte $0c   ; clear_screen

    ; neo_console_set_cursor_pos(0,0)
    stz	$ff04	;  (0, 
    stz	$ff05	;     0)
    jsr $fff7
    .byte $02   ; console
    .byte $07   ; set_cursor_pos

chars:
    ldx	#$00
loop:
    lda	str,x	; get char
    beq	done	; null, we are finished
    ; neo_console_write_char
    sta	$ff04
    jsr $fff7
    .byte $02   ; console
    .byte $06   ; write_char
    inx
    bne	loop

done:
    jmp	done

str:	.ascii   "Hello, world!"
